#ifndef	_GDT_WORK_H_
#define _GDT_WORK_H_

#include "intel.h"
#include "work_includes.h" 

#include <ntddk.h>
#include <strsafe.h>

//-----------------------------------------------


#define MAKE_SELECTOR(index, RPL)   ((index << 3) + RPL)


//-----------------------------------------------



VOID DumpGDT (
	OUT PVOID Buf, 
	OUT PULONG length
	);

VOID DumpIDT (
	OUT PVOID Buf, 
	OUT PULONG length
	);




//-----------------------------------------------

#endif