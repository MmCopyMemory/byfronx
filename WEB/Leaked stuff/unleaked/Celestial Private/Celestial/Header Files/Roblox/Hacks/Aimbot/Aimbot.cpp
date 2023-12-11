#pragma once

#include <../Renderer/Renderer.h>
#include <Roblox/Globals/Globals.h>

#include <Themida/ThemidaSDK.h>
#include <ImGui/imgui_impl_win32.h>

int aimbot_bind[] = {
VK_RBUTTON,
VK_LBUTTON,
VK_RSHIFT,
VK_LSHIFT,
VK_RCONTROL,
VK_LCONTROL,
0x41,
0x42,
0x43,
0x44,
0x45,
0x46,
0x47,
0x48,
0x49,
0x4A,
0x4B,
0x4C,
0x4D,
0x4E,
0x4F,
0x50,
0x51,
0x52,
0x53,
0x54,
0x55,
0x56,
0x57,
0x58,
0x59,
0x5A,
0x30,
0x31,
0x32,
0x33,
0x34,
0x35,
0x36,
0x37,
0x38,
0x39,
};

Celestial::World Roblox2;

void Celestial::Aimbot::Hook()
{
	/*Function to update everything outside of the render function*/
	/*Runs in sync with renderer*/

	this->Players = []() {
		std::vector<std::uintptr_t> ret;

		if (Celestial::Globals::Game == "CBRO")
		{
			for (std::uintptr_t _ : Roblox2.GetChildren(Globals::Workspace))
			{
				if (Roblox2.FindFirstChild(_, "HumanoidRootPart"))
				{
					if (!Globals::Teamcheck)
						ret.push_back(_);

					if (Globals::Teamcheck)
					{
						std::uintptr_t Player = Roblox2.FindFirstChild(Globals::Players, Roblox2.GetName(_));
						if (!Player)
							continue;

						if (Roblox2.GetTeam(Globals::LocalPlayer) != Roblox2.GetTeam(Player))
							ret.push_back(_);
					}
				}
			}
		}

		if (Celestial::Globals::Game == "Recoil")
		{
			for (std::uintptr_t _ : Roblox2.GetChildren(Globals::Workspace))
			{
				if (Roblox2.FindFirstChild(_, "HumanoidRootPart"))
				{
					if (!Globals::Teamcheck)
						ret.push_back(_);

					if (Globals::Teamcheck)
					{
						std::uintptr_t Player = Roblox2.FindFirstChild(Globals::Players, Roblox2.GetName(_).c_str());
						if (!Player)
							continue;

						std::uintptr_t LocalTeam = Roblox2.FindFirstChild(Roblox2.FindFirstChild(Globals::LocalPlayer, "GameStats"), "Team");

						if (!LocalTeam)
							continue;

						std::string LocalName = *reinterpret_cast<std::string*>(LocalTeam + 0x60);

						std::uintptr_t Team = Roblox2.FindFirstChild(Roblox2.FindFirstChild(Player, "GameStats"), "Team");

						if (!Team)
							continue;

						std::string OtherName = *reinterpret_cast<std::string*>(Team + 0x60);

						if (LocalName == "T.R.F")
						{
							if (OtherName == "Insurgents")
							{
								ret.push_back(_);
							}
						}

						if (LocalName == "Insurgents")
						{
							if (OtherName == "T.R.F")
							{
								ret.push_back(_);
							}
						}

					}

					//printf("%x\n", Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "GameStats"), "Team"));
				}



				//if (Roblox.GetName(_).c_str() != Roblox.GetName(Globals::LocalPlayer).c_str())
				//{
				//	if (Roblox.FindFirstChild(_, "FakeHead") && Globals::Teamcheck) // only ppl on opposite teams have fake heads :)
				//	{
				//		ret.push_back(_);
				//	}

				//	if (Roblox.FindFirstChild(_, "Head") && !Globals::Teamcheck)
				//	{
				//		ret.push_back(_);
				//	}
				//}
			}
		}


		if (Celestial::Globals::Game == "Phantom Forces")
		{
			if (Roblox2.GetName(Roblox2.GetTeam(Globals::LocalPlayer)) == "Ghosts")
			{
				for (std::uintptr_t InsideTeam : Roblox2.GetChildren(Roblox2.FindFirstChild(Roblox2.FindFirstChild(Globals::Workspace, "Players"), "Bright blue")))
					ret.push_back(InsideTeam);
			}

			if (Roblox2.GetName(Roblox2.GetTeam(Globals::LocalPlayer)) == "Phantoms")
			{
				for (std::uintptr_t InsideTeam : Roblox2.GetChildren(Roblox2.FindFirstChild(Roblox2.FindFirstChild(Globals::Workspace, "Players"), "Bright orange")))
					ret.push_back(InsideTeam);
			}
		}

		if (Celestial::Globals::Game == "Bad Business")
		{
			for (std::uintptr_t child : Roblox2.GetChildren(Roblox2.FindFirstChild(Globals::Workspace, "Characters")))
				if (!Roblox2.IsPlayerOnTeam(child) && child != Roblox2.IsLocalCharacter())
					ret.push_back(child);
		}

		if (Celestial::Globals::Game == "Rush Point")
		{
			for (std::uintptr_t Inside : Roblox2.GetChildren(Roblox2.FindFirstChild(Roblox2.FindFirstChild(Globals::Workspace, "MapFolder"), "Players")))
			{
				if (!Inside)
					continue;

				if (Roblox2.GetName(Inside) == "Combat Turrent") continue;

				if (Inside == Globals::LocalPlayer) continue;

				if (!Globals::Teamcheck)
					ret.push_back(Inside);

				if (Globals::Teamcheck)
				{
					std::uintptr_t LocalTeam = Roblox2.FindFirstChild(Globals::LocalPlayer, "PermanentTeam");

					if (!LocalTeam)
						continue;

					std::string LocalName = *reinterpret_cast<std::string*>(LocalTeam + 0x60);

					std::uintptr_t Team = Roblox2.FindFirstChild(Inside, "PermanentTeam");

					if (!Team)
						continue;

					std::string OtherName = *reinterpret_cast<std::string*>(Team + 0x60);

					if (Roblox2.GetName(Inside).c_str() != Roblox2.GetName(Globals::LocalPlayer).c_str())
					{
						if (LocalName == "Team1")
						{
							if (OtherName == "Team2")
							{
								ret.push_back(Inside);
							}
						}

						if (LocalName == "Team2")
						{
							if (OtherName == "Team1")
							{
								ret.push_back(Inside);
							}
						}
					}
				}
			}

		}

		if (Celestial::Globals::Game == "Frontlines")
		{
			for (std::uintptr_t Inside : Roblox2.GetChildren(Globals::Workspace))
			{
				if (Roblox2.GetName(Inside) == "soldier_model")
				{
					if (Roblox2.FindFirstChild(Inside, "friendly_marker") == 0)
					{
						ret.push_back(Inside);
					}
				}
			}
		}

		if (Celestial::Globals::Game == "Criminality")
		{
			for (std::uintptr_t child : Roblox2.GetChildren(Roblox2.FindFirstChild(Globals::Workspace, "Characters")))
				ret.push_back(child);
		}

		if (Celestial::Globals::Game == "Apoc 2")
		{
			for (std::uintptr_t child : Roblox2.GetChildren(Roblox2.FindFirstChild(Globals::Workspace, "Characters")))
				ret.push_back(child);
		}

		if (Celestial::Globals::Game == "th3ltgrounds")
		{
			for (std::uintptr_t Child : Roblox2.GetChildren(Globals::Workspace))
			{
				if (Roblox2.FindFirstChild(Child, "HumanoidRootPart"))
					ret.push_back(Child);
			}
		}

		if (Celestial::Globals::Game == "future")
		{
			for (std::uintptr_t Child : Roblox2.GetChildren(Globals::Workspace))
			{
				if (Roblox2.FindFirstChild(Child, "HumanoidRootPart"))
					ret.push_back(Child);
			}
		}


		if (Globals::Game == "Universal" || Globals::Game == "Riotfall" ||Globals::Game == "ProjDelta")
		{
			ret = Roblox2.GetChildren(Celestial::Globals::Players);
		}

		return ret;
	}();
}

