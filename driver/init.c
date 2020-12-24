/****************************************************************************

    Модуль init.c

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
// предварительное объявление функций


DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DriverUnload;




//-----------------------------------------------
// описание функций

//
// Функция инициализации драйвера.
//
NTSTATUS DriverEntry (IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {
	
	CHAR buf[1024];
	ULONG len = 0;	
	PDEVICE_OBJECT  DeviceObject;				// указатель на объект устройство
	NTSTATUS		status = STATUS_SUCCESS;	// статус завершения функции

	DriverObject->MajorFunction [IRP_MJ_CREATE       ] = DispatchCreate;
    DriverObject->MajorFunction [IRP_MJ_CLOSE        ] = DispatchClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchControl;

    DriverObject->DriverUnload = DriverUnload;
	 RtlInitUnicodeString(&DevName, DEVICE_NAME);
    RtlInitUnicodeString(&SymLinkName, SYM_LINK_NAME);

    // создание устройства
	status = IoCreateDevice(DriverObject,	// указатель на объект драйвера
                            0,				// размер области дополнительной памяти устройства
                            &DevName,		// имя устройства
                            FILE_DEVICE_UNKNOWN,	// идентификатор типа устройства
                            0,				// дополнительная информация об устройстве
                            FALSE,			// без эксклюзивного доступа
                            &DeviceObject); // адрес для сохранения указателя на объект устройства
    if(!NT_SUCCESS(status))
        return status;

    DbgPrint("Create device %ws",DEVICE_NAME);

    // создание символьной ссылки
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
// Функция, вызываемая при выгрузке драйвера.
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
