#pragma once
#include "header.h"

#define STATUS_SUCCESS 0x00000000
#define STATUS_UNSUCCESSFUL 0xC0000001
#define STATUS_ACCESS_DENIED 0xC0000022

typedef struct _CLIENT_ID {
	PVOID UniqueProcess;
	PVOID UniqueThread;
}CLIENT_ID, *PCLIENT_ID;

typedef NTSTATUS(NTAPI *pLdrGetProcedureAddress)(HMODULE ModuleHandle, PANSI_STRING FunctionName OPTIONAL, WORD Ordinal OPTIONAL, PVOID *FunctionAddress);
typedef NTSTATUS(NTAPI *pLdrLoadDll)(PWSTR SearchPath OPTIONAL, PULONG LoadFlags OPTIONAL, PUNICODE_STRING ModuleName, HMODULE *pvBaseAddress OPTIONAL);
typedef NTSTATUS(NTAPI *pNtOpenProcess)(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId);
typedef NTSTATUS(NTAPI *pNtAllocateVirtualMemory)(HANDLE ProcessHandle, PVOID *BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect);
typedef NTSTATUS(NTAPI *pNtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);
typedef NTSTATUS(NTAPI *pRtlCreateUserThread)(HANDLE ProcessHandle, PSECURITY_DESCRIPTOR SecurityDescriptor OPTIONAL, BOOLEAN CreateSuspended, ULONG StackZeroBits, PULONG StackReserved, PULONG StackCommit, PVOID StartAddress, PVOID StartParameter OPTIONAL, PHANDLE ThreadHandle, PCLIENT_ID ClientId);

HANDLE NtOpenProcess(DWORD dwProcessId);