using getpartobs_ins = float(__thiscall*)(std::uintptr_t Camera, std::vector<D3DXVECTOR3> CastPoints, std::vector<std::uintptr_t> IgnoreList, double a4, int a5);
const auto getpartobs = reinterpret_cast<getpartobs_ins>(reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr)) + 0xF8E4E0 - 0x890000);

D3DXVECTOR2 SmoothAim(D3DXVECTOR2 pos)
{
	D3DXVECTOR2 ClientDimensions = Roblox2.GetRobloxDimensions(Celestial::Globals::VisualEngine);
	D3DXVECTOR2 center = { ClientDimensions.x / 2, ClientDimensions.y / 2 };
	D3DXVECTOR2 target{ 0, 0 };

	if (pos.x != 0) {
		if (pos.x > center.x) {
			target.x = -(center.x - pos.x);
			target.x /= 1;
			if (target.x + center.x > center.x * 2)
				target.x = 0;
		}

		if (pos.x < center.x) {
			target.x = pos.x - center.x;
			target.x /= 1;
			if (target.x + center.x < 0)
				target.x = 0;
		}
	}

	if (pos.y != 0) {
		if (pos.y > center.y) {
			target.y = -(center.y - pos.y);
			target.y /= 1;
			if (target.y + center.y > center.y * 2)
				target.y = 0;
		}

		if (pos.y < center.y) {
			target.y = pos.y - center.y;
			target.y /= 1;
			if (target.y + center.y < 0)
				target.y = 0;
		}
	}

	target.x /= Celestial::Globals::Smoothness;
	target.y /= Celestial::Globals::Smoothness;

	if (abs(target.x) < 1) {
		if (target.x > 0) {
			target.x = 1;
		}
		if (target.x < 0) {
			target.x = -1;
		}
	}
	if (abs(target.y) < 1) {
		if (target.y > 0) {
			target.y = 1;
		}
		if (target.y < 0) {
			target.y = -1;
		}
	}

	return target;
}

