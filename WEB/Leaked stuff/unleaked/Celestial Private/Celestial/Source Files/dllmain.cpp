#include <Windows.h>
#include <Task Scheduler/Task Scheduler.h>
#include <thread>
#include <iostream>

#include <Roblox/Memcheck/Memcheck.h>
#include <Utilities/Benchmark.h>
#include <SDK/SDK.h>
#include <Roblox/Hacks/Triggerbot/Triggerbot.h>

#include <Roblox/Direct X/Direct X.h>

#include <Renderer/Renderer.h>

#include <Themida/ThemidaSDK.h>
#include <Roblox/Bypasses/IsTainted/IsTainted.h>

#include <Roblox/Globals/Globals.h>

#include <DbgHelp.h>
#include <Xorstr/XorStr.h>
#include <ShlObj_core.h>

#pragma comment(lib, "dbghelp.lib")

Celestial::Scheduler RScheduler;
Celestial::Aimbot Aim;

void Console()
{
	DWORD old{};

	static const auto ptr = reinterpret_cast<std::uintptr_t>(&FreeConsole);
	static auto ptr_jmp = ptr + 0x6;

	VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, PAGE_EXECUTE_READWRITE, &old);
	*reinterpret_cast<std::uintptr_t**>(ptr + 0x2) = &ptr_jmp;
	*reinterpret_cast<std::uint8_t*>(ptr + 0x6) = 0xC3;

	VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, old, &old);

	FILE* file_stream{};

	AllocConsole();
	freopen_s(&file_stream, "CONIN$", "r", stdin);
	freopen_s(&file_stream, "CONOUT$", "w", stdout);
	freopen_s(&file_stream, "CONOUT$", "w", stderr);
	::SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

}

auto __stdcall DllMain(void*, std::uint32_t call_reason, void*) -> bool
{
	VM_TIGER_WHITE_START;

    if (call_reason != 1)
        return false;

    std::thread([] {

		PWSTR appdata = NULL;
		char dest[MAX_PATH];

		if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &appdata) == S_OK) {
			wcstombs(dest, appdata, MAX_PATH);
		}
		else {
			fprintf(stderr, "error getting appdata path\n");
		}

		std::string AppDataCelestial = std::string(std::string(dest) + "\\Celestial\\Configs");

		if (CreateDirectoryA(AppDataCelestial.c_str(), NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
		}

		Console();

		Celestial::SDK SDK;
		Celestial::DirectX Direct;

		std::thread([] {
			Aim.Update();
		}).detach();

    }).detach();

	VM_TIGER_WHITE_END;

    return true;
}
