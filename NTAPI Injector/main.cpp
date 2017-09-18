#include "header.h"

int main()
{
	INJECT_INFO InjectionInfo;
	InjectionInfo.ProcessHandle = NtOpenProcess(dwRetProcessId("TARGETPROCESS.exe"));
	InjectionInfo.dwProcessId = 0;
	InjectionInfo.bNativeInject = TRUE;
	InjectionInfo.DllPath = "TARGETDLLPATH";
	HANDLE ThreadHandle = InjectDll(InjectionInfo);
	if (ThreadHandle)
	{
		std::cout << "Success\n";
	}
	else
	{
		std::cout << "Failed" << GetLastError() << std::endl;
	}
	getchar();
	return 0;
}