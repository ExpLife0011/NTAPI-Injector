#pragma once
#include "header.h"

typedef struct _INJECT_INFO {
	HANDLE ProcessHandle;
	DWORD dwProcessId;
	BOOL bNativeInject;
	LPSTR DllPath;
}INJECT_INFO, *PINJECT_INFO;

HANDLE InjectDll(INJECT_INFO InjectionInfo);