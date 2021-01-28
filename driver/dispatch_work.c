#include "dispatch_work.h"


//-----------------------------------------------

PCHAR glWriteBuffer;
ULONG glSizeWriteBuffer;

//-----------------------------------------------

//-----------------------------------------------

// ������� ��� �������� ������� ����������
NTSTATUS DispatchCreate(IN PDEVICE_OBJECT DeviceObject,IN PIRP pIrp){
	DbgPrint("DispatchCreate\n");
    return CompleteIrp(pIrp,STATUS_SUCCESS,0); // ���������� IRP
}

//----------------------------------------

// ������� ��� �������� ������� ����������
NTSTATUS DispatchClose(IN PDEVICE_OBJECT DeviceObject,IN PIRP pIrp){

    return CompleteIrp(pIrp,STATUS_SUCCESS,0);
}

//----------------------------------------

// ������� ���������� �������� � IRP-�������
NTSTATUS CompleteIrp(PIRP Irp, NTSTATUS status,ULONG Info){

    Irp->IoStatus.Status = status;		// ������ ���������� ��������
    Irp->IoStatus.Information = Info;	// ���������� ����������� ����������
    IoCompleteRequest(Irp,IO_NO_INCREMENT);	// ���������� �������� �����-������
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
			len = outLen;
			DumpIDT(out, &len);

			len = outLen > len ? len : outLen;
			//RtlCopyMemory(out, buf, len);
			
			//ExFreePool(buf);

			info = len;

			break;
		
		default:
			Status = STATUS_INVALID_PARAMETER;
            break;
	}

	return CompleteIrp(pIrp,Status,info);
}

//----------------------------------------