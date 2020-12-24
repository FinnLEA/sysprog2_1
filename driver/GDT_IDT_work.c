#include "GDT_IDT_work.h"


//-----------------------------------------------

PCHAR glWriteBuffer;
ULONG glSizeWriteBuffer;

//-----------------------------------------------

//-----------------------------------------------

// функция для открытия объекта устройства
NTSTATUS DispatchCreate(IN PDEVICE_OBJECT DeviceObject,IN PIRP pIrp){
	DbgPrint("DispatchCreate\n");
    return CompleteIrp(pIrp,STATUS_SUCCESS,0); // Завершение IRP
}

//----------------------------------------

// функция для закрытия объекта устройства
NTSTATUS DispatchClose(IN PDEVICE_OBJECT DeviceObject,IN PIRP pIrp){

    return CompleteIrp(pIrp,STATUS_SUCCESS,0);
}

//----------------------------------------

// функция завершения операции с IRP-пакетом
NTSTATUS CompleteIrp(PIRP Irp, NTSTATUS status,ULONG Info){

    Irp->IoStatus.Status = status;		// статус завершении операции
    Irp->IoStatus.Information = Info;	// количество возращаемой информации
    IoCompleteRequest(Irp,IO_NO_INCREMENT);	// завершение операции ввода-вывода
    return status;
}

//----------------------------------------

NTSTATUS DispatchControl(PDEVICE_OBJECT DeviceObject, PIRP pIrp){
	
	NTSTATUS Status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIrpStack;
	ULONG info = 0;
	ULONG inLen;
	ULONG outLen;
	ULONG len;
	PUCHAR in;
	PUCHAR out;
	ULONG ioctlCode;
	PCHAR buf;

	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	
	ioctlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	inLen = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	outLen = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	if ((ioctlCode & 0x00000003) == METHOD_BUFFERED) {
		out = in = pIrp->AssociatedIrp.SystemBuffer;
	}
	else {
		in = pIrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
		out = pIrp->UserBuffer;
	}

	switch (ioctlCode){
		case DUMP_GDT_IOCTL:
			/*ExAllocatePool(buf, 1024);
			RtlZeroMemory(buf, 1024);
			*/
			len = outLen;
			DumpGDT(out, &len);

			len = outLen > len ? len : outLen;
			//RtlCopyMemory(out, buf, len);
			
			//ExFreePool(buf);

			info = len;
			break;

		case DUMP_IDT_IOCTL:
			break;
		
		default:
			Status = STATUS_INVALID_PARAMETER;
            break;
	}

	return CompleteIrp(pIrp,Status,info);
}


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
			tmpLen = needLen - 	*length;
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

//-----------------------------------------------