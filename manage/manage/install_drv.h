#ifndef _INSTALL_DRV_H_
#define _INSTALL_DRV_H_

#include <iostream>
#include "codes.h"
#include <Windows.h>
#include "system.h"

extern SC_HANDLE schSCManager;

RET_STATUS InstallDrv(PCCH szObjectName, PCCH szDriverLocation);
RET_STATUS OpenDevice(PCCH szDriverName);
RET_STATUS UninstallDrv(PCCH szDriverName);

#endif // _INSTALL_DRV_H_