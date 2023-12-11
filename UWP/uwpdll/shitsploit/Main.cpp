#include <Windows.h>
#include <thread>
#include "Roblox.hpp"
#include "Execution.hpp"

DWORD WINAPI input(PVOID lvpParameter)
{
	MessageBoxA(0, "hi", "hi", 0);
	std::string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	//                             PIPENAME
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ApeMonkey"),PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,PIPE_WAIT,1,999999,999999,NMPWAIT_USE_DEFAULT_WAIT,NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			Execution::execute_script(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

void entry() noexcept {
	try {
		//This is for making this dll work with pipe API 
		//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
		//I commented it out because I am not sure if the code works
		Execution::execute_script("printidentity()");
		Execution::execute_script("print(\"ByfronHQ on top\")");
		Execution::execute_script("print(\"github.com/dvlq/byfron\")");
		//This is using convert2bytecode method for execution
	}
    catch (const std::exception& error) {
        Roblox::print(3, "Niggerdll error: %s", error.what());
    }
}

bool __stdcall DllMain(HMODULE h_module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(h_module);
        std::thread(entry).detach();
    }
    return true;
}
