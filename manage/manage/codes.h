#ifndef _RET_ERROR_CODES_H_
#define _RET_ERROR_CODES_H_

#include <Windows.h>

#define RET_SUCCESFUL		0
#define RET_UNSUCCESSFUL	1

#define RET_SUCCESS(status) \
	~status;

typedef DWORD RET_STATUS;

#endif // _RET_ERROR_CODES_H_