float DistanceTo(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return sqrt(((from.x - to.x) * (from.x - to.x)) + ((from.y - to.y) * (from.y - to.y)) + ((from.z - to.z) * (from.z - to.z)));
}

// https://stackoverflow.com/questions/14029626/getting-a-specific-window-name-in-c
// Added a struct for easier return and passing of the PID
// One note. Fuck wstring with GetWindowTextW.
struct Window12
{
	DWORD PID;
	HWND Handle;
};

BOOL CALLBACK EnumWindowsProc12(HWND hwnd, LPARAM lParam)
{
	Window12& pid = *(Window12*)(lParam);

	DWORD WinId;
	GetWindowThreadProcessId(hwnd, &WinId);

	if (WinId == pid.PID)
		return FALSE;

	return TRUE;
}

// Find the main window of the program, returns 0 if not found
HWND FindMainWindow12()
{
	Window12 ToFind = { GetCurrentProcessId(), 0 };

	EnumWindows(EnumWindowsProc12, (LPARAM)&ToFind);

	return ToFind.Handle;
}

HWND main_window12 = 0;
HWND GetMainWindow12()
{
	// Set the proper scaling
	if (!main_window12)
		main_window12 = FindMainWindow12();

	return main_window12;
}

D3DXVECTOR2 Celestial::Aimbot::GrabClosestToCursor()
{
	D3DXVECTOR2 Off = { -1, -1 };
	int Short = 9e9;
	
	this->Hook();

	D3DXVECTOR3 CameraPosition = Roblox2.GetCameraPosition(Globals::VisualEngine);

	POINT CursorPosition;
	GetCursorPos(&CursorPosition);
	ScreenToClient(FindWindow(NULL, L"ROBLOX"), &CursorPosition);

	float scale = ImGui_ImplWin32_GetDpiScaleForHwnd(GetMainWindow12());
	scale = scale ? scale : 1.0f;  // If somehow 0?

	for (std::uintptr_t Player : this->Players)
	{
		if (Player != Globals::LocalPlayer)
		{
			if (Globals::Game == "Criminality" && (Roblox2.GetName(Player) == Roblox2.GetName(Globals::LocalPlayer))) continue;
			if (Globals::Game == "Universal" && Globals::Teamcheck && (Roblox2.GetTeam(Player) == Roblox2.GetTeam(Globals::LocalPlayer))) continue;
			if (Globals::Game == "Apoc 2" && (Roblox2.GetName(Roblox2.GetPlayerFromCharacter(Player)).c_str() == Roblox2.GetName(Globals::LocalPlayer))) continue;
			if (Globals::Game == "Riotfall" && Globals::Teamcheck && (Roblox2.GetTeam(Player) == Roblox2.GetTeam(Globals::LocalPlayer))) continue;

			if (Globals::Game == "th3ltgrounds" && (Roblox2.GetName(Roblox2.GetPlayerFromCharacter(Player)).c_str() == Roblox2.GetName(Globals::LocalPlayer))) continue;
			if (Globals::Game == "th3ltgrounds" && Globals::Teamcheck && (Roblox2.GetTeam(Roblox2.GetPlayerFromCharacter(Player)) == Roblox2.GetTeam(Globals::LocalPlayer))) continue;

			if (Globals::Game == "future" && (Roblox2.GetName(Roblox2.GetPlayerFromCharacter(Player)).c_str() == Roblox2.GetName(Globals::LocalPlayer))) continue;
			if (Globals::Game == "future" && Globals::Teamcheck && (Roblox2.GetTeam(Roblox2.GetPlayerFromCharacter(Player)) == Roblox2.GetTeam(Globals::LocalPlayer))) continue;

			std::uintptr_t Character;

			if (Globals::Game == "Universal" || Globals::Game == "Riotfall" || Globals::Game == "ProjDelta")
				Character = Roblox2.GetCharacter(Player);

			if (Globals::Game == "Riotfall")
				Character = Roblox2.GetCharacter(Player);

			if (Globals::Game == "Universal" && !Character) continue;
			if (Globals::Game == "Riotfall" && !Character) continue;


			if (Globals::Game == "Universal")
			{
				float Health = Roblox2.GetPlayerHealth(Roblox2.GetPlayerHumanoid(Player));
				if (Globals::Healthcheck && Health == 0) continue;
			}

			std::uintptr_t HumanoidRootPart;

			if (Globals::Game == "Universal" || Globals::Game == "ProjDelta")
				HumanoidRootPart = Roblox2.FindFirstChild(Character, "HumanoidRootPart");

			if (Globals::Game == "Recoil")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "CBRO")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "Criminality")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "Frontlines")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "Apoc 2")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "Riotfall")
				HumanoidRootPart = Roblox2.FindFirstChild(Character, "HumanoidRootPart");

			if (Globals::Game == "Rush Point")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "Phantom Forces")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "Torso");

			if (Globals::Game == "Bad Business")
				HumanoidRootPart = Roblox2.FindFirstChild(Roblox2.FindFirstChild(Player, "Body"), "Chest");

			if (Globals::Game == "Rush Point")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "th3ltgrounds")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "future")
				HumanoidRootPart = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (!HumanoidRootPart) continue;

			std::uintptr_t Head;

			if (Globals::Game == "Universal" || Globals::Game == "ProjDelta")
				Head = Roblox2.FindFirstChild(Character, "Head");

			if (Globals::Game == "CBRO")
				Head = Roblox2.FindFirstChild(Player, "Head");

			if (Globals::Game == "Recoil")
			{

				std::uintptr_t Head1 = Roblox2.FindFirstChild(Player, "Head1");
				if(Head1)
					Head = Roblox2.FindFirstChild(Player, "Head1");

				std::uintptr_t Head2 = Roblox2.FindFirstChild(Player, "Head2");
				if (Head2)
					Head = Roblox2.FindFirstChild(Player, "Head2");

				std::uintptr_t Head3 = Roblox2.FindFirstChild(Player, "Head3");
				if (Head3)
					Head = Roblox2.FindFirstChild(Player, "Head3");

				std::uintptr_t Head9 = Roblox2.FindFirstChild(Player, "Head9");
				if (Head9)
					Head = Roblox2.FindFirstChild(Player, "Head9");

				std::uintptr_t Head6 = Roblox2.FindFirstChild(Player, "Head6");
				if (Head6)
					Head = Roblox2.FindFirstChild(Player, "Head6");
			}


			if (Globals::Game == "Criminality")
				Head = Roblox2.FindFirstChild(Player, "Head");

			if (Globals::Game == "Frontlines")
				Head = Roblox2.FindFirstChild(Player, "HumanoidRootPart");

			if (Globals::Game == "Apoc 2")
				Head = Roblox2.FindFirstChild(Player, "Head");

			if (Globals::Game == "Riotfall")
				Head = Roblox2.FindFirstChild(Character, "HumanoidRootPart");

			if (Globals::Game == "Rush Point")
				Head = Roblox2.FindFirstChild(Player, "Head");

			if (Globals::Game == "Phantom Forces")
				Head = Roblox2.FindFirstChild(Player, "Head");

			if (Globals::Game == "Bad Business")
				Head = Roblox2.FindFirstChild(Roblox2.FindFirstChild(Player, "Body"), "Head");

			if (Globals::Game == "Rush Point")
				Head = Roblox2.FindFirstChild(Player, "UpperTorso");

			if (Globals::Game == "th3ltgrounds")
				Head = Roblox2.FindFirstChild(Player, "Head");

			if (Globals::Game == "future")
				Head = Roblox2.FindFirstChild(Player, "Head");

			if (!Head) continue;

			std::uintptr_t AimTarget;

			if (Globals::AimPart == 0)
				AimTarget = Head;
			else if (Globals::AimPart == 1)
				AimTarget = HumanoidRootPart;

			if (!AimTarget) continue;

			D3DXVECTOR3 Position;

			if (Globals::prediction)
			{
				D3DXVECTOR3 Velocity = Roblox2.GetPrimitiveVelocity(AimTarget) / Globals::p_strength;
				Position = AddVec3(Roblox2.GetPartPosition(AimTarget), Velocity);
			}

			if(!Globals::prediction)
				Position = Roblox2.GetPartPosition(AimTarget);

			float DistanceBetween = Roblox2.GetVector3Magnitude(SubVec3(this->Camera, Position));
			if (DistanceBetween == 0) continue;

			if (DistanceBetween < 7 && Globals::Game == "Rush Point") continue;

			D3DXVECTOR2 Screen;
			if (Roblox2.WorldToScreen(Position, Screen))
			{
			
				float DistanceToPlayer = DistanceTo(Roblox2.GetPartPosition(Roblox2.FindFirstChild(Roblox2.GetCharacter(Globals::LocalPlayer), "HumanoidRootPart")), Roblox2.GetPartPosition(HumanoidRootPart));
				D3DXVECTOR3 Pos = this->SubVec3(Roblox2.GetCameraPosition(Celestial::Globals::VisualEngine), Roblox2.GetPartPosition(HumanoidRootPart));


				float Magnitude = Roblox2.GetVector2Magnitude(SubVec2(Screen, {
					static_cast<float>(CursorPosition.x / scale),
					static_cast<float>(CursorPosition.y / scale)
				}));

				if ((Magnitude <= Globals::FOVValue) && (Magnitude < Short) && Roblox2.GetVector3Magnitude(Pos) <= Globals::LockDist)
				{
					Off = Screen;
					Short = Magnitude;
				}
			}
		}
	}

	return Off;
}

