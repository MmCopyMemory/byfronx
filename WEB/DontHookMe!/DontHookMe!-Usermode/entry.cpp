#include "unhook.hpp"
//All includes even the one for main part are in unhook.hpp to keep clean :P

int main() {
	/*
	unhookuser32 {
	Unhook User32.dll functions!
	}
	unhookntdll {
	Unhook ntdll.dll functions!
	}
	unhookkernel {
	Unhook Kernel32.dll functions!
	}
	Info {
	Add more stuff in unhook.hpp :P
	}
	*/
	//Example :P
	printf("MessageBoxA\n");
	unhookuser32("MessageBoxA");
	
	printf("NtWriteVirtualMemory\n");
	unhookntdll("NtWriteVirtualMemory");

	printf("NtReadVirtualMemory\n");
	unhookntdll("NtReadVirtualMemory");

	printf("WriteProcessMemory\n");
	unhookkernel("WriteProcessMemory");

	printf("ReadProcessMemory\n");
	unhookkernel("ReadProcessMemory");

	printf("VirtualAllocEx\n");
	unhookkernel("VirtualAllocEx");

	printf("OpenProcess\n");
	unhookkernel("OpenProcess");

	printf("CreateRemoteThread\n");
	unhookkernel("CreateRemoteThread");

	printf("VirtualFree\n");
	unhookkernel("VirtualFree");
}