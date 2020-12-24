#include "install_drv.h"

SC_HANDLE schSCManager;

RET_STATUS InstallDrv(PCCH szObjectName, PCCH szDriverLocation){
	RET_STATUS status = RET_SUCCESFUL;
	SC_HANDLE schService;	

	schSCManager = OpenSCManager(NULL, 
								 NULL,
								 SC_MANAGER_ALL_ACCESS
								 );

	schService = CreateService(schSCManager,
		szObjectName,
		szObjectName,
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		szDriverLocation,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if(schService == NULL){
		status = GetLastError();
		if (status == ERROR_SERVICE_EXISTS) {
			std::cout<<"failure: CreateService ERROR_SERVICE_EXISTS\n";
			//PrintLastErrorMessage();
			return status;
		}
		else{
			std::cout<<"failure: CreateService " <<std::hex<< status<<std::endl;
			//PrintLastErrorMessage();
			return status;
		}
	}
	else {
		std::cout<<"CreateService SUCCESS\n";
	}

	CloseServiceHandle(schService);

	schService = OpenService(schSCManager,
		szObjectName,
		SERVICE_ALL_ACCESS);

	if(schService == NULL){
		status = GetLastError();
		if (status == ERROR_SERVICE_EXISTS) {
			std::cout<<"failure: OpenDevice ERROR_SERVICE_EXISTS "<<std::hex<<status<<std::endl;
			//PrintLastErrorMessage();
			return status;
		}
	}
	else {
		std::cout<<"OpenDevice SUCCESS\n";
	}
	
	BOOL ret;
	ret = StartService(schService,
		0,
		NULL);
	if(!ret){
		status = GetLastError();
		if (status == ERROR_SERVICE_ALREADY_RUNNING) {
			std::cout<<"failure: StartService, ERROR_SERVICE_ALREADY_RUNNING"<<std::endl;
			//PrintLastErrorMessage();
			return status;
		}
		else {
			std::cout<<"failure: StartService "<<std::hex<<status<<std::endl;
			//PrintLastErrorMessage();
			return status;
		}
	}

	CloseServiceHandle(schService);

	return status;
}

RET_STATUS OpenDevice(PCCH szDriverName){
	RET_STATUS status = RET_SUCCESFUL;
	char completeDeviceName[MAX_PATH] = "";
	LPCTSTR dosDeviceName = szDriverName;
	HANDLE hDevice;
	BOOL ret;

	strcat(completeDeviceName, "\\\\.\\");
	strcat(completeDeviceName, dosDeviceName);

	hDevice = CreateFile(completeDeviceName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE) {
		status == GetLastError();
		std::cout<<"Can't get a handle to "<<completeDeviceName<<std::hex<<status<<std::endl;
		//PrintLastErrorMessage();
		return status;
	}
	else {
		std::cout<<"CreateFile SUCCESS\n";
		CloseHandle(hDevice);
	}

	return status;
}

RET_STATUS UninstallDrv(PCCH szDriverName){
	RET_STATUS status = RET_SUCCESFUL;
	SC_HANDLE schService;
	SERVICE_STATUS serviceStatus;
	BOOL ret;

	schSCManager = OpenSCManager(NULL, 
							 NULL,
							 SC_MANAGER_ALL_ACCESS
							 );

	schService = OpenService(schSCManager,
		szDriverName,
		SERVICE_ALL_ACCESS);

	if (schService == NULL) {
		status = GetLastError();
		std::cout<<"failure: OpenDevice in " <<std::hex<<status<<std::endl;
		return status;
	}

	ret = ControlService(schService,
		SERVICE_CONTROL_STOP,
		&serviceStatus);

	if(!ret) {
		status = GetLastError();
		std::cout<<"failure: DeleteService in "<<std::hex<<status<<std::endl;
		return status;
	}
	else {
		std::cout<<"ControlService SUCCESS\n";
	}

	CloseHandle(schService);

	//------------------

	schService = OpenService(schSCManager,
		szDriverName,
		SERVICE_ALL_ACCESS);
	
	if (schService == NULL) {
		status = GetLastError();
		std::cout<<"failure: OpenDevice in " <<std::hex<<status<<std::endl;
		return status;
	}

	ret = DeleteService(schService);
	if(!ret){
		status = GetLastError();
		std::cout<<"failure: DeleteService in "<<std::hex<<status<<std::endl;
		return status;
	}
	else {
		std::cout<<"ControlService SUCCESS\n";
	}

	CloseHandle(schService);

	return status;
}