D3DXVECTOR2 Celestial::Aimbot::GetPlayerRelativeToCursor(D3DXVECTOR2 Position)
{
	D3DXVECTOR2 CPosition;

	POINT CursorPosition;
	GetCursorPos(&CursorPosition);
	ScreenToClient(FindWindow(NULL, L"ROBLOX"), &CursorPosition);

	float scale = ImGui_ImplWin32_GetDpiScaleForHwnd(GetMainWindow12());
	scale = scale ? scale : 1.0f;  // If somehow 0?

	CPosition.x = (Position.x - CursorPosition.x / scale) / Globals::Smoothness;
	CPosition.y = (Position.y - CursorPosition.y / scale) / Globals::Smoothness;

	return CPosition;
}


void Celestial::Aimbot::Update()
{
	__try {

		while (true && Globals::GameOpen)
		{
			if (!RenderSettings.Enabled)
			{
				do {
					Sleep(1);
				} while (!RenderSettings.Enabled);
			}

			if (GetAsyncKeyState(aimbot_bind[Globals::AimKey]) && Globals::AimMaster)
			{
				D3DXVECTOR2 ClosestToCursor = GrabClosestToCursor();
				if (ClosestToCursor.x == -1) continue;

				D3DXVECTOR2 PlayerRelativeToCursor = GetPlayerRelativeToCursor(ClosestToCursor);
				if (PlayerRelativeToCursor.x == -1) continue;

				INPUT input;
				input.mi.time = 0;
				input.type = INPUT_MOUSE;
				input.mi.mouseData = 0;
				input.mi.dx = PlayerRelativeToCursor.x - Globals::XOffset;
				input.mi.dy = PlayerRelativeToCursor.y - Globals::YOffset;
				input.mi.dwFlags = MOUSEEVENTF_MOVE;
				SendInput(1, &input, sizeof(input));
			}

			Sleep(1);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		Celestial::Aimbot::Update();
	}
}
