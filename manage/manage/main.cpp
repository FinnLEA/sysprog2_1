#include <iostream>
#include "install_drv.h"
#include "codes.h"
#include "work.h"

#define NAME_HEADER "\\\\.\\"

int main(int argc, char** argv){
	RET_STATUS status = RET_SUCCESFUL;

	if(argc < 3){
		std::cout<<"Usage manage -i <Driver_name> <.sys location>(to start - install driver) -d <Driver_name> (to finish uninstall driver)"<<std::endl;
		return RET_UNSUCCESSFUL;
	}

	for(int i = 1; i < argc; ){
		if(!strcmp(argv[i], "-i")) {
			if(argc < 4){
				std::cout<<"Usage manage -i <Driver_name> <.sys location>(to start - install driver) -d <Driver_name> (to finish uninstall driver)"<<std::endl;
				return RET_UNSUCCESSFUL;
			}
			if(status = InstallDrv(argv[i+1], argv[i+2]) != RET_SUCCESFUL) {
				return status;
			}
			if(status = OpenDevice(argv[i+2]) != RET_SUCCESFUL) {
				return status;
			}
			break;
		}
		else if(!strcmp(argv[i], "-d")){
			if(status = UninstallDrv(argv[i+1]) != RET_SUCCESFUL) {
				return status;
			}
			return status;
			break;
		}
		else{
			++i;
		}
	}
	
	memcpy(glDriverName, NAME_HEADER, sizeof(NAME_HEADER));
	strcat(glDriverName, argv[2]);
	StartListenCommand();

	return 0;
}