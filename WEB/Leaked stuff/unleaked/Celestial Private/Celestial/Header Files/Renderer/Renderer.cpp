
#include "Renderer.h"
#include <Roblox/Globals/Globals.h>
#include "Ryleigh/Ryleigh.h"

#include <Task Scheduler/Task Scheduler.h>
#include <ImGui/imgui_internal.h>
#include <Themida/ThemidaSDK.h>
#include <Roblox/World/BrickColor/ColorMap.h>

#include <array>
#include <ImGui/imgui_impl_win32.h>

Celestial::World Roblox;
Ryleigh* Engine;
Celestial::Scheduler bb;
CustomColorMap BrickColor;

D3DXVECTOR3 AddVec3(D3DXVECTOR3 operand1, D3DXVECTOR3 operand2) {
	return {
		operand1.x + operand2.x,
		operand1.y + operand2.y,
		operand1.z + operand2.z
	};
}

D3DXVECTOR3 SubVec3(D3DXVECTOR3 operand1, D3DXVECTOR3 operand2) {
	return {
		operand1.x - operand2.x,
		operand1.y - operand2.y,
		operand1.z - operand2.z
	};
}

D3DXVECTOR2 SubVec2(D3DXVECTOR2 operand1, D3DXVECTOR2 operand2) {
	return {
		operand1.x - operand2.x,
		operand1.y - operand2.y
	};
}

const auto viewmodel_sets = std::to_array<std::string>({
	"GoldenDV2",
	"PlasmaNinjato",
	"FuelingStationKey",
	"LighthouseKey",
	"VillageKey",
	"CraneKey",
	"FactoryGarageKey",
	"AA2",
	"AsVal",
	"Rags",
	"DV2",
	"Beans",
	"FlareGun",
	"M4",
	"M84",
	"RGO",
	"Lighter",
	"TFZ0",
	"MP443",
	"IZh81",
	"Makarov",
	"PPSH41",
	"SVD",
	"Mosin",
	"F1",
	"RGD5",
	"AI2",
	"Bandage",
	"AKMN",
	"TT33",
	"MaxEnergy",
	"AKM",
	"BloxyCola",
	"ResKola",
	"CatfrogSoda",
	"MP5SD",
	"AnarchyTomahawk"
});

void Celestial::Renderer::Update()
{
	/*Function to update everything outside of the render function*/
	/*Runs in sync with renderer*/

	POINT CursorPosition;
	GetCursorPos(&CursorPosition);
	ScreenToClient(FindWindow(NULL, L"ROBLOX"), &CursorPosition);

	this->ClientDimensions = Roblox.GetRobloxDimensions(Celestial::Globals::VisualEngine);
	this->Center = { ClientDimensions.x / 2, ClientDimensions.y / 2 };
	this->CursorPosition = CursorPosition;

	if (RenderSettings.Enabled)
	{
		this->Camera = Roblox.GetCameraPosition(Celestial::Globals::VisualEngine);
		this->LocalPlayer = Roblox.GetLocalPlayer(Roblox.GetPlayers(Celestial::Globals::DataModel));

		this->Players = []() {
			std::vector<std::uintptr_t> ret;

			if (Celestial::Globals::Game == "CBRO")
			{
				for (std::uintptr_t _ : Roblox.GetChildren(Globals::Workspace))
				{
					if (Roblox.FindFirstChild(_, "HumanoidRootPart"))
					{
						if (!Globals::Teamcheck)
							ret.push_back(_);

						if (Globals::Teamcheck)
						{
							std::uintptr_t Player = Roblox.FindFirstChild(Globals::Players, Roblox.GetName(_).c_str());
							if (!Player)
								continue;

							if (Roblox.GetTeam(Globals::LocalPlayer) != Roblox.GetTeam(Player))
								ret.push_back(_);
						}
					}
				}
			}

			if (Celestial::Globals::Game == "Recoil")
			{
				for (std::uintptr_t _ : Roblox.GetChildren(Globals::Workspace))
				{
					if (Roblox.FindFirstChild(_, "HumanoidRootPart"))
					{
						if (!Globals::Teamcheck)
							ret.push_back(_);

						if (Globals::Teamcheck)
						{
							std::uintptr_t Player = Roblox.FindFirstChild(Globals::Players, Roblox.GetName(_).c_str());
							if (!Player)
								continue;

							std::uintptr_t LocalTeam = Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::LocalPlayer, "GameStats"), "Team");

							if (!LocalTeam)
								continue;

							std::string LocalName = *reinterpret_cast<std::string*>(LocalTeam + 0x60);

							std::uintptr_t Team = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "GameStats"), "Team");

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
				if (Roblox.GetName(Roblox.GetTeam(Globals::LocalPlayer)) == "Ghosts")
				{
					for (std::uintptr_t InsideTeam : Roblox.GetChildren(Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::Workspace, "Players"), "Bright blue")))
						ret.push_back(InsideTeam);

					Globals::teamcolor = ImColor(41, 77, 91);
				}

				if (Roblox.GetName(Roblox.GetTeam(Globals::LocalPlayer)) == "Phantoms")
				{
					for (std::uintptr_t InsideTeam : Roblox.GetChildren(Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::Workspace, "Players"), "Bright orange")))
						ret.push_back(InsideTeam);

					Globals::teamcolor = ImColor(109, 82, 46);
				}
			}

			if (Celestial::Globals::Game == "Bad Business")
			{
				for (std::uintptr_t child : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "Characters")))
					if (!Roblox.IsPlayerOnTeam(child) && child != Roblox.IsLocalCharacter())
						ret.push_back(child);
			}

			if (Celestial::Globals::Game == "Rush Point")
			{
				for (std::uintptr_t Inside : Roblox.GetChildren(Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::Workspace, "MapFolder"), "Players")))
				{
					if (!Inside)
						continue;

					if (Roblox.GetName(Inside) == "Combat Turrent") continue;

					if (Inside == Globals::LocalPlayer) continue;

					if (!Globals::Teamcheck)
						ret.push_back(Inside);

					if (Globals::Teamcheck)
					{
						std::uintptr_t LocalTeam = Roblox.FindFirstChild(Globals::LocalPlayer, "PermanentTeam");

						if (!LocalTeam)
							continue;

						std::string LocalName = *reinterpret_cast<std::string*>(LocalTeam + 0x60);


						std::uintptr_t Team = Roblox.FindFirstChild(Inside, "PermanentTeam");

						if (!Team)
							continue;

						std::string OtherName = *reinterpret_cast<std::string*>(Team + 0x60);

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

			if (Celestial::Globals::Game == "Frontlines")
			{
				for (std::uintptr_t Inside : Roblox.GetChildren(Globals::Workspace))
				{
					if (Roblox.GetName(Inside) == "soldier_model")
					{
						if (Roblox.FindFirstChild(Inside, "friendly_marker") == 0)
						{
							ret.push_back(Inside);
						}
					}
				}
			}

			if (Celestial::Globals::Game == "Rift")
			{
				for (std::uintptr_t child : Roblox.GetChildren(Globals::Workspace))
				{
					if (Roblox.GetName(child).c_str() == "R15Rig")
					{
						ret.push_back(child);
					}
				}
			}

			if (Celestial::Globals::Game == "Criminality")
			{
				for (std::uintptr_t child : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "Characters")))
					ret.push_back(child);
			}

			if (Celestial::Globals::Game == "Apoc 2")
			{
				for (std::uintptr_t child : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "Characters")))
					ret.push_back(child);
			}

			if (Celestial::Globals::Game == "Deadline")
			{
				for (std::uintptr_t Child : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "characters")))
				{
					ret.push_back(Child);
				}
			}

			if (Celestial::Globals::Game == "th3ltgrounds")
			{
				for (std::uintptr_t Child : Roblox.GetChildren(Globals::Workspace))
				{
					if (Roblox.FindFirstChild(Child, "HumanoidRootPart"))
						ret.push_back(Child);
				}
			}

			if (Celestial::Globals::Game == "future")
			{
				for (std::uintptr_t Child : Roblox.GetChildren(Globals::Workspace))
				{
					if (Roblox.FindFirstChild(Child, "HumanoidRootPart"))
						ret.push_back(Child);
				}
			}

			if (Globals::Game == "Universal" || Globals::Game == "Riotfall" || Globals::Game == "ProjDelta")
			{
				ret = Roblox.GetChildren(Celestial::Globals::Players);
			}

			return ret;
		}();
	}
}

void DrawInfo(int X, int Y, int H, float Health, float MaxHealth, float Distance)
{
	const ImVec2 label_size = ImGui::CalcTextSize(std::to_string(Health).c_str(), NULL, true);

	auto HealthInfo = [Health, MaxHealth] {
		return std::to_string((int)Health) + " / " + std::to_string((int)MaxHealth);
	};

	auto DistanceInfo = [Distance] {
		return std::to_string((int)Distance) + "s";
	};

	if (Celestial::Globals::Game == "Universal" || Celestial::Globals::Game == "ProjDelta")
	{
		ImVec4 HealthColor;

		if (Health <= 100)
			HealthColor = ImColor(66, 245, 164);

		if (Health <= 90)
			HealthColor = ImColor(66, 245, 111);

		if (Health <= 70)
			HealthColor = ImColor(239, 245, 66);

		if (Health <= 60)
			HealthColor = ImColor(245, 215, 66);

		if (Health <= 50)
			HealthColor = ImColor(245, 176, 66);

		if (Health <= 40)
			HealthColor = ImColor(245, 150, 66);

		if (Health <= 30)
			HealthColor = ImColor(245, 123, 66);

		if (Health <= 20)
			HealthColor = ImColor(245, 102, 66);

		if (Health <= 10)
			HealthColor = ImColor(245, 66, 66);

		Engine->DrawString(HealthInfo().c_str(), X, Y + H + (H / 2.f) - 3, (ImColor)HealthColor, false, false);
	}
	else {
		Engine->DrawString("N/A", X, Y + H + (H / 2.f) - 3, ImColor(255, 255, 255), false, false);
	}

	Engine->DrawString(DistanceInfo().c_str(), X, Y + H + (H / 2.f) + 7, (ImColor)Celestial::Globals::DistanceColor, false, false);
}
// https://stackoverflow.com/questions/14029626/getting-a-specific-window-name-in-c
// Added a struct for easier return and passing of the PID
// One note. Fuck wstring with GetWindowTextW.
struct Window1
{
	DWORD PID;
	HWND Handle;
};

BOOL CALLBACK EnumWindowsProc1(HWND hwnd, LPARAM lParam)
{
	Window1& pid = *(Window1*)(lParam);

	DWORD WinId;
	GetWindowThreadProcessId(hwnd, &WinId);

	if (WinId == pid.PID)
		return FALSE;

	return TRUE;
}

// Find the main window of the program, returns 0 if not found
HWND FindMainWindow1()
{
	Window1 ToFind = { GetCurrentProcessId(), 0 };

	EnumWindows(EnumWindowsProc1, (LPARAM)&ToFind);

	return ToFind.Handle;
}

HWND main_window1 = 0;
HWND GetMainWindow1()
{
	// Set the proper scaling
	if (!main_window1)
		main_window1 = FindMainWindow1();

	return main_window1;
}

