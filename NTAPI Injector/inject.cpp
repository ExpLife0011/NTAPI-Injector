#include "header.h"

HANDLE InjectDll(INJECT_INFO InjectionInfo)
{
	HANDLE ProcessHandle = InjectionInfo.ProcessHandle;
	PVOID pvDllMemory = 0; // used later for memory allocation
	HANDLE ThreadHandle = 0; // used later to hold the thread handle
	SIZE_T DllCalc = strlen(InjectionInfo.DllPath); // calculation for DLL path lenth
	SIZE_T NumberOfBytesWritten = 0; // used for memory write
	CLIENT_ID ClientId; // needed for RtlCreateUserThread

	// check if the process handle exists
	if (!ProcessHandle)
	{
		// handle is invalid so check for a PID?
		if (!InjectionInfo.dwProcessId)
		{
			// no PID was specified either so stop right now
			return 0;
		}

		// attempt to use the PID to open a process handle
		ProcessHandle = NtOpenProcess(InjectionInfo.dwProcessId);

		// handle couldnt be opened
		if (!ProcessHandle)
		{
			return 0;
		}
	}

	// check if the injection type is for NTAPI usage or not
	if (InjectionInfo.bNativeInject)
	{

		// array of function addresses
		FARPROC fpAddresses[4] = { 
			GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"),
			GetProcAddress(GetModuleHandle("ntdll.dll"), "NtAllocateVirtualMemory"),
			GetProcAddress(GetModuleHandle("ntdll.dll"), "NtWriteVirtualMemory"),
			GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlCreateUserThread") 
		};
		
		// loop through the array
		for (FARPROC &fpAddress : fpAddresses)
		{
			if (!fpAddress) // address couldn't be obtained
			{
				// return nothing
				return 0;
			}
		}

		// setup for NTAPI functions
		pNtAllocateVirtualMemory fNtAllocateVirtualMemory = (pNtAllocateVirtualMemory)fpAddresses[1];
		pNtWriteVirtualMemory fNtWriteVirtualMemory = (pNtWriteVirtualMemory)fpAddresses[2];
		pRtlCreateUserThread fRtlCreateUserThread = (pRtlCreateUserThread)fpAddresses[3];

		// allocate memory with NtAllocateVirtualMemory
		if (NT_SUCCESS(fNtAllocateVirtualMemory(ProcessHandle, &pvDllMemory, NULL, &DllCalc, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
		{

			// write to allocated memory with NtWriteVirtualMemory . store DLL file path
			if (NT_SUCCESS(fNtWriteVirtualMemory(ProcessHandle, pvDllMemory, InjectionInfo.DllPath, DllCalc, NULL)))
			{

				// create the remote thread via RtlCreateUserThread . call LoadLibraryA . DLL injection
				if (NT_SUCCESS(fRtlCreateUserThread(ProcessHandle, NULL, FALSE, NULL, NULL, NULL, fpAddresses[0], pvDllMemory, &ThreadHandle, &ClientId)))
				{

					// check if the memory allocation exists
					if (pvDllMemory)
					{
						// free the memory we allocated before to hold the dll path
						VirtualFreeEx(ProcessHandle, pvDllMemory, DllCalc, MEM_RELEASE);
					}

					// close the handle to the process we injected into
					CloseHandle(ProcessHandle);

					return ThreadHandle; // success , return the thread handle
				}
			}
		}
	}
	else // win32 api
	{

		// get the address of kernel32!LoadLibraryA
		FARPROC fpLoadLibraryA = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

		// check if the address is valid
		if (fpLoadLibraryA)
		{

			// allocate memory to hold our dll path
			pvDllMemory = VirtualAllocEx(ProcessHandle, NULL, DllCalc, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (pvDllMemory) // allocation was successful
			{
				// write to the allocated memory
				if (WriteProcessMemory(ProcessHandle, pvDllMemory, InjectionInfo.DllPath, DllCalc, &NumberOfBytesWritten))
				{
					
					// create the remote thread
					ThreadHandle = CreateRemoteThread(ProcessHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)fpLoadLibraryA, pvDllMemory, NULL, NULL);
					if (ThreadHandle) // check if success
					{
						/*
							success
						*/

						if (pvDllMemory)
						{
							// free the memory
							VirtualFreeEx(ProcessHandle, pvDllMemory, DllCalc, MEM_RELEASE);
						}

						// close handle
						CloseHandle(ProcessHandle);

						// return the thread handle
						return ThreadHandle;
					}

				}
			}
		}
	}

	// check if the memory allocation exists
	if (pvDllMemory)
	{
		// free the memory
		VirtualFreeEx(ProcessHandle, pvDllMemory, DllCalc, MEM_RELEASE);
	}

	// close the handle to the process
	CloseHandle(ProcessHandle);

	return 0; // failed
}