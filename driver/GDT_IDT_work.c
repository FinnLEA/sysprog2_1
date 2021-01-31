#include "GDT_IDT_work.h"


//----------------------------------------

PGATE pGatesArray;
PULONG glpOriginGateHandlers;
volatile ULONG gltmpOriginHandler;
ULONG glCR0reg;

//----------------------------------------

//----------------------------------------

ULONG ClearWP (void) {

ULONG reg;

    __asm {
        mov eax, cr0
        mov [reg], eax
        and eax, 0xFFFEFFFF
        mov cr0, eax
    }

    return reg;
}


//--------------------



void WriteCR0 (ULONG reg) {

    __asm {
        mov eax, [reg]
        mov cr0, eax
    }

}


//--------------------

VOID ShowCodeSeg (DescriptorCode *des, ULONG index, PVOID buf) {

ULONG base = des->BaseLow + (des->BaseMedium << 16) + (des->BaseHigh << 24);
ULONG limit = des->LimitLow + (des->LimitHigh << 16);

    StringCchPrintfA((STRSAFE_LPCSTR)buf, STRSAFE_MAX_CCH, "%3x   %08x - %08x   %d C%s\n", 
              MAKE_SELECTOR(index, des->DPL),
              base,
              base + (des->G ? (limit << 12) + 0xFFF : limit),
              des->DPL,
              des->D ? "32" : "16");

	//DbgPrint("buf: %s", (PCHAR)buf);

    return;
}


//--------------------


VOID ShowDataSeg (DescriptorData *des, ULONG index, PVOID buf) {

ULONG base = des->BaseLow + (des->BaseMedium << 16) + (des->BaseHigh << 24);
ULONG limit = des->LimitLow + (des->LimitHigh << 16);

    StringCchPrintfA((STRSAFE_LPCSTR)buf, STRSAFE_MAX_CCH, "%3x   %08x - %08x   %d D%s\n",
              MAKE_SELECTOR(index, des->DPL),
              base,
              base + (des->G ? (limit << 12) + 0xFFF : limit),
              des->DPL,
              des->B ? "32" : "16");


    return;
}


//--------------------


VOID ShowSystemSeg (DescriptorSystem *des, ULONG index, PVOID buf) {

ULONG base = des->BaseLow + (des->BaseMedium << 16) + (des->BaseHigh << 24);
ULONG limit = des->LimitLow + (des->LimitHigh << 16);

    switch (des->Type) {
        case TYPE_AVAILABLE_TSS_286:
        case TYPE_BUZY_TSS_286:
        case TYPE_AVAILABLE_TSS_386:
        case TYPE_BUZY_TSS_386:
            StringCchPrintfA((STRSAFE_LPCSTR)buf, STRSAFE_MAX_CCH, "%3x   %08x - %08x   %d TSS\n", 
                      MAKE_SELECTOR(index, des->DPL),
                      base,
                      des->G ? (limit << 12) + 0xFFFF : limit,
                      des->DPL);
            break;
        case TYPE_LDT:
            StringCchPrintfA((STRSAFE_LPCSTR)buf, STRSAFE_MAX_CCH, "%3x   %08x - %08x   %d LDT\n", 
                      MAKE_SELECTOR(index, des->DPL),
                      base,
                      des->G ? (limit << 12) + 0xFFFF : limit,
                      des->DPL);
            break;
        case TYPE_CALLGATE_286:
        case TYPE_TASKGATE:
        case TYPE_INTERRUPTGATE_286:
        case TYPE_TRAPGATE_286:
        case TYPE_CALLGATE_386:
        case TYPE_GATE_RESERVED:
        case TYPE_INTERRUPTGATE:
        case TYPE_TRAPGATE_386:
            {
            DescriptorGate *gate = (DescriptorGate*) des;
            ULONG offset = gate->DestinationOffsetLow + (gate->DestinationOffsetHigh << 16);
            StringCchPrintfA((STRSAFE_LPCSTR)buf, STRSAFE_MAX_CCH, "%3x   %02x:%08x   %d gate\n", 
                      MAKE_SELECTOR(index, des->DPL),
                      gate->DestinationSelector,
                      offset,
                      gate->DPL);
            }
            break;
        }

    return;
}


//--------------------