void Celestial::Renderer::Hook()
{
	this->Update();

	// Scale fixes
	float scale = ImGui_ImplWin32_GetDpiScaleForHwnd(GetMainWindow1());
	scale = scale ? scale : 1.0f;  // If somehow 0?

	if(Globals::FOV)
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(this->CursorPosition.x / scale, this->CursorPosition.y / scale), Celestial::Globals::FOVValue, (ImColor)Celestial::Globals::FOVColor, 0, 2);

	if (Celestial::Globals::Game == "Phantom Forces" && Celestial::Globals::ShowGunNames && Globals::RenderMaster)
	{
		for (std::uintptr_t GunDrop : Roblox.GetChildren(Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::Workspace, "Ignore"), "GunDrop")))
		{
			std::uintptr_t Node = Roblox.FindFirstChild(GunDrop, "UnderbarrelNode");

			if (!Node)
				continue;

			std::uintptr_t Value = Roblox.FindFirstChild(GunDrop, "Gun");

			if (!Value)
				continue;

			std::string GunName = "";
			
			if (Roblox.GetClassType(Value) == "StringValue")
			{
				GunName = *reinterpret_cast<std::string*>(Value + 0x60);
			}
			else GunName = "NULL";

			D3DXVECTOR2 NodePosition;
			bool IfNode = Roblox.WorldToScreen(Roblox.GetPartPosition(Node), NodePosition);

			if (IfNode)
			{
				Engine->DrawString(GunName.c_str(), NodePosition.x, NodePosition.y, (ImColor)Globals::GunColor, true, false);
			}
		}
	}

	if (Celestial::Globals::Game == "Bad Business" && Globals::ShowGrenades && Globals::RenderMaster)
	{
		for (std::uintptr_t Throwables : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "Throwables")))
		{

			if (Roblox.GetName(Throwables) == "Frag")
			{
				std::uintptr_t Body = Roblox.FindFirstChild(Throwables, "Body");

				if (!Body)
					continue;

				std::uintptr_t Primary = Roblox.FindFirstChild(Body, "BodyPrimary");

				if (!Primary)
					continue;

				D3DXVECTOR2 Position;
				bool IfPrimary = Roblox.WorldToScreen(Roblox.GetPartPosition(Primary), Position);

				if (IfPrimary)
				{
					Engine->DrawString("Grenade!!", Position.x, Position.y, (ImColor)Globals::GunColor, true, false);
				}
			}

			if (Roblox.GetName(Throwables) == "Molotov")
			{
				std::uintptr_t Body = Roblox.FindFirstChild(Throwables, "Body");

				if (!Body)
					continue;

				std::uintptr_t Primary = Roblox.FindFirstChild(Body, "BodyPrimary");

				if (!Primary)
					continue;

				D3DXVECTOR2 Position;
				bool IfPrimary = Roblox.WorldToScreen(Roblox.GetPartPosition(Primary), Position);

				if (IfPrimary)
				{
					Engine->DrawString("Molotov!!", Position.x, Position.y, (ImColor)Globals::GunColor, true, false);
				}
			}
		}
	}

	if (Celestial::Globals::Game == "Bad Business" && Globals::GrenadeWarning && Globals::RenderMaster)
	{
		for (std::uintptr_t Throwables : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "Throwables")))
		{

			if (Roblox.GetName(Throwables) == "Frag")
			{
				std::uintptr_t Body = Roblox.FindFirstChild(Throwables, "Body");

				if (!Body)
					continue;

				std::uintptr_t Primary = Roblox.FindFirstChild(Body, "BodyPrimary");

				if (!Primary)
					continue;

				float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(Primary)));
				if (DistanceBetween == 0) continue;

				if (DistanceBetween < 50)
					Engine->DrawString("Within Range Of Grenade!!", this->Center.x - ImGui::CalcTextSize("Within Range Of Grenade!!").x / 2 - 50, this->Center.y + - 120, ImColor(210, 115, 138, 95), false, true);
			}

			if (Roblox.GetName(Throwables) == "Molotov")
			{
				std::uintptr_t Body = Roblox.FindFirstChild(Throwables, "Body");

				if (!Body)
					continue;

				std::uintptr_t Primary = Roblox.FindFirstChild(Body, "BodyPrimary");

				if (!Primary)
					continue;

				float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(Primary)));
				if (DistanceBetween == 0) continue;

				if (DistanceBetween < 50)
					Engine->DrawString("Within Range Of Molotov!!", this->Center.x - ImGui::CalcTextSize("Within Range Of Molotov!!").x / 2 - 50, this->Center.y + -120, ImColor(210, 115, 138, 95), false, true);
			}
		}
	}

	if (Celestial::Globals::Game == "Apoc 2" && Globals::RenderMaster && Globals::ShowLocations)
	{
		for (std::uintptr_t PartLocation : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "Locations")))
		{
			if (!PartLocation)
				continue;

			D3DXVECTOR2 Position0;
			bool Location = Roblox.WorldToScreen(Roblox.GetPartPosition(PartLocation), Position0);

			if (Location)
			{
				const ImVec2 label_size = ImGui::CalcTextSize(Roblox.GetName(PartLocation).c_str(), NULL, true);
				
				float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(PartLocation)));
				if (DistanceBetween == 0) continue;

				if (DistanceBetween <= Globals::LocationDist)
				{
					ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Position0.x - label_size.x / 2, Position0.y + 9), 2, (ImColor)Globals::LocationColor);
					Engine->DrawString(Roblox.GetName(PartLocation).c_str(), Position0.x - label_size.x / 2, Position0.y + 9, (ImColor)Globals::LocationColor, Globals::ShowLocationsOutline, false);
					Engine->DrawString(std::to_string((int)DistanceBetween).c_str(), Position0.x - label_size.x / 2, Position0.y + 19, (ImColor)Globals::LocationColor, Globals::ShowLocationsOutline, false);
				}
			}
		}
	}

	if (Celestial::Globals::Game == "Apoc 2" && Globals::RenderMaster && Globals::ShowVehicles)
	{
		for (std::uintptr_t Vehicles : Roblox.GetChildren(Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::Workspace, "Vehicles"), "Spawned")))
		{
			if (!Vehicles)
				continue;

			D3DXVECTOR2 Position0;
			bool Location = Roblox.WorldToScreen(Roblox.GetPartPosition(Roblox.FindFirstChild(Vehicles, "Base")), Position0);

			if (Location)
			{
				const ImVec2 label_size = ImGui::CalcTextSize(Roblox.GetName(Vehicles).c_str(), NULL, true);

				float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(Roblox.FindFirstChild(Vehicles, "Base"))));
				if (DistanceBetween == 0) continue;

				if (DistanceBetween <= Globals::VehicleDist)
				{
					ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Position0.x - label_size.x / 2, Position0.y + 9), 2, (ImColor)Globals::VehiclesColor);
					Engine->DrawString(Roblox.GetName(Vehicles).c_str(), Position0.x - label_size.x / 2, Position0.y + 9, (ImColor)Globals::VehiclesColor, Globals::VehicleDistOutline, false);
					Engine->DrawString(std::to_string((int)DistanceBetween).c_str(), Position0.x - label_size.x / 2, Position0.y + 19, (ImColor)Globals::VehiclesColor, Globals::VehicleDistOutline, false);
				}
			}
		}
	}

	if (Celestial::Globals::Game == "Criminality" && Globals::RenderMaster && Globals::ShowDealer)
	{
		for (std::uintptr_t Dealer : Roblox.GetChildren(Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::Workspace, "Map"), "Shopz")))
		{
			if (!Dealer)
				continue;

			std::uintptr_t Part = Roblox.FindFirstChild(Dealer, "MainPart");

			if (!Part)
				continue;

			D3DXVECTOR2 Position0;
			bool PartExists = Roblox.WorldToScreen(Roblox.GetPartPosition(Part), Position0);

			if (PartExists)
			{
				const ImVec2 label_size = ImGui::CalcTextSize("Dealer", NULL, true);

				float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(Part)));
				if (DistanceBetween == 0) continue;

				if (DistanceBetween <= Globals::DealerDist)
				{
					ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Position0.x - label_size.x / 2, Position0.y + 9), 2, (ImColor)Globals::DealerColor);
					Engine->DrawString("Dealer", Position0.x - label_size.x / 2, Position0.y + 9, (ImColor)Globals::DealerColor, Globals::DealerDistOutline, false);
					Engine->DrawString(std::to_string((int)DistanceBetween).c_str(), Position0.x - label_size.x / 2, Position0.y + 19, (ImColor)Globals::DealerColor, Globals::DealerDistOutline, false);
				}
			}
		}
	}

	if (Celestial::Globals::Game == "Criminality" && Globals::RenderMaster && Globals::ShowATM)
	{
		for (std::uintptr_t ATMz : Roblox.GetChildren(Roblox.FindFirstChild(Roblox.FindFirstChild(Globals::Workspace, "Map"), "ATMz")))
		{
			if (!ATMz)
				continue;

			std::uintptr_t Part = Roblox.FindFirstChild(ATMz, "MainPart");

			if (!Part)
				continue;

			D3DXVECTOR2 Position0;
			bool PartExists = Roblox.WorldToScreen(Roblox.GetPartPosition(Part), Position0);

			if (PartExists)
			{
				const ImVec2 label_size = ImGui::CalcTextSize("ATM", NULL, true);

				float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(Part)));
				if (DistanceBetween == 0) continue;

				if (DistanceBetween <= Globals::DealerDist)
				{
					ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Position0.x - label_size.x / 2, Position0.y + 9), 2, (ImColor)Globals::ATMColor);
					Engine->DrawString("ATM", Position0.x - label_size.x / 2, Position0.y + 9, (ImColor)Globals::ATMColor, Globals::DealerDistOutline, false);
					Engine->DrawString(std::to_string((int)DistanceBetween).c_str(), Position0.x - label_size.x / 2, Position0.y + 19, (ImColor)Globals::ATMColor, Globals::DealerDistOutline, false);
				}
			}
		}
	}

	if (Celestial::Globals::Game == "ProjDelta" && Globals::RenderMaster && Globals::ShowContainers)
	{
		for (std::uintptr_t Containers : Roblox.GetChildren(Roblox.FindFirstChild(Globals::Workspace, "Containers")))
		{
			std::vector<std::uintptr_t> Inventory = Roblox.GetChildren(Roblox.FindFirstChild(Containers, "Inventory"));
			if (sizeof(Inventory) > 0)
			{
				std::uintptr_t Part = Roblox.FindFirstChild(Containers, "MeshPart");

				if (!Part)
					continue;

				D3DXVECTOR2 Position0;
				bool PartExists = Roblox.WorldToScreen(Roblox.GetPartPosition(Part), Position0);

				if (PartExists)
				{
					const ImVec2 label_size = ImGui::CalcTextSize(Roblox.GetName(Containers).c_str(), NULL, true);

					float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(Part)));
					if (DistanceBetween == 0) continue;

					if (DistanceBetween <= Globals::ContainerDist)
					{
						ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Position0.x - label_size.x / 2, Position0.y + 9), 2, ImColor(255, 255, 255));
						Engine->DrawString(Roblox.GetName(Containers).c_str(), Position0.x - label_size.x / 2, Position0.y + 9, ImColor(255, 255, 255), 0, false);
					}
				}
			}
		}
	}

	for (const auto Player : this->Players)
	{
		if(this->Players.size() == 0)
			continue;


		if (Player == Celestial::Globals::LocalPlayer) continue;

		if (Globals::Game == "Criminality" && (Roblox.GetName(Player) == Roblox.GetName(Globals::LocalPlayer))) continue;
		if (Globals::Game == "Universal" && Globals::Teamcheck && (Roblox.GetTeam(Player) == Roblox.GetTeam(Globals::LocalPlayer))) continue;
		if (Globals::Game == "Riotfall" && Globals::Teamcheck && (Roblox.GetTeam(Player) == Roblox.GetTeam(Globals::LocalPlayer))) continue;
		if (Globals::Game == "Apoc 2" && (Roblox.GetName(Roblox.GetPlayerFromCharacter(Player)).c_str() == Roblox.GetName(Globals::LocalPlayer))) continue;

		if (Globals::Game == "th3ltgrounds" && (Roblox.GetName(Roblox.GetPlayerFromCharacter(Player)).c_str() == Roblox.GetName(Globals::LocalPlayer))) continue;
		if (Globals::Game == "th3ltgrounds" && Globals::Teamcheck && (Roblox.GetTeam(Roblox.GetPlayerFromCharacter(Player)) == Roblox.GetTeam(Globals::LocalPlayer))) continue;

		if (Globals::Game == "future" && (Roblox.GetName(Roblox.GetPlayerFromCharacter(Player)).c_str() == Roblox.GetName(Globals::LocalPlayer))) continue;
		if (Globals::Game == "future" && Globals::Teamcheck && (Roblox.GetTeam(Roblox.GetPlayerFromCharacter(Player)) == Roblox.GetTeam(Globals::LocalPlayer))) continue;

		if (Globals::useteam)
		{
			if(Globals::Game == "Universal")
				Globals::teamcolor = BrickColor.GetColorFromID(Roblox.GetBrickColorID(Roblox.GetTeam(Player)));

			if (Globals::Game == "th3ltgrounds")
				Globals::teamcolor = BrickColor.GetColorFromID(Roblox.GetBrickColorID(Roblox.GetPlayerFromCharacter(Player)));

			if (Globals::Game == "future")
				Globals::teamcolor = BrickColor.GetColorFromID(Roblox.GetBrickColorID(Roblox.GetPlayerFromCharacter(Player)));
		}

		std::uintptr_t Character;

		if (Globals::Game == "Universal")
			Character = Roblox.GetCharacter(Player);

		if (Globals::Game == "Riotfall")
			Character = Roblox.GetCharacter(Player);

		if (Globals::Game == "Deadline")
			Character = Roblox.GetCharacter(Player);

		if (Globals::Game == "ProjDelta")
			Character = Roblox.GetCharacter(Player);

		if (Globals::Game == "Universal" && !Character) continue;
		if (Globals::Game == "Riotfall" && !Character) continue;
		if (Globals::Game == "ProjDelta" && !Character) continue;

		float Health = 0;
		float MaxHealth = 0;

		if (Globals::Game == "Universal" || Globals::Game == "ProjDelta")
		{
			std::uintptr_t Humanoid = Roblox.GetPlayerHumanoid(Player);
		
			if (!Humanoid)
				continue;

			Health = Roblox.GetPlayerHealth(Humanoid);
			MaxHealth = Roblox.GetMaxPlayerHealth(Humanoid);
			if (Globals::Healthcheck && Health == 0) continue;
		}


		std::uintptr_t HumanoidRootPart;

		if(Globals::Game == "Universal")
			HumanoidRootPart = Roblox.FindFirstChild(Character, "HumanoidRootPart");

		if (Globals::Game == "CBRO")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

		if (Globals::Game == "Recoil")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

		if (Globals::Game == "ProjDelta")
			HumanoidRootPart = Roblox.FindFirstChild(Character, "HumanoidRootPart");

		if (Globals::Game == "Criminality")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");
		
		if (Globals::Game == "Frontlines")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

		if (Globals::Game == "Apoc 2")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

		if (Globals::Game == "Riotfall")
			HumanoidRootPart = Roblox.FindFirstChild(Character, "HumanoidRootPart");

		if (Globals::Game == "Rush Point")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

		if (Globals::Game == "Phantom Forces")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "Torso");

		if (Globals::Game == "Bad Business")
			HumanoidRootPart = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "Chest");

		if (Globals::Game == "Rush Point")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");
		
		if(Globals::Game == "Deadline")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "humanoid_root_part");

		if (Globals::Game == "th3ltgrounds")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

		if (Globals::Game == "future")
			HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

		if (!HumanoidRootPart) continue;


		D3DXVECTOR3 HRPPosition = Roblox.GetPartPosition(HumanoidRootPart);

		D3DXVECTOR2 Position1;
		bool IfHead = Roblox.WorldToScreen(AddVec3(HRPPosition, { 0, 2, 0 }), Position1);

		D3DXVECTOR2 Position0;
		bool IfRoot = Roblox.WorldToScreen(HRPPosition, Position0);

		D3DXVECTOR2 Position3;
		bool IfLeg = Roblox.WorldToScreen(SubVec3(HRPPosition, { 0, 3, 0 }), Position3);

		float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, HRPPosition));
		if (DistanceBetween == 0) continue;
		
		if (DistanceBetween < 7 && Globals::Game == "Frontlines") continue;
		if (DistanceBetween < 7 && Globals::Game == "Rush Point") continue;
		if (DistanceBetween < 10 && Globals::Game == "Bad Business") continue;
		if (DistanceBetween < 10 && Globals::Game == "Recoil") continue;

		
		float PlayerHeight = Position1.y - Position0.y - 1.f;
		float PlayerWidth = PlayerHeight;

		if (Celestial::Globals::Game == "Apoc 2" && Globals::RenderMaster && Globals::ShowEquipped)
		{
			for (std::uintptr_t Equipped : Roblox.GetChildren(Roblox.FindFirstChild(Player, "Equipped")))
			{
				if (!Equipped)
					continue;

				std::string name = Roblox.GetName(Equipped);
				const ImVec2 label_size = ImGui::CalcTextSize(name.c_str(), NULL, true);

				Engine->DrawString(name.c_str(), Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) + 14, (ImColor)Celestial::Globals::NameColor, false, false);
			}
		}


		if (Celestial::Globals::Game == "ProjDelta" && Globals::RenderMaster && Globals::ShowEquippedDELTA)
		{
			std::uintptr_t Character = Roblox.GetCharacter(Player);

			if (!Character)
				continue;

			for (std::uintptr_t CharacterIns : Roblox.GetChildren(Character))
			{
				if (!CharacterIns)
					continue;

				if (std::find(viewmodel_sets.begin(), viewmodel_sets.end(), Roblox.GetName(CharacterIns)) != viewmodel_sets.end())
				{
					for (auto Weapon : Roblox.GetChildren(CharacterIns))
					{
						if (!Weapon)
							continue;

						if (Roblox.GetName(Weapon) == "ItemRoot")
						{
							D3DXVECTOR2 Position_0;
							bool Found = Roblox.WorldToScreen(Roblox.GetPartPosition(Weapon), Position_0);

							if (Found)
							{
								float DistanceBetween = Roblox.GetVector3Magnitude(SubVec3(this->Camera, Roblox.GetPartPosition(Weapon)));
								if (DistanceBetween == 0) continue;

								if (DistanceBetween <= Globals::ItemDist)
								{
									std::string name = Roblox.GetName(Roblox.GetInstanceParent(Weapon));
									const ImVec2 label_size = ImGui::CalcTextSize(name.c_str(), NULL, true);
									Engine->DrawString(name.c_str(), Position_0.x, Position_0.y, (ImColor)Celestial::Globals::NameColor, false, false);
								}
							}
						}
					}
				}
			}
		}

		if (IfHead && IfLeg && Celestial::Globals::RenderMaster && DistanceBetween <= Globals::RenderDist)
		{
			if (Celestial::Globals::visualize_prediction)
			{
				D3DXVECTOR3 Velocity = Roblox.GetPrimitiveVelocity(HumanoidRootPart) / Globals::p_strength;
				D3DXVECTOR3 Position = AddVec3(Roblox.GetPartPosition(HumanoidRootPart), Velocity);

				D3DXVECTOR2 Position2;
				bool IfVelocity = Roblox.WorldToScreen(Position, Position2);

				if(IfVelocity)
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position0.x, Position0.y), ImVec2(Position2.x, Position2.y), ImColor(255, 255, 255));
			}

			if (Celestial::Globals::PlayerESP)
			{
				if(Globals::useteam)
					Engine->DrawRect(Position0.x, Position0.y, PlayerWidth, PlayerHeight, (ImColor)Globals::teamcolor, 2, false);
				else
					Engine->DrawRect(Position0.x, Position0.y, PlayerWidth, PlayerHeight, (ImColor)Globals::BoxColor, 2, false);
			}

			if (Celestial::Globals::NameESP)
			{
				if (Globals::Game == "Deadline")
				{
					Engine->DrawString("Enemy", Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::NameColor, false, false);
				}

				if (Globals::Game == "Apoc 2")
				{
					std::uintptr_t CustomPlayer = Roblox.GetPlayerFromCharacter(Player);
					
					if (!CustomPlayer)
						continue;

					Engine->DrawString(Roblox.GetName(CustomPlayer).c_str(), Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::NameColor, false, false);
				}

				if (Globals::Game == "Universal" || Globals::Game == "th3ltgrounds" || Globals::Game == "future" || Globals::Game == "ProjDelta" || Globals::Game == "Recoil")
				{
					if(Globals::useteam)
						Engine->DrawString(Roblox.GetName(Player).c_str(), Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::teamcolor, false, false);
					else
						Engine->DrawString(Roblox.GetName(Player).c_str(), Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::NameColor, false, false);
				}

				if (Globals::Game == "Phantom Forces")
				{
					Engine->DrawString("Enemy", Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::NameColor, false, false);
				}

				if (Globals::Game == "Bad Business")
				{
					Engine->DrawString("Enemy", Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::NameColor, false, false);
				}

				if (Globals::Game == "Rush Point")
				{
					Engine->DrawString(Roblox.GetName(Player).c_str(), Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::NameColor, false, false);
				}

				if (Globals::Game == "Riotfall")
				{
					Engine->DrawString(Roblox.GetName(Player).c_str(), Position0.x, Position0.y + PlayerHeight + (PlayerHeight / 2.f) - 14, (ImColor)Celestial::Globals::NameColor, false, false);
				}
			}

			if (Celestial::Globals::ShowDistance)
			{
				float HealthHeight = Position1.y - Position0.y - 1.f;
				float X = (Position1.x - (HealthHeight - 20));

				if (Globals::Game == "Bad Business")
				{
					DrawInfo(X, Position0.y, PlayerHeight, Health, MaxHealth, DistanceBetween);
				}
				else
				{
					DrawInfo(X, Position0.y, PlayerHeight, Health, MaxHealth, DistanceBetween);
				}
			}

			if (Celestial::Globals::Tracers)
			{
				if (Globals::Game == "Universal" || Globals::Game == "th3ltgrounds" || Globals::Game == "future")
				{
					if (Globals::useteam)
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(this->Center.x, this->ClientDimensions.y), ImVec2(Position0.x, Position0.y - PlayerHeight - (PlayerHeight)), (ImColor)Celestial::Globals::teamcolor, 2);
					else
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(this->Center.x, this->ClientDimensions.y), ImVec2(Position0.x, Position0.y - PlayerHeight - (PlayerHeight)), (ImColor)Celestial::Globals::TracersColor, 2);
				}
				else
				{
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(this->Center.x, this->ClientDimensions.y), ImVec2(Position0.x, Position0.y - PlayerHeight - (PlayerHeight)), (ImColor)Celestial::Globals::TracersColor, 2);
				}
			}

			if (Celestial::Globals::SkeletonESP)
			{
				if (Celestial::Globals::Game == "Universal" || Celestial::Globals::Game == "ProjDelta")
				{
					std::uintptr_t Humanoid = Roblox.GetPlayerHumanoid(Player);

					if (!Humanoid)
						continue;

					std::int32_t HumanoidRigType = Roblox.GetRigType(Humanoid);

					switch (HumanoidRigType)
					{
						case 0: /* R6 */
						{

							std::uintptr_t Head = Roblox.FindFirstChild(Character, "Head");

							if (!Head)
								continue;

							std::uintptr_t Torso = Roblox.FindFirstChild(Character, "Torso");

							if (!Torso)
								continue;

							std::uintptr_t LeftArm = Roblox.FindFirstChild(Character, "Left Arm");

							if (!LeftArm)
								continue;

							std::uintptr_t RightArm = Roblox.FindFirstChild(Character, "Right Arm");

							if (!RightArm)
								continue;

							std::uintptr_t HumanoidRootPart = Roblox.FindFirstChild(Character, "HumanoidRootPart");

							if (!HumanoidRootPart)
								continue;

							std::uintptr_t LeftLeg = Roblox.FindFirstChild(Character, "Left Leg");

							if (!LeftLeg)
								continue;

							std::uintptr_t RightLeg = Roblox.FindFirstChild(Character, "Right Leg");

							if (!RightLeg)
								continue;

							D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);
							D3DXVECTOR3 TorsoPosition = Roblox.GetPartPosition(Torso);
							D3DXVECTOR3 LeftArmPosition = Roblox.GetPartPosition(LeftArm);
							D3DXVECTOR3 RightArmPosition = Roblox.GetPartPosition(RightArm);
							D3DXVECTOR3 RootPartPosition = Roblox.GetPartPosition(HumanoidRootPart);
							D3DXVECTOR3 LeftLegPosition = Roblox.GetPartPosition(LeftLeg);
							D3DXVECTOR3 RightLegPosition = Roblox.GetPartPosition(RightLeg);

							D3DXVECTOR2 Position1;
							bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

							D3DXVECTOR2 Position2;
							bool IfTorso = Roblox.WorldToScreen(TorsoPosition, Position2);

							D3DXVECTOR2 Position3;
							bool IfLeftArm = Roblox.WorldToScreen(LeftArmPosition, Position3);

							D3DXVECTOR2 Position4;
							bool IfRightArm = Roblox.WorldToScreen(RightArmPosition, Position4);

							D3DXVECTOR2 Position5;
							bool IfRootPart = Roblox.WorldToScreen(RootPartPosition, Position5);

							D3DXVECTOR2 Position6;
							bool IfLeftLeg = Roblox.WorldToScreen(LeftLegPosition, Position6);

							D3DXVECTOR2 Position7;
							bool IfRightLeg = Roblox.WorldToScreen(RightLegPosition, Position7);

							if (IfHead && IfTorso && IfLeftArm && IfRightArm && IfRootPart && IfLeftLeg && IfRightLeg)
							{
								{
									if (!Globals::useteam)
									{
										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
									}
								}

								{
									if (Globals::useteam)
									{
										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::teamcolor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::teamcolor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::teamcolor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::teamcolor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

										ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::teamcolor, 2);
										//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
									}
								}
							}

						}

						case 1: /* R15 */
						{
							std::uintptr_t Head = Roblox.FindFirstChild(Character, "Head");

							if (!Head)
								continue;

							std::uintptr_t UpperTorso = Roblox.FindFirstChild(Character, "UpperTorso");

							if (!UpperTorso)
								continue;

							std::uintptr_t LowerTorso = Roblox.FindFirstChild(Character, "LowerTorso");

							if (!LowerTorso)
								continue;

							/*Left Arm*/

							std::uintptr_t LeftUpper = Roblox.FindFirstChild(Character, "LeftUpperArm");

							if (!LeftUpper)
								continue;

							std::uintptr_t LeftLower = Roblox.FindFirstChild(Character, "LeftLowerArm");

							if (!LeftLower)
								continue;

							std::uintptr_t LeftHand = Roblox.FindFirstChild(Character, "LeftHand");

							if (!LeftHand)
								continue;

							/*Right Arm*/

							std::uintptr_t RightUpper = Roblox.FindFirstChild(Character, "RightUpperArm");

							if (!RightUpper)
								continue;

							std::uintptr_t RightLower = Roblox.FindFirstChild(Character, "RightLowerArm");

							if (!RightLower)
								continue;

							std::uintptr_t RightHand = Roblox.FindFirstChild(Character, "RightHand");

							if (!RightHand)
								continue;

							/*Left Leg*/

							std::uintptr_t LeftUpperLeg = Roblox.FindFirstChild(Character, "LeftUpperLeg");

							if (!LeftUpperLeg)
								continue;

							std::uintptr_t LeftLowerLeg = Roblox.FindFirstChild(Character, "LeftLowerLeg");

							if (!LeftLowerLeg)
								continue;

							std::uintptr_t LeftFoot = Roblox.FindFirstChild(Character, "LeftFoot");

							if (!LeftFoot)
								continue;

							/*Right Leg*/

							std::uintptr_t RightUpperLeg = Roblox.FindFirstChild(Character, "RightUpperLeg");

							if (!RightUpperLeg)
								continue;

							std::uintptr_t RightLowerLeg = Roblox.FindFirstChild(Character, "RightLowerLeg");

							if (!RightLowerLeg)
								continue;

							std::uintptr_t RightFoot = Roblox.FindFirstChild(Character, "RightFoot");

							if (!RightFoot)
								continue;


							/*Get Position*/

							D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);

							D3DXVECTOR3 UpperPosition = Roblox.GetPartPosition(UpperTorso);
							D3DXVECTOR3 LowerPosition = Roblox.GetPartPosition(LowerTorso);

							/*Left Arm*/
							D3DXVECTOR3 LeftUpperPosition = Roblox.GetPartPosition(LeftUpper);
							D3DXVECTOR3 LeftLowerPosition = Roblox.GetPartPosition(LeftLower);
							D3DXVECTOR3 LeftHandPosition = Roblox.GetPartPosition(LeftHand);

							/*Right Arm*/
							D3DXVECTOR3 RightUpperPosition = Roblox.GetPartPosition(RightUpper);
							D3DXVECTOR3 RightLowerPosition = Roblox.GetPartPosition(RightLower);
							D3DXVECTOR3 RightHandPosition = Roblox.GetPartPosition(RightHand);

							/*Left Leg*/
							D3DXVECTOR3 LeftUpperLegPosition = Roblox.GetPartPosition(LeftUpperLeg);
							D3DXVECTOR3 LeftLowerLegPosition = Roblox.GetPartPosition(LeftLowerLeg);
							D3DXVECTOR3 LeftFootPosition = Roblox.GetPartPosition(LeftFoot);

							/*Right Leg*/
							D3DXVECTOR3 RightUpperLegPosition = Roblox.GetPartPosition(RightUpperLeg);
							D3DXVECTOR3 RightLowerLegPosition = Roblox.GetPartPosition(RightLowerLeg);
							D3DXVECTOR3 RightFootPosition = Roblox.GetPartPosition(RightFoot);

							D3DXVECTOR2 Position1;
							bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

							D3DXVECTOR2 Position2;
							bool IfUpperTorso = Roblox.WorldToScreen(UpperPosition, Position2);

							D3DXVECTOR2 Position3;
							bool IfLowerTorso = Roblox.WorldToScreen(LowerPosition, Position3);

							D3DXVECTOR2 Position4;
							bool IfLeftUpper = Roblox.WorldToScreen(LeftUpperPosition, Position4);

							D3DXVECTOR2 Position5;
							bool IfLeftLower = Roblox.WorldToScreen(LeftLowerPosition, Position5);

							D3DXVECTOR2 Position6;
							bool IfLeftHand = Roblox.WorldToScreen(LeftHandPosition, Position6);

							D3DXVECTOR2 Position7;
							bool IfRightUpper = Roblox.WorldToScreen(RightUpperPosition, Position7);

							D3DXVECTOR2 Position8;
							bool IfRightLower = Roblox.WorldToScreen(RightLowerPosition, Position8);

							D3DXVECTOR2 Position9;
							bool IfRightHand = Roblox.WorldToScreen(RightHandPosition, Position9);

							D3DXVECTOR2 Position10;
							bool IfLeftUpperLeg = Roblox.WorldToScreen(LeftUpperLegPosition, Position10);

							D3DXVECTOR2 Position11;
							bool IfLeftLowerLeg = Roblox.WorldToScreen(LeftLowerLegPosition, Position11);

							D3DXVECTOR2 Position12;
							bool IfLeftFoot = Roblox.WorldToScreen(LeftFootPosition, Position12);

							D3DXVECTOR2 Position13;
							bool IfRightUpperLeg = Roblox.WorldToScreen(RightUpperLegPosition, Position13);

							D3DXVECTOR2 Position14;
							bool IfRightLowerLeg = Roblox.WorldToScreen(RightLowerLegPosition, Position14);

							D3DXVECTOR2 Position15;
							bool IfRightFoot = Roblox.WorldToScreen(RightFootPosition, Position15);

							if (IfHead && IfUpperTorso && IfLowerTorso && IfLeftUpper && IfLeftLower && IfLeftHand && IfRightUpper && IfRightLower && IfRightHand && IfLeftUpperLeg && IfLeftLowerLeg && IfLeftFoot && IfRightUpperLeg && IfRightLowerLeg && IfRightFoot)
							{
								if (!Globals::useteam)
								{
									/*Head -> Upper Torso*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);

									/*Upper Torso -> Lower Torso*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);

									/*Upper Torso -> Left Upper Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);

									/*Left Upper Arm -> Left Lower Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::SkeletonColor, 2);

									/*Left Lower Arm -> Left Hand*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);

									/*<><><><><><>*/

									/*Upper Torso -> Right Upper Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);

									/*Right Upper Arm -> Right Lower Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position7.x, Position7.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::SkeletonColor, 2);

									/*Right Lower Arm -> Right Hand*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::SkeletonColor, 2);

									/*Lower half of body*/

									/*Lower Torso -> Left Upper Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::SkeletonColor, 2);

									/*Left Upper Leg -> Lower Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position10.x, Position10.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::SkeletonColor, 2);

									/*Lower Leg -> Left Foot*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::SkeletonColor, 2);

									/*Lower Torso -> Right Upper Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::SkeletonColor, 2);

									/*Right Upper Leg -> Lower Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position13.x, Position13.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::SkeletonColor, 2);

									/*Lower Leg -> Right Foot*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::SkeletonColor, 2);
								}

								if (Globals::useteam)
								{
									/*Head -> Upper Torso*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::teamcolor, 2);

									/*Upper Torso -> Lower Torso*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::teamcolor, 2);

									/*Upper Torso -> Left Upper Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::teamcolor, 2);

									/*Left Upper Arm -> Left Lower Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::teamcolor, 2);

									/*Left Lower Arm -> Left Hand*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::teamcolor, 2);

									/*<><><><><><>*/

									/*Upper Torso -> Right Upper Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::teamcolor, 2);

									/*Right Upper Arm -> Right Lower Arm*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position7.x, Position7.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::teamcolor, 2);

									/*Right Lower Arm -> Right Hand*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::teamcolor, 2);

									/*Lower half of body*/

									/*Lower Torso -> Left Upper Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::teamcolor, 2);

									/*Left Upper Leg -> Lower Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position10.x, Position10.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::teamcolor, 2);

									/*Lower Leg -> Left Foot*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::teamcolor, 2);

									/*Lower Torso -> Right Upper Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::teamcolor, 2);

									/*Right Upper Leg -> Lower Leg*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position13.x, Position13.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::teamcolor, 2);

									/*Lower Leg -> Right Foot*/
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::teamcolor, 2);
								}
							}
						}
					}
				}

				if (Celestial::Globals::Game == "th3ltgrounds")
				{
					std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

					if (!Head)
						continue;

					std::uintptr_t Torso = Roblox.FindFirstChild(Player, "Torso");

					if (!Torso)
						continue;

					std::uintptr_t LeftArm = Roblox.FindFirstChild(Player, "Left Arm");

					if (!LeftArm)
						continue;

					std::uintptr_t RightArm = Roblox.FindFirstChild(Player, "Right Arm");

					if (!RightArm)
						continue;

					std::uintptr_t HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

					if (!HumanoidRootPart)
						continue;

					std::uintptr_t LeftLeg = Roblox.FindFirstChild(Player, "Left Leg");

					if (!LeftLeg)
						continue;

					std::uintptr_t RightLeg = Roblox.FindFirstChild(Player, "Right Leg");

					if (!RightLeg)
						continue;

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);
					D3DXVECTOR3 TorsoPosition = Roblox.GetPartPosition(Torso);
					D3DXVECTOR3 LeftArmPosition = Roblox.GetPartPosition(LeftArm);
					D3DXVECTOR3 RightArmPosition = Roblox.GetPartPosition(RightArm);
					D3DXVECTOR3 RootPartPosition = Roblox.GetPartPosition(HumanoidRootPart);
					D3DXVECTOR3 LeftLegPosition = Roblox.GetPartPosition(LeftLeg);
					D3DXVECTOR3 RightLegPosition = Roblox.GetPartPosition(RightLeg);

					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

					D3DXVECTOR2 Position2;
					bool IfTorso = Roblox.WorldToScreen(TorsoPosition, Position2);

					D3DXVECTOR2 Position3;
					bool IfLeftArm = Roblox.WorldToScreen(LeftArmPosition, Position3);

					D3DXVECTOR2 Position4;
					bool IfRightArm = Roblox.WorldToScreen(RightArmPosition, Position4);

					D3DXVECTOR2 Position5;
					bool IfRootPart = Roblox.WorldToScreen(RootPartPosition, Position5);

					D3DXVECTOR2 Position6;
					bool IfLeftLeg = Roblox.WorldToScreen(LeftLegPosition, Position6);

					D3DXVECTOR2 Position7;
					bool IfRightLeg = Roblox.WorldToScreen(RightLegPosition, Position7);

					if (IfHead && IfTorso && IfLeftArm && IfRightArm && IfRootPart && IfLeftLeg && IfRightLeg)
					{
						{
							if (!Globals::useteam)
							{
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
							}
						}

						{
							if (Globals::useteam)
							{
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
							}
						}
					}
				}

				if (Celestial::Globals::Game == "future")
				{
					std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

					if (!Head)
						continue;

					std::uintptr_t Torso = Roblox.FindFirstChild(Player, "Torso");

					if (!Torso)
						continue;

					std::uintptr_t LeftArm = Roblox.FindFirstChild(Player, "Left Arm");

					if (!LeftArm)
						continue;

					std::uintptr_t RightArm = Roblox.FindFirstChild(Player, "Right Arm");

					if (!RightArm)
						continue;

					std::uintptr_t HumanoidRootPart = Roblox.FindFirstChild(Player, "HumanoidRootPart");

					if (!HumanoidRootPart)
						continue;

					std::uintptr_t LeftLeg = Roblox.FindFirstChild(Player, "Left Leg");

					if (!LeftLeg)
						continue;

					std::uintptr_t RightLeg = Roblox.FindFirstChild(Player, "Right Leg");

					if (!RightLeg)
						continue;

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);
					D3DXVECTOR3 TorsoPosition = Roblox.GetPartPosition(Torso);
					D3DXVECTOR3 LeftArmPosition = Roblox.GetPartPosition(LeftArm);
					D3DXVECTOR3 RightArmPosition = Roblox.GetPartPosition(RightArm);
					D3DXVECTOR3 RootPartPosition = Roblox.GetPartPosition(HumanoidRootPart);
					D3DXVECTOR3 LeftLegPosition = Roblox.GetPartPosition(LeftLeg);
					D3DXVECTOR3 RightLegPosition = Roblox.GetPartPosition(RightLeg);

					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

					D3DXVECTOR2 Position2;
					bool IfTorso = Roblox.WorldToScreen(TorsoPosition, Position2);

					D3DXVECTOR2 Position3;
					bool IfLeftArm = Roblox.WorldToScreen(LeftArmPosition, Position3);

					D3DXVECTOR2 Position4;
					bool IfRightArm = Roblox.WorldToScreen(RightArmPosition, Position4);

					D3DXVECTOR2 Position5;
					bool IfRootPart = Roblox.WorldToScreen(RootPartPosition, Position5);

					D3DXVECTOR2 Position6;
					bool IfLeftLeg = Roblox.WorldToScreen(LeftLegPosition, Position6);

					D3DXVECTOR2 Position7;
					bool IfRightLeg = Roblox.WorldToScreen(RightLegPosition, Position7);

					if (IfHead && IfTorso && IfLeftArm && IfRightArm && IfRootPart && IfLeftLeg && IfRightLeg)
					{
						{
							if (!Globals::useteam)
							{
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
							}
						}

						{
							if (Globals::useteam)
							{
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::teamcolor, 2);
								//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
							}
						}
					}
				}

				if (Celestial::Globals::Game == "Phantom Forces")
				{
					std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

					if (!Head)
						continue;

					std::uintptr_t Torso = Roblox.FindFirstChild(Player, "Torso");

					if (!Torso)
						continue;

					std::uintptr_t LeftArm = Roblox.FindFirstChild(Player, "Left Arm");

					if (!LeftArm)
						continue;

					std::uintptr_t RightArm = Roblox.FindFirstChild(Player, "Right Arm");

					if (!RightArm)
						continue;


					std::uintptr_t LeftLeg = Roblox.FindFirstChild(Player, "Left Leg");

					if (!LeftLeg)
						continue;

					std::uintptr_t RightLeg = Roblox.FindFirstChild(Player, "Right Leg");

					if (!RightLeg)
						continue;

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);
					D3DXVECTOR3 TorsoPosition = Roblox.GetPartPosition(Torso);
					D3DXVECTOR3 LeftArmPosition = Roblox.GetPartPosition(LeftArm);
					D3DXVECTOR3 RightArmPosition = Roblox.GetPartPosition(RightArm);
					D3DXVECTOR3 LeftLegPosition = Roblox.GetPartPosition(LeftLeg);
					D3DXVECTOR3 RightLegPosition = Roblox.GetPartPosition(RightLeg);

					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);
					if (Position1.x == -1) continue;

					D3DXVECTOR2 Position2;
					bool IfTorso = Roblox.WorldToScreen(TorsoPosition, Position2);
					if (Position2.x == -1) continue;

					D3DXVECTOR2 Position3;
					bool IfLeftArm = Roblox.WorldToScreen(LeftArmPosition, Position3);
					if (Position3.x == -1) continue;

					D3DXVECTOR2 Position4;
					bool IfRightArm = Roblox.WorldToScreen(RightArmPosition, Position4);
					if (Position4.x == -1) continue;

					D3DXVECTOR2 Position6;
					bool IfLeftLeg = Roblox.WorldToScreen(LeftLegPosition, Position6);
					if (Position6.x == -1) continue;

					D3DXVECTOR2 Position7;
					bool IfRightLeg = Roblox.WorldToScreen(RightLegPosition, Position7);
					if (Position7.x == -1) continue;

					if (IfHead && IfTorso && IfLeftArm && IfRightArm && IfLeftLeg && IfRightLeg)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
					}
				}

				if (Celestial::Globals::Game == "Bad Business")
				{
					if (Player == Roblox.IsLocalCharacter())
						continue;

					std::uintptr_t Head = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "Head");

					if (!Head)
						continue;

					std::uintptr_t Neck = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "Neck");

					if (!Neck)
						continue;

					std::uintptr_t Abodomen = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "Abdomen");

					if (!Abodomen)
						continue;

					std::uintptr_t Chest = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "Chest");

					if (!Chest)
						continue;

					/*LEFT ARM*/

					std::uintptr_t LeftArm = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "LeftArm");

					if (!LeftArm)
						continue;

					std::uintptr_t LeftForearm = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "LeftForearm");

					if (!LeftForearm)
						continue;

					std::uintptr_t LeftHand = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "LeftHand");

					if (!LeftHand)
						continue;

					/*RIGHT ARM*/

					std::uintptr_t RightArm = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "RightArm");

					if (!RightArm)
						continue;

					std::uintptr_t RightForearm = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "RightForearm");

					if (!RightForearm)
						continue;

					std::uintptr_t RightHand = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "RightHand");

					if (!RightHand)
						continue;

					/*LEFT LEG*/

					std::uintptr_t LeftLeg = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "LeftLeg");

					if (!LeftLeg)
						continue;

					std::uintptr_t LeftForeleg = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "LeftForeleg");

					if (!LeftForeleg)
						continue;

					std::uintptr_t LeftFoot = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "LeftFoot");

					if (!LeftFoot)
						continue;

					/*RIGHT LEG*/

					std::uintptr_t RightLeg = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "RightLeg");

					if (!RightLeg)
						continue;

					std::uintptr_t RightForeleg = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "RightForeleg");

					if (!RightForeleg)
						continue;

					std::uintptr_t RightFoot = Roblox.FindFirstChild(Roblox.FindFirstChild(Player, "Body"), "RightFoot");

					if (!RightFoot)
						continue;

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);
					D3DXVECTOR3 NeckPosition = Roblox.GetPartPosition(Neck);
					D3DXVECTOR3 AbdomenPosition = Roblox.GetPartPosition(Abodomen);
					D3DXVECTOR3 ChestPosition = Roblox.GetPartPosition(Chest);

					D3DXVECTOR3 LeftArmPosition = Roblox.GetPartPosition(LeftArm);
					D3DXVECTOR3 LeftForearmPosition = Roblox.GetPartPosition(LeftForearm);
					D3DXVECTOR3 LeftHandPosition = Roblox.GetPartPosition(LeftHand);

					D3DXVECTOR3 RightArmPosition = Roblox.GetPartPosition(RightArm);
					D3DXVECTOR3 RightForearmPosition = Roblox.GetPartPosition(RightForearm);
					D3DXVECTOR3 RightHandPosition = Roblox.GetPartPosition(RightHand);

					D3DXVECTOR3 LeftLegPosition = Roblox.GetPartPosition(LeftLeg);
					D3DXVECTOR3 LeftForelegPosition = Roblox.GetPartPosition(LeftForeleg);
					D3DXVECTOR3 LeftFootPosition = Roblox.GetPartPosition(LeftFoot);

					D3DXVECTOR3 RightLegPosition = Roblox.GetPartPosition(RightLeg);
					D3DXVECTOR3 RightForelegPosition = Roblox.GetPartPosition(RightForeleg);
					D3DXVECTOR3 RightFootPosition = Roblox.GetPartPosition(RightFoot);


					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);
					if (Position1.x == -1) continue;

					D3DXVECTOR2 Position2;
					bool IfNeck = Roblox.WorldToScreen(NeckPosition, Position2);
					if (Position2.x == -1) continue;

					D3DXVECTOR2 Position3;
					bool IfAbdomen = Roblox.WorldToScreen(AbdomenPosition, Position3);
					if (Position3.x == -1) continue;

					D3DXVECTOR2 Position4;
					bool IfChest = Roblox.WorldToScreen(ChestPosition, Position4);
					if (Position4.x == -1) continue;

					D3DXVECTOR2 Position5;
					bool IfLeftArm = Roblox.WorldToScreen(LeftArmPosition, Position5);
					if (Position5.x == -1) continue;

					D3DXVECTOR2 Position6;
					bool IfLeftForearm = Roblox.WorldToScreen(LeftForearmPosition, Position6);
					if (Position6.x == -1) continue;

					D3DXVECTOR2 Position7;
					bool IfLeftHand = Roblox.WorldToScreen(LeftHandPosition, Position7);
					if (Position7.x == -1) continue;

					D3DXVECTOR2 Position8;
					bool IfRightArm = Roblox.WorldToScreen(RightArmPosition, Position8);
					if (Position8.x == -1) continue;

					D3DXVECTOR2 Position9;
					bool IfRightForearm = Roblox.WorldToScreen(RightForearmPosition, Position9);
					if (Position9.x == -1) continue;

					D3DXVECTOR2 Position10;
					bool IfRightHand = Roblox.WorldToScreen(RightHandPosition, Position10);
					if (Position10.x == -1) continue;

					D3DXVECTOR2 Position11;
					bool IfLeftLeg = Roblox.WorldToScreen(LeftLegPosition, Position11);
					if (Position11.x == -1) continue;

					D3DXVECTOR2 Position12;
					bool IfLeftForeleg = Roblox.WorldToScreen(LeftForelegPosition, Position12);
					if (Position12.x == -1) continue;

					D3DXVECTOR2 Position13;
					bool IfLeftFoot = Roblox.WorldToScreen(LeftFootPosition, Position13);
					if (Position13.x == -1) continue;

					D3DXVECTOR2 Position14;
					bool IfRightLeg = Roblox.WorldToScreen(RightLegPosition, Position14);
					if (Position14.x == -1) continue;

					D3DXVECTOR2 Position15;
					bool IfRightForeleg = Roblox.WorldToScreen(RightForelegPosition, Position15);
					if (Position15.x == -1) continue;

					D3DXVECTOR2 Position16;
					bool IfRightFoot = Roblox.WorldToScreen(RightFootPosition, Position16);
					if (Position16.x == -1) continue;

					if (IfHead && IfNeck && IfAbdomen && IfChest && IfLeftArm && IfLeftForearm && IfLeftHand && IfRightArm && IfRightForearm && IfRightHand && IfLeftLeg && IfLeftForeleg && IfLeftFoot && IfRightLeg && IfRightForeleg && IfRightFoot)
					{
						/*Head -> Neck*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);

						/*Neck -> Left Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Arm -> Left Forearm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Forearm -> Left Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position6.x, Position6.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);

						/*Neck -> Right Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Arm -> Right Forearm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Forearm -> Right Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position9.x, Position9.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::SkeletonColor, 2);

						/*Neck -> Chest*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);

						/*Chest -> Abdomen*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);

						/*Abdomen -> Left Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Leg -> Left Foreleg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Foreleg -> Left Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position12.x, Position12.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::SkeletonColor, 2);

						/*Abdomen -> Right Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Leg -> Right Foreleg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Foreleg -> Right Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position15.x, Position15.y), ImVec2(Position16.x, Position16.y), (ImColor)Globals::SkeletonColor, 2);
					}
				}


				if (Celestial::Globals::Game == "Rush Point")
				{
					std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

					if (!Head)
						continue;

					std::uintptr_t UpperTorso = Roblox.FindFirstChild(Player, "UpperTorso");

					if (!UpperTorso)
						continue;

					std::uintptr_t LowerTorso = Roblox.FindFirstChild(Player, "LowerTorso");

					if (!LowerTorso)
						continue;

					/*Left Arm*/

					std::uintptr_t LeftUpper = Roblox.FindFirstChild(Player, "LeftUpperArm");

					if (!LeftUpper)
						continue;

					std::uintptr_t LeftLower = Roblox.FindFirstChild(Player, "LeftLowerArm");

					if (!LeftLower)
						continue;

					std::uintptr_t LeftHand = Roblox.FindFirstChild(Player, "LeftHand");

					if (!LeftHand)
						continue;

					/*Right Arm*/

					std::uintptr_t RightUpper = Roblox.FindFirstChild(Player, "RightUpperArm");

					if (!RightUpper)
						continue;

					std::uintptr_t RightLower = Roblox.FindFirstChild(Player, "RightLowerArm");

					if (!RightLower)
						continue;

					std::uintptr_t RightHand = Roblox.FindFirstChild(Player, "RightHand");

					if (!RightHand)
						continue;

					/*Left Leg*/

					std::uintptr_t LeftUpperLeg = Roblox.FindFirstChild(Player, "LeftUpperLeg");

					if (!LeftUpperLeg)
						continue;

					std::uintptr_t LeftLowerLeg = Roblox.FindFirstChild(Player, "LeftLowerLeg");

					if (!LeftLowerLeg)
						continue;

					std::uintptr_t LeftFoot = Roblox.FindFirstChild(Player, "LeftFoot");

					if (!LeftFoot)
						continue;

					/*Right Leg*/

					std::uintptr_t RightUpperLeg = Roblox.FindFirstChild(Player, "RightUpperLeg");

					if (!RightUpperLeg)
						continue;

					std::uintptr_t RightLowerLeg = Roblox.FindFirstChild(Player, "RightLowerLeg");

					if (!RightLowerLeg)
						continue;

					std::uintptr_t RightFoot = Roblox.FindFirstChild(Player, "RightFoot");

					if (!RightFoot)
						continue;


					/*Get Position*/

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);

					D3DXVECTOR3 UpperPosition = Roblox.GetPartPosition(UpperTorso);
					D3DXVECTOR3 LowerPosition = Roblox.GetPartPosition(LowerTorso);

					/*Left Arm*/
					D3DXVECTOR3 LeftUpperPosition = Roblox.GetPartPosition(LeftUpper);
					D3DXVECTOR3 LeftLowerPosition = Roblox.GetPartPosition(LeftLower);
					D3DXVECTOR3 LeftHandPosition = Roblox.GetPartPosition(LeftHand);

					/*Right Arm*/
					D3DXVECTOR3 RightUpperPosition = Roblox.GetPartPosition(RightUpper);
					D3DXVECTOR3 RightLowerPosition = Roblox.GetPartPosition(RightLower);
					D3DXVECTOR3 RightHandPosition = Roblox.GetPartPosition(RightHand);

					/*Left Leg*/
					D3DXVECTOR3 LeftUpperLegPosition = Roblox.GetPartPosition(LeftUpperLeg);
					D3DXVECTOR3 LeftLowerLegPosition = Roblox.GetPartPosition(LeftLowerLeg);
					D3DXVECTOR3 LeftFootPosition = Roblox.GetPartPosition(LeftFoot);

					/*Right Leg*/
					D3DXVECTOR3 RightUpperLegPosition = Roblox.GetPartPosition(RightUpperLeg);
					D3DXVECTOR3 RightLowerLegPosition = Roblox.GetPartPosition(RightLowerLeg);
					D3DXVECTOR3 RightFootPosition = Roblox.GetPartPosition(RightFoot);

					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

					D3DXVECTOR2 Position2;
					bool IfUpperTorso = Roblox.WorldToScreen(UpperPosition, Position2);

					D3DXVECTOR2 Position3;
					bool IfLowerTorso = Roblox.WorldToScreen(LowerPosition, Position3);

					D3DXVECTOR2 Position4;
					bool IfLeftUpper = Roblox.WorldToScreen(LeftUpperPosition, Position4);

					D3DXVECTOR2 Position5;
					bool IfLeftLower = Roblox.WorldToScreen(LeftLowerPosition, Position5);

					D3DXVECTOR2 Position6;
					bool IfLeftHand = Roblox.WorldToScreen(LeftHandPosition, Position6);

					D3DXVECTOR2 Position7;
					bool IfRightUpper = Roblox.WorldToScreen(RightUpperPosition, Position7);

					D3DXVECTOR2 Position8;
					bool IfRightLower = Roblox.WorldToScreen(RightLowerPosition, Position8);

					D3DXVECTOR2 Position9;
					bool IfRightHand = Roblox.WorldToScreen(RightHandPosition, Position9);

					D3DXVECTOR2 Position10;
					bool IfLeftUpperLeg = Roblox.WorldToScreen(LeftUpperLegPosition, Position10);

					D3DXVECTOR2 Position11;
					bool IfLeftLowerLeg = Roblox.WorldToScreen(LeftLowerLegPosition, Position11);

					D3DXVECTOR2 Position12;
					bool IfLeftFoot = Roblox.WorldToScreen(LeftFootPosition, Position12);

					D3DXVECTOR2 Position13;
					bool IfRightUpperLeg = Roblox.WorldToScreen(RightUpperLegPosition, Position13);

					D3DXVECTOR2 Position14;
					bool IfRightLowerLeg = Roblox.WorldToScreen(RightLowerLegPosition, Position14);

					D3DXVECTOR2 Position15;
					bool IfRightFoot = Roblox.WorldToScreen(RightFootPosition, Position15);

					if (IfHead && IfUpperTorso && IfLowerTorso && IfLeftUpper && IfLeftLower && IfLeftHand && IfRightUpper && IfRightLower && IfRightHand && IfLeftUpperLeg && IfLeftLowerLeg && IfLeftFoot && IfRightUpperLeg && IfRightLowerLeg && IfRightFoot)
					{
						/*Head -> Upper Torso*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);

						/*Upper Torso -> Lower Torso*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);

						/*Upper Torso -> Left Upper Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Upper Arm -> Left Lower Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Lower Arm -> Left Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);

						/*<><><><><><>*/

						/*Upper Torso -> Right Upper Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Upper Arm -> Right Lower Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position7.x, Position7.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Lower Arm -> Right Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower half of body*/

						/*Lower Torso -> Left Upper Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Upper Leg -> Lower Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position10.x, Position10.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Leg -> Left Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Torso -> Right Upper Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Upper Leg -> Lower Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position13.x, Position13.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Leg -> Right Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::SkeletonColor, 2);

					}
				}

				if (Celestial::Globals::Game == "Apoc 2")
				{
					std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

					if (!Head)
						continue;

					std::uintptr_t UpperTorso = Roblox.FindFirstChild(Player, "UpperTorso");

					if (!UpperTorso)
						continue;

					std::uintptr_t LowerTorso = Roblox.FindFirstChild(Player, "LowerTorso");

					if (!LowerTorso)
						continue;

					/*Left Arm*/

					std::uintptr_t LeftUpper = Roblox.FindFirstChild(Player, "LeftUpperArm");

					if (!LeftUpper)
						continue;

					std::uintptr_t LeftLower = Roblox.FindFirstChild(Player, "LeftLowerArm");

					if (!LeftLower)
						continue;

					std::uintptr_t LeftHand = Roblox.FindFirstChild(Player, "LeftHand");

					if (!LeftHand)
						continue;

					/*Right Arm*/

					std::uintptr_t RightUpper = Roblox.FindFirstChild(Player, "RightUpperArm");

					if (!RightUpper)
						continue;

					std::uintptr_t RightLower = Roblox.FindFirstChild(Player, "RightLowerArm");

					if (!RightLower)
						continue;

					std::uintptr_t RightHand = Roblox.FindFirstChild(Player, "RightHand");

					if (!RightHand)
						continue;

					/*Left Leg*/

					std::uintptr_t LeftUpperLeg = Roblox.FindFirstChild(Player, "LeftUpperLeg");

					if (!LeftUpperLeg)
						continue;

					std::uintptr_t LeftLowerLeg = Roblox.FindFirstChild(Player, "LeftLowerLeg");

					if (!LeftLowerLeg)
						continue;

					std::uintptr_t LeftFoot = Roblox.FindFirstChild(Player, "LeftFoot");

					if (!LeftFoot)
						continue;

					/*Right Leg*/

					std::uintptr_t RightUpperLeg = Roblox.FindFirstChild(Player, "RightUpperLeg");

					if (!RightUpperLeg)
						continue;

					std::uintptr_t RightLowerLeg = Roblox.FindFirstChild(Player, "RightLowerLeg");

					if (!RightLowerLeg)
						continue;

					std::uintptr_t RightFoot = Roblox.FindFirstChild(Player, "RightFoot");

					if (!RightFoot)
						continue;


					/*Get Position*/

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);

					D3DXVECTOR3 UpperPosition = Roblox.GetPartPosition(UpperTorso);
					D3DXVECTOR3 LowerPosition = Roblox.GetPartPosition(LowerTorso);

					/*Left Arm*/
					D3DXVECTOR3 LeftUpperPosition = Roblox.GetPartPosition(LeftUpper);
					D3DXVECTOR3 LeftLowerPosition = Roblox.GetPartPosition(LeftLower);
					D3DXVECTOR3 LeftHandPosition = Roblox.GetPartPosition(LeftHand);

					/*Right Arm*/
					D3DXVECTOR3 RightUpperPosition = Roblox.GetPartPosition(RightUpper);
					D3DXVECTOR3 RightLowerPosition = Roblox.GetPartPosition(RightLower);
					D3DXVECTOR3 RightHandPosition = Roblox.GetPartPosition(RightHand);

					/*Left Leg*/
					D3DXVECTOR3 LeftUpperLegPosition = Roblox.GetPartPosition(LeftUpperLeg);
					D3DXVECTOR3 LeftLowerLegPosition = Roblox.GetPartPosition(LeftLowerLeg);
					D3DXVECTOR3 LeftFootPosition = Roblox.GetPartPosition(LeftFoot);

					/*Right Leg*/
					D3DXVECTOR3 RightUpperLegPosition = Roblox.GetPartPosition(RightUpperLeg);
					D3DXVECTOR3 RightLowerLegPosition = Roblox.GetPartPosition(RightLowerLeg);
					D3DXVECTOR3 RightFootPosition = Roblox.GetPartPosition(RightFoot);

					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

					D3DXVECTOR2 Position2;
					bool IfUpperTorso = Roblox.WorldToScreen(UpperPosition, Position2);

					D3DXVECTOR2 Position3;
					bool IfLowerTorso = Roblox.WorldToScreen(LowerPosition, Position3);

					D3DXVECTOR2 Position4;
					bool IfLeftUpper = Roblox.WorldToScreen(LeftUpperPosition, Position4);

					D3DXVECTOR2 Position5;
					bool IfLeftLower = Roblox.WorldToScreen(LeftLowerPosition, Position5);

					D3DXVECTOR2 Position6;
					bool IfLeftHand = Roblox.WorldToScreen(LeftHandPosition, Position6);

					D3DXVECTOR2 Position7;
					bool IfRightUpper = Roblox.WorldToScreen(RightUpperPosition, Position7);

					D3DXVECTOR2 Position8;
					bool IfRightLower = Roblox.WorldToScreen(RightLowerPosition, Position8);

					D3DXVECTOR2 Position9;
					bool IfRightHand = Roblox.WorldToScreen(RightHandPosition, Position9);

					D3DXVECTOR2 Position10;
					bool IfLeftUpperLeg = Roblox.WorldToScreen(LeftUpperLegPosition, Position10);

					D3DXVECTOR2 Position11;
					bool IfLeftLowerLeg = Roblox.WorldToScreen(LeftLowerLegPosition, Position11);

					D3DXVECTOR2 Position12;
					bool IfLeftFoot = Roblox.WorldToScreen(LeftFootPosition, Position12);

					D3DXVECTOR2 Position13;
					bool IfRightUpperLeg = Roblox.WorldToScreen(RightUpperLegPosition, Position13);

					D3DXVECTOR2 Position14;
					bool IfRightLowerLeg = Roblox.WorldToScreen(RightLowerLegPosition, Position14);

					D3DXVECTOR2 Position15;
					bool IfRightFoot = Roblox.WorldToScreen(RightFootPosition, Position15);

					if (IfHead && IfUpperTorso && IfLowerTorso && IfLeftUpper && IfLeftLower && IfLeftHand && IfRightUpper && IfRightLower && IfRightHand && IfLeftUpperLeg && IfLeftLowerLeg && IfLeftFoot && IfRightUpperLeg && IfRightLowerLeg && IfRightFoot)
					{
						/*Head -> Upper Torso*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);

						/*Upper Torso -> Lower Torso*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);

						/*Upper Torso -> Left Upper Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Upper Arm -> Left Lower Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Lower Arm -> Left Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);

						/*<><><><><><>*/

						/*Upper Torso -> Right Upper Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Upper Arm -> Right Lower Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position7.x, Position7.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Lower Arm -> Right Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower half of body*/

						/*Lower Torso -> Left Upper Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Upper Leg -> Lower Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position10.x, Position10.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Leg -> Left Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Torso -> Right Upper Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Upper Leg -> Lower Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position13.x, Position13.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Leg -> Right Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::SkeletonColor, 2);
					}
				}

				if (Celestial::Globals::Game == "Recoil")
				{
					if (!Globals::Teamcheck)
					{
						std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

						if (!Head)
							continue;

						std::uintptr_t UpperTorso = Roblox.FindFirstChild(Player, "UpperTorso");

						if (!UpperTorso)
							continue;

						std::uintptr_t LowerTorso = Roblox.FindFirstChild(Player, "LowerTorso");

						if (!LowerTorso)
							continue;

						/*Left Arm*/

						std::uintptr_t LeftUpper = Roblox.FindFirstChild(Player, "LeftUpperArm");

						if (!LeftUpper)
							continue;

						std::uintptr_t LeftLower = Roblox.FindFirstChild(Player, "LeftLowerArm");

						if (!LeftLower)
							continue;

						std::uintptr_t LeftHand = Roblox.FindFirstChild(Player, "LeftHand");

						if (!LeftHand)
							continue;

						/*Right Arm*/

						std::uintptr_t RightUpper = Roblox.FindFirstChild(Player, "RightUpperArm");

						if (!RightUpper)
							continue;

						std::uintptr_t RightLower = Roblox.FindFirstChild(Player, "RightLowerArm");

						if (!RightLower)
							continue;

						std::uintptr_t RightHand = Roblox.FindFirstChild(Player, "RightHand");

						if (!RightHand)
							continue;

						/*Left Leg*/

						std::uintptr_t LeftUpperLeg = Roblox.FindFirstChild(Player, "LeftUpperLeg");

						if (!LeftUpperLeg)
							continue;

						std::uintptr_t LeftLowerLeg = Roblox.FindFirstChild(Player, "LeftLowerLeg");

						if (!LeftLowerLeg)
							continue;

						std::uintptr_t LeftFoot = Roblox.FindFirstChild(Player, "LeftFoot");

						if (!LeftFoot)
							continue;

						/*Right Leg*/

						std::uintptr_t RightUpperLeg = Roblox.FindFirstChild(Player, "RightUpperLeg");

						if (!RightUpperLeg)
							continue;

						std::uintptr_t RightLowerLeg = Roblox.FindFirstChild(Player, "RightLowerLeg");

						if (!RightLowerLeg)
							continue;

						std::uintptr_t RightFoot = Roblox.FindFirstChild(Player, "RightFoot");

						if (!RightFoot)
							continue;


						/*Get Position*/

						D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);

						D3DXVECTOR3 UpperPosition = Roblox.GetPartPosition(UpperTorso);
						D3DXVECTOR3 LowerPosition = Roblox.GetPartPosition(LowerTorso);

						/*Left Arm*/
						D3DXVECTOR3 LeftUpperPosition = Roblox.GetPartPosition(LeftUpper);
						D3DXVECTOR3 LeftLowerPosition = Roblox.GetPartPosition(LeftLower);
						D3DXVECTOR3 LeftHandPosition = Roblox.GetPartPosition(LeftHand);

						/*Right Arm*/
						D3DXVECTOR3 RightUpperPosition = Roblox.GetPartPosition(RightUpper);
						D3DXVECTOR3 RightLowerPosition = Roblox.GetPartPosition(RightLower);
						D3DXVECTOR3 RightHandPosition = Roblox.GetPartPosition(RightHand);

						/*Left Leg*/
						D3DXVECTOR3 LeftUpperLegPosition = Roblox.GetPartPosition(LeftUpperLeg);
						D3DXVECTOR3 LeftLowerLegPosition = Roblox.GetPartPosition(LeftLowerLeg);
						D3DXVECTOR3 LeftFootPosition = Roblox.GetPartPosition(LeftFoot);

						/*Right Leg*/
						D3DXVECTOR3 RightUpperLegPosition = Roblox.GetPartPosition(RightUpperLeg);
						D3DXVECTOR3 RightLowerLegPosition = Roblox.GetPartPosition(RightLowerLeg);
						D3DXVECTOR3 RightFootPosition = Roblox.GetPartPosition(RightFoot);

						D3DXVECTOR2 Position1;
						bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

						D3DXVECTOR2 Position2;
						bool IfUpperTorso = Roblox.WorldToScreen(UpperPosition, Position2);

						D3DXVECTOR2 Position3;
						bool IfLowerTorso = Roblox.WorldToScreen(LowerPosition, Position3);

						D3DXVECTOR2 Position4;
						bool IfLeftUpper = Roblox.WorldToScreen(LeftUpperPosition, Position4);

						D3DXVECTOR2 Position5;
						bool IfLeftLower = Roblox.WorldToScreen(LeftLowerPosition, Position5);

						D3DXVECTOR2 Position6;
						bool IfLeftHand = Roblox.WorldToScreen(LeftHandPosition, Position6);

						D3DXVECTOR2 Position7;
						bool IfRightUpper = Roblox.WorldToScreen(RightUpperPosition, Position7);

						D3DXVECTOR2 Position8;
						bool IfRightLower = Roblox.WorldToScreen(RightLowerPosition, Position8);

						D3DXVECTOR2 Position9;
						bool IfRightHand = Roblox.WorldToScreen(RightHandPosition, Position9);

						D3DXVECTOR2 Position10;
						bool IfLeftUpperLeg = Roblox.WorldToScreen(LeftUpperLegPosition, Position10);

						D3DXVECTOR2 Position11;
						bool IfLeftLowerLeg = Roblox.WorldToScreen(LeftLowerLegPosition, Position11);

						D3DXVECTOR2 Position12;
						bool IfLeftFoot = Roblox.WorldToScreen(LeftFootPosition, Position12);

						D3DXVECTOR2 Position13;
						bool IfRightUpperLeg = Roblox.WorldToScreen(RightUpperLegPosition, Position13);

						D3DXVECTOR2 Position14;
						bool IfRightLowerLeg = Roblox.WorldToScreen(RightLowerLegPosition, Position14);

						D3DXVECTOR2 Position15;
						bool IfRightFoot = Roblox.WorldToScreen(RightFootPosition, Position15);

						if (IfHead && IfUpperTorso && IfLowerTorso && IfLeftUpper && IfLeftLower && IfLeftHand && IfRightUpper && IfRightLower && IfRightHand && IfLeftUpperLeg && IfLeftLowerLeg && IfLeftFoot && IfRightUpperLeg && IfRightLowerLeg && IfRightFoot)
						{
							/*Head -> Upper Torso*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);

							/*Upper Torso -> Lower Torso*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);

							/*Upper Torso -> Left Upper Arm*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);

							/*Left Upper Arm -> Left Lower Arm*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::SkeletonColor, 2);

							/*Left Lower Arm -> Left Hand*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);

							/*<><><><><><>*/

							/*Upper Torso -> Right Upper Arm*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);

							/*Right Upper Arm -> Right Lower Arm*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position7.x, Position7.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::SkeletonColor, 2);

							/*Right Lower Arm -> Right Hand*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower half of body*/

							/*Lower Torso -> Left Upper Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::SkeletonColor, 2);

							/*Left Upper Leg -> Lower Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position10.x, Position10.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower Leg -> Left Foot*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower Torso -> Right Upper Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::SkeletonColor, 2);

							/*Right Upper Leg -> Lower Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position13.x, Position13.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower Leg -> Right Foot*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::SkeletonColor, 2);
						}
					}

					if (Globals::Teamcheck)
					{
						std::uintptr_t UpperTorso = Roblox.FindFirstChild(Player, "UpperTorso5");

						if (!UpperTorso)
							continue;

						std::uintptr_t LowerTorso = Roblox.FindFirstChild(Player, "LowerTorso");

						if (!LowerTorso)
							continue;

						/*Left Arm*/

						std::uintptr_t LeftUpper = Roblox.FindFirstChild(Player, "LeftUpperArm");

						if (!LeftUpper)
							continue;

						std::uintptr_t LeftLower = Roblox.FindFirstChild(Player, "LeftLowerArm");

						if (!LeftLower)
							continue;

						std::uintptr_t LeftHand = Roblox.FindFirstChild(Player, "LeftHand");

						if (!LeftHand)
							continue;

						/*Right Arm*/

						std::uintptr_t RightUpper = Roblox.FindFirstChild(Player, "RightUpperArm");

						if (!RightUpper)
							continue;

						std::uintptr_t RightLower = Roblox.FindFirstChild(Player, "RightLowerArm");

						if (!RightLower)
							continue;

						std::uintptr_t RightHand = Roblox.FindFirstChild(Player, "RightHand");

						if (!RightHand)
							continue;

						/*Left Leg*/

						std::uintptr_t LeftUpperLeg = Roblox.FindFirstChild(Player, "LeftUpperLeg");

						if (!LeftUpperLeg)
							continue;

						std::uintptr_t LeftLowerLeg = Roblox.FindFirstChild(Player, "LeftLowerLeg");

						if (!LeftLowerLeg)
							continue;

						std::uintptr_t LeftFoot = Roblox.FindFirstChild(Player, "LeftFoot");

						if (!LeftFoot)
							continue;

						/*Right Leg*/

						std::uintptr_t RightUpperLeg = Roblox.FindFirstChild(Player, "RightUpperLeg");

						if (!RightUpperLeg)
							continue;

						std::uintptr_t RightLowerLeg = Roblox.FindFirstChild(Player, "RightLowerLeg");

						if (!RightLowerLeg)
							continue;

						std::uintptr_t RightFoot = Roblox.FindFirstChild(Player, "RightFoot");

						if (!RightFoot)
							continue;


						/*Get Position*/

						D3DXVECTOR3 UpperPosition = Roblox.GetPartPosition(UpperTorso);
						D3DXVECTOR3 LowerPosition = Roblox.GetPartPosition(LowerTorso);

						/*Left Arm*/
						D3DXVECTOR3 LeftUpperPosition = Roblox.GetPartPosition(LeftUpper);
						D3DXVECTOR3 LeftLowerPosition = Roblox.GetPartPosition(LeftLower);
						D3DXVECTOR3 LeftHandPosition = Roblox.GetPartPosition(LeftHand);

						/*Right Arm*/
						D3DXVECTOR3 RightUpperPosition = Roblox.GetPartPosition(RightUpper);
						D3DXVECTOR3 RightLowerPosition = Roblox.GetPartPosition(RightLower);
						D3DXVECTOR3 RightHandPosition = Roblox.GetPartPosition(RightHand);

						/*Left Leg*/
						D3DXVECTOR3 LeftUpperLegPosition = Roblox.GetPartPosition(LeftUpperLeg);
						D3DXVECTOR3 LeftLowerLegPosition = Roblox.GetPartPosition(LeftLowerLeg);
						D3DXVECTOR3 LeftFootPosition = Roblox.GetPartPosition(LeftFoot);

						/*Right Leg*/
						D3DXVECTOR3 RightUpperLegPosition = Roblox.GetPartPosition(RightUpperLeg);
						D3DXVECTOR3 RightLowerLegPosition = Roblox.GetPartPosition(RightLowerLeg);
						D3DXVECTOR3 RightFootPosition = Roblox.GetPartPosition(RightFoot);


						D3DXVECTOR2 Position2;
						bool IfUpperTorso = Roblox.WorldToScreen(UpperPosition, Position2);

						D3DXVECTOR2 Position3;
						bool IfLowerTorso = Roblox.WorldToScreen(LowerPosition, Position3);

						D3DXVECTOR2 Position4;
						bool IfLeftUpper = Roblox.WorldToScreen(LeftUpperPosition, Position4);

						D3DXVECTOR2 Position5;
						bool IfLeftLower = Roblox.WorldToScreen(LeftLowerPosition, Position5);

						D3DXVECTOR2 Position6;
						bool IfLeftHand = Roblox.WorldToScreen(LeftHandPosition, Position6);

						D3DXVECTOR2 Position7;
						bool IfRightUpper = Roblox.WorldToScreen(RightUpperPosition, Position7);

						D3DXVECTOR2 Position8;
						bool IfRightLower = Roblox.WorldToScreen(RightLowerPosition, Position8);

						D3DXVECTOR2 Position9;
						bool IfRightHand = Roblox.WorldToScreen(RightHandPosition, Position9);

						D3DXVECTOR2 Position10;
						bool IfLeftUpperLeg = Roblox.WorldToScreen(LeftUpperLegPosition, Position10);

						D3DXVECTOR2 Position11;
						bool IfLeftLowerLeg = Roblox.WorldToScreen(LeftLowerLegPosition, Position11);

						D3DXVECTOR2 Position12;
						bool IfLeftFoot = Roblox.WorldToScreen(LeftFootPosition, Position12);

						D3DXVECTOR2 Position13;
						bool IfRightUpperLeg = Roblox.WorldToScreen(RightUpperLegPosition, Position13);

						D3DXVECTOR2 Position14;
						bool IfRightLowerLeg = Roblox.WorldToScreen(RightLowerLegPosition, Position14);

						D3DXVECTOR2 Position15;
						bool IfRightFoot = Roblox.WorldToScreen(RightFootPosition, Position15);

						if (IfUpperTorso && IfLowerTorso && IfLeftUpper && IfLeftLower && IfLeftHand && IfRightUpper && IfRightLower && IfRightHand && IfLeftUpperLeg && IfLeftLowerLeg && IfLeftFoot && IfRightUpperLeg && IfRightLowerLeg && IfRightFoot)
						{
							///*Upper Torso -> Lower Torso*/
							//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);

							/*Upper Torso -> Left Upper Arm*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);

							/*Left Upper Arm -> Left Lower Arm*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::SkeletonColor, 2);

							/*Left Lower Arm -> Left Hand*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);

							///*<><><><><><>*/

							///*Upper Torso -> Right Upper Arm*/
							//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);

							/*Right Upper Arm -> Right Lower Arm*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position7.x, Position7.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::SkeletonColor, 2);

							/*Right Lower Arm -> Right Hand*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower half of body*/

							/*Lower Torso -> Left Upper Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::SkeletonColor, 2);

							/*Left Upper Leg -> Lower Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position10.x, Position10.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower Leg -> Left Foot*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower Torso -> Right Upper Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::SkeletonColor, 2);

							/*Right Upper Leg -> Lower Leg*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position13.x, Position13.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::SkeletonColor, 2);

							/*Lower Leg -> Right Foot*/
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::SkeletonColor, 2);
						}
					}
				}

				if (Celestial::Globals::Game == "Frontlines")
				{
					std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

					if (!Head)
						continue;

					std::uintptr_t UpperTorso = Roblox.FindFirstChild(Player, "UpperTorso");

					if (!UpperTorso)
						continue;

					std::uintptr_t LowerTorso = Roblox.FindFirstChild(Player, "LowerTorso");

					if (!LowerTorso)
						continue;

					/*Left Arm*/

					std::uintptr_t LeftUpper = Roblox.FindFirstChild(Player, "LeftUpperArm");

					if (!LeftUpper)
						continue;

					std::uintptr_t LeftLower = Roblox.FindFirstChild(Player, "LeftLowerArm");

					if (!LeftLower)
						continue;

					std::uintptr_t LeftHand = Roblox.FindFirstChild(Player, "LeftHand");

					if (!LeftHand)
						continue;

					/*Right Arm*/

					std::uintptr_t RightUpper = Roblox.FindFirstChild(Player, "RightUpperArm");

					if (!RightUpper)
						continue;

					std::uintptr_t RightLower = Roblox.FindFirstChild(Player, "RightLowerArm");

					if (!RightLower)
						continue;

					std::uintptr_t RightHand = Roblox.FindFirstChild(Player, "RightHand");

					if (!RightHand)
						continue;

					/*Left Leg*/

					std::uintptr_t LeftUpperLeg = Roblox.FindFirstChild(Player, "LeftUpperLeg");

					if (!LeftUpperLeg)
						continue;

					std::uintptr_t LeftLowerLeg = Roblox.FindFirstChild(Player, "LeftLowerLeg");

					if (!LeftLowerLeg)
						continue;

					std::uintptr_t LeftFoot = Roblox.FindFirstChild(Player, "LeftFoot");

					if (!LeftFoot)
						continue;

					/*Right Leg*/

					std::uintptr_t RightUpperLeg = Roblox.FindFirstChild(Player, "RightUpperLeg");

					if (!RightUpperLeg)
						continue;

					std::uintptr_t RightLowerLeg = Roblox.FindFirstChild(Player, "RightLowerLeg");

					if (!RightLowerLeg)
						continue;

					std::uintptr_t RightFoot = Roblox.FindFirstChild(Player, "RightFoot");

					if (!RightFoot)
						continue;


					/*Get Position*/

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);

					D3DXVECTOR3 UpperPosition = Roblox.GetPartPosition(UpperTorso);
					D3DXVECTOR3 LowerPosition = Roblox.GetPartPosition(LowerTorso);

					/*Left Arm*/
					D3DXVECTOR3 LeftUpperPosition = Roblox.GetPartPosition(LeftUpper);
					D3DXVECTOR3 LeftLowerPosition = Roblox.GetPartPosition(LeftLower);
					D3DXVECTOR3 LeftHandPosition = Roblox.GetPartPosition(LeftHand);

					/*Right Arm*/
					D3DXVECTOR3 RightUpperPosition = Roblox.GetPartPosition(RightUpper);
					D3DXVECTOR3 RightLowerPosition = Roblox.GetPartPosition(RightLower);
					D3DXVECTOR3 RightHandPosition = Roblox.GetPartPosition(RightHand);

					/*Left Leg*/
					D3DXVECTOR3 LeftUpperLegPosition = Roblox.GetPartPosition(LeftUpperLeg);
					D3DXVECTOR3 LeftLowerLegPosition = Roblox.GetPartPosition(LeftLowerLeg);
					D3DXVECTOR3 LeftFootPosition = Roblox.GetPartPosition(LeftFoot);

					/*Right Leg*/
					D3DXVECTOR3 RightUpperLegPosition = Roblox.GetPartPosition(RightUpperLeg);
					D3DXVECTOR3 RightLowerLegPosition = Roblox.GetPartPosition(RightLowerLeg);
					D3DXVECTOR3 RightFootPosition = Roblox.GetPartPosition(RightFoot);

					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

					D3DXVECTOR2 Position2;
					bool IfUpperTorso = Roblox.WorldToScreen(UpperPosition, Position2);

					D3DXVECTOR2 Position3;
					bool IfLowerTorso = Roblox.WorldToScreen(LowerPosition, Position3);

					D3DXVECTOR2 Position4;
					bool IfLeftUpper = Roblox.WorldToScreen(LeftUpperPosition, Position4);

					D3DXVECTOR2 Position5;
					bool IfLeftLower = Roblox.WorldToScreen(LeftLowerPosition, Position5);

					D3DXVECTOR2 Position6;
					bool IfLeftHand = Roblox.WorldToScreen(LeftHandPosition, Position6);

					D3DXVECTOR2 Position7;
					bool IfRightUpper = Roblox.WorldToScreen(RightUpperPosition, Position7);

					D3DXVECTOR2 Position8;
					bool IfRightLower = Roblox.WorldToScreen(RightLowerPosition, Position8);

					D3DXVECTOR2 Position9;
					bool IfRightHand = Roblox.WorldToScreen(RightHandPosition, Position9);

					D3DXVECTOR2 Position10;
					bool IfLeftUpperLeg = Roblox.WorldToScreen(LeftUpperLegPosition, Position10);

					D3DXVECTOR2 Position11;
					bool IfLeftLowerLeg = Roblox.WorldToScreen(LeftLowerLegPosition, Position11);

					D3DXVECTOR2 Position12;
					bool IfLeftFoot = Roblox.WorldToScreen(LeftFootPosition, Position12);

					D3DXVECTOR2 Position13;
					bool IfRightUpperLeg = Roblox.WorldToScreen(RightUpperLegPosition, Position13);

					D3DXVECTOR2 Position14;
					bool IfRightLowerLeg = Roblox.WorldToScreen(RightLowerLegPosition, Position14);

					D3DXVECTOR2 Position15;
					bool IfRightFoot = Roblox.WorldToScreen(RightFootPosition, Position15);

					if (IfHead && IfUpperTorso && IfLowerTorso && IfLeftUpper && IfLeftLower && IfLeftHand && IfRightUpper && IfRightLower && IfRightHand && IfLeftUpperLeg && IfLeftLowerLeg && IfLeftFoot && IfRightUpperLeg && IfRightLowerLeg && IfRightFoot)
					{
						/*Head -> Upper Torso*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);

						/*Upper Torso -> Lower Torso*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);

						/*Upper Torso -> Left Upper Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Upper Arm -> Left Lower Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position4.x, Position4.y), ImVec2(Position5.x, Position5.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Lower Arm -> Left Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position5.x, Position5.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);

						/*<><><><><><>*/

						/*Upper Torso -> Right Upper Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Upper Arm -> Right Lower Arm*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position7.x, Position7.y), ImVec2(Position8.x, Position8.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Lower Arm -> Right Hand*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position8.x, Position8.y), ImVec2(Position9.x, Position9.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower half of body*/

						/*Lower Torso -> Left Upper Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position10.x, Position10.y), (ImColor)Globals::SkeletonColor, 2);

						/*Left Upper Leg -> Lower Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position10.x, Position10.y), ImVec2(Position11.x, Position11.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Leg -> Left Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position11.x, Position11.y), ImVec2(Position12.x, Position12.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Torso -> Right Upper Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position3.x, Position3.y), ImVec2(Position13.x, Position13.y), (ImColor)Globals::SkeletonColor, 2);

						/*Right Upper Leg -> Lower Leg*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position13.x, Position13.y), ImVec2(Position14.x, Position14.y), (ImColor)Globals::SkeletonColor, 2);

						/*Lower Leg -> Right Foot*/
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position14.x, Position14.y), ImVec2(Position15.x, Position15.y), (ImColor)Globals::SkeletonColor, 2);
					}
				}

				if (Celestial::Globals::Game == "Criminality")
				{
					std::uintptr_t Head = Roblox.FindFirstChild(Player, "Head");

					if (!Head)
						continue;

					std::uintptr_t Torso = Roblox.FindFirstChild(Player, "HumanoidRootPart");

					if (!Torso)
						continue;

					std::uintptr_t LeftArm = Roblox.FindFirstChild(Player, "Left Arm");

					if (!LeftArm)
						continue;

					std::uintptr_t RightArm = Roblox.FindFirstChild(Player, "Right Arm");

					if (!RightArm)
						continue;

					std::uintptr_t LeftLeg = Roblox.FindFirstChild(Player, "Left Leg");

					if (!LeftLeg)
						continue;

					std::uintptr_t RightLeg = Roblox.FindFirstChild(Player, "Right Leg");

					if (!RightLeg)
						continue;

					D3DXVECTOR3 HeadPosition = Roblox.GetPartPosition(Head);
					D3DXVECTOR3 TorsoPosition = Roblox.GetPartPosition(Torso);
					D3DXVECTOR3 LeftArmPosition = Roblox.GetPartPosition(LeftArm);
					D3DXVECTOR3 RightArmPosition = Roblox.GetPartPosition(RightArm);
					D3DXVECTOR3 RootPartPosition = Roblox.GetPartPosition(HumanoidRootPart);
					D3DXVECTOR3 LeftLegPosition = Roblox.GetPartPosition(LeftLeg);
					D3DXVECTOR3 RightLegPosition = Roblox.GetPartPosition(RightLeg);

					D3DXVECTOR2 Position1;
					bool IfHead = Roblox.WorldToScreen(HeadPosition, Position1);

					D3DXVECTOR2 Position2;
					bool IfTorso = Roblox.WorldToScreen(TorsoPosition, Position2);

					D3DXVECTOR2 Position3;
					bool IfLeftArm = Roblox.WorldToScreen(LeftArmPosition, Position3);

					D3DXVECTOR2 Position4;
					bool IfRightArm = Roblox.WorldToScreen(RightArmPosition, Position4);

					D3DXVECTOR2 Position5;
					bool IfRootPart = Roblox.WorldToScreen(RootPartPosition, Position5);

					D3DXVECTOR2 Position6;
					bool IfLeftLeg = Roblox.WorldToScreen(LeftLegPosition, Position6);

					D3DXVECTOR2 Position7;
					bool IfRightLeg = Roblox.WorldToScreen(RightLegPosition, Position7);

					if (IfHead && IfTorso && IfLeftArm && IfRightArm && IfRootPart && IfLeftLeg && IfRightLeg)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position1.x, Position1.y), ImVec2(Position2.x, Position2.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(HeadPosition_W2S.x, HeadPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position3.x, Position3.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerArmPosition_W2S.x, LeftLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position4.x, Position4.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerArmPosition_W2S.x, RightLowerArmPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position6.x, Position6.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(LeftLowerLegPosition_W2S.x, LeftLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Position2.x, Position2.y), ImVec2(Position7.x, Position7.y), (ImColor)Globals::SkeletonColor, 2);
						//ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RightLowerLegPosition_W2S.x, RightLowerLegPosition_W2S.y), 2, (ImColor)globals::ally_skeleton_color);
					}
				}
			}
		}
	}
}
