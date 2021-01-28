#include "GDT_IDT_work.h"


//----------------------------------------

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

//-----------------------------------------------