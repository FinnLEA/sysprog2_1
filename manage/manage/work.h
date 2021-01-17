#ifndef _WORK_H_
#define _WORK_H_

#include <windows.h>
#include <string>
#include <iostream>
#include "codes.h"
#include "system.h"
#include <regex>

using namespace std;
using namespace tr1;
//using namespace ;

//-----------------------------------------------

#define FIND_COMMAND(inCommand, findCommand) \
	(inCommand.find(findCommand) != std::string::npos)

#define FIND_COMMAND_ARG(inCommand, findArg) \
	(inCommand.find(findArg) != std::string::npos)

#define PRINT_ALL_USAGE()	\
	cout << "\n"; \
	for(int index = 0; index < COUNT_COMMAND - 1; ++index){ \
		cout << glUsageCommand[index] << endl; \
	} \
	cout << "\n";

#define PRINT_USAGE(index) \
	cout << "\n"; \
	cout << glUsageCommand[index] << endl; \
	cout << "\n";

#define DUMP_GDT_IOCTL  CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define DUMP_IDT_IOCTL  CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_READ_ACCESS | FILE_WRITE_ACCESS)


#define COUNT_COMMAND 8

extern PCHAR glUsageCommand[8];
extern CHAR glDriverName[128];

//-----------------------------------------------

//-----------------------------------------------

VOID StartListenCommand(VOID);
BOOL GetDump(DWORD code, string& outStr, PULONG len);

//-----------------------------------------------

#endif //_WORK_H_