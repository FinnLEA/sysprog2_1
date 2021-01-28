#ifndef _DISPATCH_WORK_H_
#define _DISPATCH_WORK_H_

#include <ntddk.h>
#include "GDT_IDT_work.h"

//-----------------------------------------------

#define DUMP_GDT_IOCTL  CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define DUMP_IDT_IOCTL  CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_READ_ACCESS | FILE_WRITE_ACCESS)

extern PCHAR glWriteBuffer;
extern ULONG glSizeWriteBuffer;

//-----------------------------------------------

NTSTATUS DispatchControl(PDEVICE_OBJECT DeviceObject, PIRP pIrp);
NTSTATUS CompleteIrp( PIRP Irp, NTSTATUS status,ULONG Info);
NTSTATUS DispatchCreate(IN PDEVICE_OBJECT DeviceObject,IN PIRP pIrp);
NTSTATUS DispatchClose(IN PDEVICE_OBJECT DeviceObject,IN PIRP pIrp);

//-----------------------------------------------

#endif //_DISPATCH_WORK_H_