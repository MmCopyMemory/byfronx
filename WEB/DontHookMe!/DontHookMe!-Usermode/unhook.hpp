#include <windows.h>
#include <ImageHlp.h>
#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;
#pragma comment(lib,"imagehlp")
//
//
// If anything found that can be made better pls create a issue ^^
// Would highly appreciate!!! <3
//
//
void unhookuser32(const char* functionName) {

	HMODULE lib = LoadLibrary(L"C:\\Windows\\System32\\User32.dll");
	BYTE assemblyBytes[5] = {};

	if (lib) {
		void* fa = GetProcAddress(lib, functionName);
		if (fa) {
			BYTE* read = (BYTE*)fa;
			for (int i = 0; i < 5; i++) {
				assemblyBytes[i] = read[i];
			}
			WriteProcessMemory(GetCurrentProcess(), GetProcAddress(GetModuleHandle(L"User32"), functionName), (LPCVOID)assemblyBytes, 5, NULL);
			FreeLibrary(lib);
			printf("Yay!\n");
		}
		else
			printf("Function not found!\n");
	}
	else
		printf("Error loading library!\n");
}
void unhookntdll(const char* functionName) {

	HMODULE lib = LoadLibrary(L"C:\\Windows\\System32\\ntdll.dll");
	BYTE assemblyBytes[5] = {};

	if (lib) {
		void* fa = GetProcAddress(lib, functionName);
		if (fa) {
			BYTE* read = (BYTE*)fa;
			for (int i = 0; i < 5; i++) {
				assemblyBytes[i] = read[i];
			}
			WriteProcessMemory(GetCurrentProcess(), GetProcAddress(GetModuleHandle(L"ntdll"), functionName), (LPCVOID)assemblyBytes, 5, NULL);
			FreeLibrary(lib);
			printf("Yay!\n");
		}
		else
			printf("Function not found!\n");
	}
	else
		printf("Error loading library!\n");
}
void unhookkernel(const char* functionName) {

	HMODULE lib = LoadLibrary(L"C:\\Windows\\System32\\Kernel32.dll");
	BYTE assemblyBytes[5] = {};

	if (lib) {
		void* fa = GetProcAddress(lib, functionName);
		if (fa) {
			BYTE* read = (BYTE*)fa;
			for (int i = 0; i < 5; i++) {
				assemblyBytes[i] = read[i];
			}
			WriteProcessMemory(GetCurrentProcess(), GetProcAddress(GetModuleHandle(L"Kernel32"), functionName), (LPCVOID)assemblyBytes, 5, NULL);
			FreeLibrary(lib);
			printf("Yay!\n");
		}
		else
			printf("Function not found!\n");
	}
	else
		printf("Error loading library!\n");
}