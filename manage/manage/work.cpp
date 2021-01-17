#include "work.h"


/*
	Набор команд:
	0) .dump [-g | -i] (system dump (GDT \ IDT))
	1) .hookcount <[-s | -f] | -i> -- start or finish | information IDT interrupts
	2) .hookcontext <[-s | -f] | -i> -- start or finish | information IDT interrupts
	3) .hooksysenter <[-s | -f] | -i <number>> -- start or finish | information sysenter
	4) .memory <[-r | -w] <addr>> -- read \ write at the specified address
	5) .port <addr size> -- read I\O port
*/

PCHAR glUsageCommand[COUNT_COMMAND] = {
	{"Usage: .dump <[-g | -i]> (system dump (GDT \\ IDT))"},
	{"Usage: .hookcount <[-s | -f] | -i> -- start or finish | information IDT interrupts"},
	{"Usage: .hookcontext <[-s | -f] | -i> -- start or finish | information IDT interrupts"},
	{"Usage: .hooksysenter <[-s | -f] | -i <number>> -- start or finish | information sysenter"},
	{"Usage: .memory <[-r | -w] <addr>> -- read \\ write at the specified address"},
	{"Usage: .port <addr size> -- read I\\O port"},
	{"Usage: .exit for exit from programm"},
	{"Wrong command! .help for command list"}
};

CHAR glDriverName[128];


VOID StartListenCommand(VOID){
	std::string command;

	while(true) {
		std::getline(std::cin, command);
		int found = 0;
		if(FIND_COMMAND(command, ".dump")){
			if(FIND_COMMAND_ARG(command, "-g")){
				string outStr;
				ULONG outLen = 0x1000;
				if(!GetDump(DUMP_GDT_IOCTL, outStr, &outLen)){
					cout << "Error get dump GDT\n";
				}
				else{
					cout << "\n\n------------------Dump GDT : ------------------\n\n"
						<< outStr << endl
						<< "\n\n-----------------------------------------------\n\n";
				}
			}
			else if (FIND_COMMAND_ARG(command, "-i")){
				string outStr;
				ULONG outLen = 0x1C00;
				if(!GetDump(DUMP_IDT_IOCTL, outStr, &outLen)){
					cout << "Error get dump IDT\n";
				}
				else{
					cout << "\n\n------------------Dump IDT : ------------------\n\n"
						<< outStr << endl
						<< "\n\n-----------------------------------------------\n\n";
				}
			}
			else{
				PRINT_USAGE(0);
			}
		}
		else if(FIND_COMMAND(command, ".hookcount")){
			if(FIND_COMMAND_ARG(command, "-s")){}
			else if(FIND_COMMAND_ARG(command, "-f")){}
			else if(FIND_COMMAND_ARG(command, "-i")){}
			else {
				PRINT_USAGE(1);
			}
		}
		else if(FIND_COMMAND(command, ".hookcontext")){
			if(FIND_COMMAND_ARG(command, "-s")){}
			else if(FIND_COMMAND_ARG(command, "-f")){}
			else if(FIND_COMMAND_ARG(command, "-i")){}
			else {
				PRINT_USAGE(2);
			}
		}
		else if(FIND_COMMAND(command, ".hooksysenter")){
			if(FIND_COMMAND_ARG(command, "-s")){}
			else if(FIND_COMMAND_ARG(command, "-f")){}
			else if(FIND_COMMAND_ARG(command, "-i")){}
			else {
				PRINT_USAGE(3);
			}
		}
		else if(FIND_COMMAND(command, ".memory")){
			if(FIND_COMMAND_ARG(command, "-r")){
				smatch match;
				if(regex_search(command, match, regex(".*(0x[0-9a-zA-Z]+){1,}"))){
					//cout << match[1].first.ptr << endl;
					string addr = match[1];
					addr = addr.c_str() + 2;
					cout << addr << endl;
				}
				else {
					PRINT_USAGE(4);
				}
			}
			else if(FIND_COMMAND_ARG(command, "-w")){
				smatch match;
				if(regex_search(command, match, regex(".*(0x[0-9a-zA-Z]+){1,}"))){
					//cout << match[1].first.ptr << endl;
					string addr = match[1];
					addr = addr.c_str() + 2;
					cout << addr << endl;
				}
				else {
					PRINT_USAGE(4);
				}
			}
			else {
				PRINT_USAGE(4);
			}
		}
		else if(FIND_COMMAND(command, ".port")){
			smatch match;
			if(regex_search(command, match, regex(".*(0x[0-9a-zA-Z]+){1,}"))){
				//cout << match[1].first.ptr << endl;
				string addr = match[1];
				addr = addr.c_str() + 2;
				cout << addr << endl;
			}
			else {
				PRINT_USAGE(5);
			}
		}
		else if(FIND_COMMAND(command, ".help")){
			PRINT_ALL_USAGE();
		}
		else if(FIND_COMMAND(command, ".exit")){
			break;
		}
		else {
			PRINT_ALL_USAGE();
		}
	}
}


BOOL GetDump(DWORD code, string& outStr, PULONG len){
	PCH Buf = (PCH)malloc(*len);
	//ULONG len = 1024;
	HANDLE hFile;
	
	
	hFile = CreateFileA(glDriverName,
                   GENERIC_READ | GENERIC_WRITE, 
                   FILE_SHARE_READ | FILE_SHARE_WRITE, 
                   NULL, 
                   OPEN_EXISTING, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL);
	
	if(hFile == INVALID_HANDLE_VALUE){
		cout << "Error open device " << glDriverName << endl;
		return FALSE;
	}

	if( !DeviceIoControl(hFile,
                        code,
                        NULL,
                        0,
                        Buf,
                        *len,
                        len,
						NULL) ) {

        printf("Error read\n");
		return FALSE;
	}

    Buf[*len] = 0;
    outStr = Buf;

	CloseHandle(hFile);

	return TRUE;
}