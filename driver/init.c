/****************************************************************************

    ������ init.c

****************************************************************************/

#include <ntddk.h>

#include "includes.h"

//-----------------------------------------------

#define	SYM_LINK_NAME   L"\\Global??\\Driver"
#define DEVICE_NAME     L"\\Device\\DDriver"


UNICODE_STRING  DevName;
UNICODE_STRING	SymLinkName;

//-----------------------------------------------

//-----------------------------------------------
// ��������������� ���������� �������


DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DriverUnload;




//-----------------------------------------------
// �������� �������

//
// ������� ������������� ��������.
//
NTSTATUS DriverEntry (IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {
	
	CHAR buf[1024];
	ULONG len = 0;	
	PDEVICE_OBJECT  DeviceObject;				// ��������� �� ������ ����������
	NTSTATUS		status = STATUS_SUCCESS;	// ������ ���������� �������

	DriverObject->MajorFunction [IRP_MJ_CREATE       ] = DispatchCreate;
    DriverObject->MajorFunction [IRP_MJ_CLOSE        ] = DispatchClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchControl;

    DriverObject->DriverUnload = DriverUnload;
	 RtlInitUnicodeString(&DevName, DEVICE_NAME);
    RtlInitUnicodeString(&SymLinkName, SYM_LINK_NAME);

    // �������� ����������
	status = IoCreateDevice(DriverObject,	// ��������� �� ������ ��������
                            0,				// ������ ������� �������������� ������ ����������
                            &DevName,		// ��� ����������
                            FILE_DEVICE_UNKNOWN,	// ������������� ���� ����������
                            0,				// �������������� ���������� �� ����������
                            FALSE,			// ��� ������������� �������
                            &DeviceObject); // ����� ��� ���������� ��������� �� ������ ����������
    if(!NT_SUCCESS(status))
        return status;

    DbgPrint("Create device %ws",DEVICE_NAME);

    // �������� ���������� ������
	status = IoCreateSymbolicLink(&SymLinkName, &DevName);
    if (!NT_SUCCESS(status)){
        IoDeleteDevice(DeviceObject);
        return status;
    }

    DbgPrint("Create symbolic link %ws",SYM_LINK_NAME);


	RtlZeroMemory(buf, 1024);
    //DumpGDT(buf, &len);

    return status;
}


//--------------------

//
// �������, ���������� ��� �������� ��������.
//
VOID DriverUnload (IN PDRIVER_OBJECT DriverObject) {
	
	IoDeleteSymbolicLink(&SymLinkName);
    IoDeleteDevice( DriverObject->DeviceObject);

#if DBG
    DbgPrint ("Driver unload\n");
#endif

    return;  	
}


//--------------------



//-----------------------------------------------
