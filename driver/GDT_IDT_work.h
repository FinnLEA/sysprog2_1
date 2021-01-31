#ifndef	_GDT_WORK_H_
#define _GDT_WORK_H_

#include "intel.h"

#include <ntddk.h>
#include <strsafe.h>


//-----------------------------------------------

typedef struct _GATE_ {
	volatile ULONG count;
	ULONG origHandler;
} GATE, *PGATE;

//-----------------------------------------------


#define MAKE_SELECTOR(index, RPL)   ((index << 3) + RPL)

extern PGATE pGatesArray;
extern PULONG glpOriginGateHandlers;
extern volatile ULONG gltmpOriginHandler;

//-----------------------------------------------

ULONG ClearWP ();
void WriteCR0 (
	ULONG reg
	);

//---------------------

VOID DumpGDT (
	OUT PVOID Buf, 
	OUT PULONG length
	);

VOID DumpIDT (
	OUT PVOID Buf, 
	OUT PULONG length
	);

//---------------------

VOID HookAllIDTGates();

ULONG HookIDTGate(
	PDescriptorGate hookedGate,
	ULONG index,
	ULONG HookAddr
	);

VOID HookRoutine();

VOID IncrementCount(
	ULONG index
	);

NTSTATUS AllocateAndInitGlArrays(
	PDescriptor idt,
	ULONG gateCount
	);

VOID FreeGlArrays();

//-----------------------------------------------

#endif