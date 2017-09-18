# NTAPI Injector

The project demonstrates how you can use Native API (NTAPI) functions such as NtAllocateVirtualMemory, NtWriteVirtualMemory and RtlCreateUserThread (you can use NtCreateThreadEx instead which performs a system call and becomes called anyway - the RtlCreateUserThread doesn't actually perform a system call in NTDLL). In the project I use it to demonstrate DLL injection.

If you wish to make the injection method rely entirely on NTAPI then use LdrLoadDll instead of LoadLibraryA via code-cave/shell-code injection. Or, manual map by using LdrpLoadDll.

Please use responsibly. I'm not responsible for how you use the code. 