//
// Отображает записи GDT.
//
VOID DumpGDT (
	OUT PVOID Buf, 
	OUT PULONG length
){

	GDTR gdtr;
	ULONG i;
	ULONG segCount;
	Descriptor *gdt;
	PCHAR tmpBuf;
	ULONG needLen = *length;
	
	*length = 0;

	tmpBuf = ExAllocatePool(NonPagedPool, 256);
	if(!tmpBuf){
		DbgPrint("Error allocate pool");
		return;
	}
	RtlZeroMemory(tmpBuf, 256);
    __asm {sgdt gdtr}

    segCount = (gdtr.Limit + 1) / sizeof(Descriptor);
    gdt = (Descriptor *) gdtr.Base;

    DbgPrint ("GDT (%08X) contains %d segments\n", gdtr.Base, segCount);

    for (i = 0; i < segCount; ++i) {
		ULONG tmpLen = 0;
        if (!gdt[i].SecurityByteDetail.P) {
            continue;
        }

        if (gdt[i].SecurityByteDetail.SystemOrUser) {
            if (gdt[i].Data.CodeOrData) {
                ShowCodeSeg (&gdt[i].Code, i, tmpBuf);
			}
            else {
                ShowDataSeg (&gdt[i].Data, i, tmpBuf);
			}
        }
        else {
            ShowSystemSeg (&gdt[i].System, i, tmpBuf);
        }

		tmpLen = strlen(tmpBuf);
		if(tmpLen + *length > needLen){
			tmpLen = needLen - 	*length - 1;
			strncat(Buf, tmpBuf, tmpLen);
			RtlZeroMemory(tmpBuf, 256);
			*length = needLen;
			break;
		} 
		else{
			*length += tmpLen;
			strcat(Buf, tmpBuf);
			RtlZeroMemory(tmpBuf, 256);
		}
	}
	//*length = strlen(Buf);
	ExFreePool(tmpBuf);
	//DbgPrint("%s\n", Buf);
    return;
}

//--------------------


VOID DumpIDT (
	OUT PVOID Buf, 
	OUT PULONG length
) {
	
	IDTR idtr;
	ULONG i;
	ULONG segCount;
	PDescriptor idt;
	PCHAR tmpBuf;
	ULONG needLen = *length;
	
	*length = 0;

	tmpBuf = ExAllocatePool(NonPagedPool, 256);
	if(!tmpBuf){
		DbgPrint("Error allocate pool");
		return;
	}
	RtlZeroMemory(tmpBuf, 256);
    __asm {sidt idtr}

    segCount = (idtr.Limit + 1) / sizeof(Descriptor);
    idt = (Descriptor *) idtr.Base;

    DbgPrint ("IDT (%08X) contains %d segments\n", idtr.Base, segCount);

	for (i = 0; i < segCount; ++i) {
		ULONG tmpLen = 0;
        if (!idt[i].SecurityByteDetail.P) {
            continue;
        }
		
		if (idt[i].SecurityByteDetail.SystemOrUser) {
			StringCchPrintfA(tmpBuf, STRSAFE_MAX_CCH, "SecurityByteDetail.SystemOrUser == 1\n");
		}
		else {
			ShowSystemSeg (&idt[i].System, i, tmpBuf);
		}
		
		tmpLen = strlen(tmpBuf);
		if(tmpLen + *length > needLen){
			tmpLen = needLen - 	*length - 1;
			strncat(Buf, tmpBuf, tmpLen);
			RtlZeroMemory(tmpBuf, 256);
			*length = needLen;
			break;
		} 
		else{
			*length += tmpLen;
			strcat(Buf, tmpBuf);
			RtlZeroMemory(tmpBuf, 256);
		}

	}

	ExFreePool(tmpBuf);
	
	return;
}

NTSTATUS AllocateAndInitGlArrays(
	PDescriptor idt,
	ULONG gateCount
) {
	
	NTSTATUS status = STATUS_SUCCESS;
	ULONG i;
	PDescriptorSystem des;
	PDescriptorGate gate;
	
	pGatesArray = (PGATE)ExAllocatePool(NonPagedPool, sizeof(GATE) * gateCount);
	if(pGatesArray == NULL) {
		DbgPrint("Error allocate in AllocateAndInitGlArrays\n");
		return STATUS_UNSUCCESSFUL;
	}
	RtlZeroMemory(pGatesArray, sizeof(GATE) * gateCount);

	for(i = 0; i < gateCount; ++i) {
		ULONG handleAddr;
		if (!idt[i].SecurityByteDetail.P) {
            continue;
        }
		
		des = (PDescriptorSystem)(&idt[i].System);
		if(des->Type == TYPE_TASKGATE) {
			continue;
		}
		
		gate = (PDescriptorGate)des;
		pGatesArray[i].origHandler = gate->DestinationOffsetLow + (gate->DestinationOffsetHigh << 16);
		DbgPrint("%3d: 0x%08X", i, pGatesArray[i].origHandler);
	}

}

VOID FreeGlArrays() {
	ExFreePool(pGatesArray);
}

VOID HookAllIDTGates() {
	IDTR idtr;
	ULONG i;
	ULONG segCount;
	PDescriptor idt;
	
	__asm {sidt idtr}
	segCount = (idtr.Limit + 1) / sizeof(Descriptor);
    idt = (Descriptor *) idtr.Base;

	if(!NT_SUCCESS(AllocateAndInitGlArrays(idt, segCount))) {
		return;
	}
	
}


/*
 * 
 * return адрес оригинального обработчика
*/
ULONG HookIDTGate(
	PDescriptorGate hookedGate,
	ULONG index,
	ULONG HookAddr
) {
	
	return 0;
}

__declspec(naked)
static VOID CallStub(){}

__declspec(naked)
VOID HookRoutine(){
	__asm {
		pushad
		pushfd
		
		push 0x11111111
		call IncrementCount

		popfd
		popad
		
		jmp CallStub
	}
}



VOID IncrementCount(
	ULONG index
) {
	
	InterlockedExchangeAdd(&(pGatesArray[index].count), 1); 
	return;
}




//-----------------------------------------------