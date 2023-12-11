#pragma once

#include "World.h"

#include <Roblox/Globals/Globals.h>
#include <Utilities/Benchmark.h>

/*World Functions*/

std::vector<std::uintptr_t> Celestial::World::GetChildren(const std::uintptr_t Instance)
{
	std::vector<std::uintptr_t> children;

	if (!Instance)
		return children;

	const auto current = *reinterpret_cast<const std::uintptr_t*>(Instance + 0x2C);
	if (!current) return children;

	const auto end = *reinterpret_cast<const std::uintptr_t*>(current + 4u);

	if ((!end) || (end < 0))
	{
		return children;
	}

	for (auto parent = *reinterpret_cast<const std::uintptr_t*>(current); parent < end; parent += 8u)
		children.push_back(*reinterpret_cast<const std::uintptr_t*>(parent));

	return children;
}

std::string Celestial::World::GetClassType(const std::uintptr_t Name)
{
	if (!Name)
		return "None";

	std::uintptr_t ClassDescriptor = *reinterpret_cast<std::uintptr_t*>(Name + 0xC);

	return *reinterpret_cast<std::string*>(*reinterpret_cast<std::uintptr_t*>((ClassDescriptor + 0x4)));
}

std::uintptr_t Celestial::World::GetService(const std::uintptr_t Instance, std::string Class)
{
	if (!Instance)
		return NULL;

	for (std::uintptr_t Child : GetChildren(Instance))
	{
		if (GetClassType(Child) == Class) {
			return Child;
		}
	}
}

std::uintptr_t Celestial::World::FindFirstChild(const std::uintptr_t Instance, std::string Child)
{
	if (!Instance)
		return NULL;

	for (std::uintptr_t InsideChild : GetChildren(Instance)) {
		if (*reinterpret_cast<std::string*>(*reinterpret_cast<std::uintptr_t*>((InsideChild + 0x28))) == Child) {
			return static_cast<std::uintptr_t>(InsideChild);
		}
	}

	return NULL;
}


std::uintptr_t Celestial::World::GetWorkspace(const std::uintptr_t Instance)
{
	if (!Instance)
		return NULL;

	return { *reinterpret_cast<std::uintptr_t*>(Instance + 0x194) };
}

std::uintptr_t Celestial::World::GetPlayers(const std::uintptr_t Instance)
{
	if (!Instance)
		return NULL;

	std::uintptr_t Services = *reinterpret_cast<std::uintptr_t*>(Instance + 0x5C);
	if (!Services)
		return 0;

	std::uintptr_t Players = *reinterpret_cast<std::uintptr_t*>(Services + 0x68);
	if (!Players)
		return 0;

	return Players;
}

float* Celestial::World::GetProjection(const std::uintptr_t VisualEngine)
{
	if (!VisualEngine)
		return NULL;

	ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(VisualEngine + 0x170), &this->matrixprojection, sizeof(this->matrixprojection), 0);
	return this->matrixprojection;
}

D3DXVECTOR2 Celestial::World::GetRobloxDimensions(const std::uintptr_t VisualEngine)
{
	if (!VisualEngine)
		return { NULL, NULL };

	D3DXVECTOR2 Dimensions = { 0, 0 };

	std::uintptr_t Height = 0x6FC;

	Dimensions.x = *reinterpret_cast<float*>(VisualEngine + Height);
	Dimensions.y = *reinterpret_cast<float*>(VisualEngine + Height + sizeof(float));

	return { Dimensions };
}

bool Celestial::World::WorldToScreen(D3DXVECTOR3 Position, D3DXVECTOR2& ToScreen)
{
	D3DXVECTOR2 ClientDimensions = GetRobloxDimensions(Celestial::Globals::VisualEngine);
	
	float* Projection = GetProjection(Celestial::Globals::VisualEngine);

	D3DXVECTOR4 Coordinate = { NULL, NULL, NULL, NULL};
	Coordinate.x = (Position.x * Projection[0]) + (Position.y * Projection[1]) + (Position.z * Projection[2]) + Projection[3];
	Coordinate.y = (Position.x * Projection[4]) + (Position.y * Projection[5]) + (Position.z * Projection[6]) + Projection[7];
	Coordinate.z = (Position.x * Projection[8]) + (Position.y * Projection[9]) + (Position.z * Projection[10]) + Projection[11];
	Coordinate.w = (Position.x * Projection[12]) + (Position.y * Projection[13]) + (Position.z * Projection[14]) + Projection[15];

	if (Coordinate.w < 0.1f)
		return false;

	D3DXVECTOR3 NDC = {NULL, NULL, NULL};
	NDC.x = Coordinate.x / Coordinate.w;
	NDC.y = Coordinate.y / Coordinate.w;
	NDC.z = Coordinate.z / Coordinate.w;

	ToScreen.x = (ClientDimensions.x / 2 * NDC.x) + (NDC.x + ClientDimensions.x / 2);
	ToScreen.y = -(ClientDimensions.y / 2 * NDC.y) + (NDC.y + ClientDimensions.y / 2);

	return true;
}

D3DXVECTOR3 Celestial::World::GetCameraPosition(const std::uintptr_t Instance)
{
	D3DXVECTOR3 Position = { NULL, NULL, NULL };

	if(!Instance)
		return { NULL, NULL, NULL };

	Position = *reinterpret_cast<D3DXVECTOR3*>(Instance + 0x80 + 0x10);

	return { Position };
}

std::uintptr_t Celestial::World::GetCreatorId(const std::uintptr_t Instance)
{
	if (!Instance)
		return NULL;

	std::uintptr_t creator_id = *reinterpret_cast<std::uintptr_t*>(Instance + 0x408 + 0x8);
	std::uintptr_t obf = *(DWORD*)creator_id - creator_id;

	return obf;
}

bool Celestial::World::IsLoaded(const std::uintptr_t Instance)
{
	const auto datamodel_instance = Instance;
	const auto is_gameloaded = *reinterpret_cast<bool*>(datamodel_instance + 0x22C + 0x8);

	return is_gameloaded;
}

float Celestial::World::GetVector3Magnitude(D3DXVECTOR3 vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

float Celestial::World::GetVector2Magnitude(D3DXVECTOR2 vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

/*Player Functions*/

std::uintptr_t Celestial::World::GetLocalPlayer(const std::uintptr_t Players)
{
	if (!Players)
		return NULL;

	return { *reinterpret_cast<std::uintptr_t*>(Players + 0x138) };
}

std::uintptr_t Celestial::World::GetCharacter(const std::uintptr_t LocalPlayer)
{
	if (!LocalPlayer)
		return NULL;

	return { *reinterpret_cast<std::uintptr_t*>(LocalPlayer + 0x7C)};
}

std::uintptr_t Celestial::World::GetTeam(const std::uintptr_t Player)
{
	return { *reinterpret_cast<std::uintptr_t*>(Player + 0xB0) };
}

std::uintptr_t Celestial::World::GetBrickColorID(const std::uintptr_t Team)
{
	if (!Team)
		return NULL;

	return { *reinterpret_cast<std::uintptr_t*>(Team + 0x64) };
}

std::string Celestial::World::GetName(const std::uintptr_t Player)
{
	if (!Player)
		return "None";

	return *reinterpret_cast<std::string*>(*reinterpret_cast<std::uintptr_t*>((Player + 0x28)));
}

std::uintptr_t Celestial::World::GetPlayerHumanoid(const std::uintptr_t Player)
{
	if (!Player)
		return NULL;

	std::uintptr_t Humanoid = FindFirstChild(GetCharacter(Player), "Humanoid");

	if (!Humanoid)
		return NULL;

	return { Humanoid };
}

float Celestial::World::GetPlayerHealth(const std::uintptr_t Humanoid)
{
	if (!Humanoid)
		return NULL;

	union
	{
		std::uint32_t HealthObfuscation;
		float Conv;
	} Obfuscated;

	std::uintptr_t V1 = *reinterpret_cast<std::uintptr_t*>(Humanoid + 0x218);
	std::uintptr_t V2 = **reinterpret_cast<std::uintptr_t**>(Humanoid + 0x218);

	Obfuscated.HealthObfuscation = (V1 ^ V2);
	return Obfuscated.Conv;
}

float Celestial::World::GetMaxPlayerHealth(const std::uintptr_t Humanoid)
{
	if (!Humanoid)
		return NULL;

	union
	{
		std::uint32_t HealthObfuscation;
		float Conv;
	} Obfuscated;

	std::uintptr_t V1 = **reinterpret_cast<std::uintptr_t**>(Humanoid + 0x21C);
	std::uintptr_t V2 = *reinterpret_cast<std::uintptr_t*>(Humanoid + 0x21C);

	Obfuscated.HealthObfuscation = (V1 - V2);
	return Obfuscated.Conv;
}

D3DXVECTOR3 Celestial::World::GetPartPosition(const std::uintptr_t Part)
{
	D3DXVECTOR3 Position = { NULL, NULL, NULL };

	if (!Part)
		return { NULL, NULL, NULL };

	std::uintptr_t Primitive = *reinterpret_cast<std::uintptr_t*>(Part + 0xB8);

	if (!Primitive)
		return { NULL, NULL, NULL };

	Position = *reinterpret_cast<D3DXVECTOR3*>(Primitive + 0xFC);

	return { Position };
}

D3DXVECTOR3 Celestial::World::GetPrimitiveVelocity(const std::uintptr_t Part)
{
	D3DXVECTOR3 Velocity = { NULL, NULL, NULL };

	if (!Part)
		return { NULL, NULL, NULL };

	std::uintptr_t Primitive = *reinterpret_cast<std::uintptr_t*>(Part + 0xB8);

	if (!Primitive)
		return { NULL, NULL, NULL };

	Velocity = *reinterpret_cast<D3DXVECTOR3*>(Primitive + 0x108);

	return { Velocity };
}

D3DXVECTOR3 Celestial::World::GetLookVector(const std::uintptr_t Instance)
{
	if (!Instance)
		return { NULL, NULL, NULL };

	auto CFrame = reinterpret_cast<int(__thiscall*)(int)>(reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr)) + 0x12A0CD0 - 0xD40000)(*reinterpret_cast<std::uintptr_t*>(Instance + 0xC0) + 0x38);
	
	if (!CFrame)
		return { NULL, NULL, NULL };

	float LookX = *reinterpret_cast<float*>(CFrame + 0x18);
	float LookY = *reinterpret_cast<float*>(CFrame + 0x1C);
	float LookZ = -*reinterpret_cast<float*>(CFrame + 0x20);

	return { LookX, LookY, LookZ };
}

D3DXVECTOR3 Celestial::World::GetCFrame(const std::uintptr_t Camera)
{
	if (!Camera)
		return { NULL, NULL, NULL };

	float X = *reinterpret_cast<std::uintptr_t*>(Camera + 0xD8);
	float Y = *reinterpret_cast<std::uintptr_t*>(Camera + 0xDC);
	float Z = *reinterpret_cast<std::uintptr_t*>(Camera + 0xE0);

	float Rot0 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x114);
	float Rot1 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x118);
	float Rot2 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x11C);

	float Rot3 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x120);
	float Rot4 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x124);
	float Rot5 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x128);

	float Rot6 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x12C);
	float Rot7 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x130);
	float Rot8 = *reinterpret_cast<std::uintptr_t*>(Camera + 0x134);

	
}

std::uintptr_t Celestial::World::GetInstanceParent(const std::uintptr_t Instance)
{
	if (!Instance)
		return NULL;

	return *reinterpret_cast<std::uintptr_t*>(Instance + 0x34);
}

bool Celestial::World::IsDescendantOf(std::uintptr_t Instance, std::uintptr_t Old)
{
	if (!Instance)
		return NULL;

	int v3 = GetInstanceParent(Instance);

	if (Old == v3)
		return 1;
	else if (v3)
		return IsDescendantOf(v3, Old);

	return 0;
}

std::uintptr_t Celestial::World::GetPartAdornee(const std::uintptr_t Part)
{
	if (!Part)
		return NULL;

	return *reinterpret_cast<std::uintptr_t*>(Part + 0x31C);
}


std::uintptr_t Celestial::World::IsLocalCharacter()
{
	for (std::uintptr_t v : GetChildren(FindFirstChild(Celestial::Globals::Workspace, "Characters")))
	{
		D3DXVECTOR3 Pos = this->SubVec3(GetCameraPosition(Celestial::Globals::VisualEngine), GetPartPosition(FindFirstChild(FindFirstChild(v, "Body"), "Chest")));

		if (GetVector3Magnitude(Pos) < 5)
			return v;
	}
}

std::uintptr_t Celestial::World::GetPlayerFromCharacter(const std::uintptr_t Character)
{
	if (!Character)
		return NULL;

	for (std::uintptr_t Player : GetChildren(Globals::Players))
	{
		if (!Player)
			return NULL;

		if (GetCharacter(Player) == Character)
			return Player;
	}

	return NULL;
}

std::int32_t Celestial::World::GetRigType(const std::uintptr_t Humanoid)
{
	if (!Humanoid)
		return NULL;

	return *reinterpret_cast<std::uint8_t*>(Humanoid + 0x234);
}

std::uintptr_t Celestial::World::GetPrimaryPart(const std::uintptr_t Part)
{
	if (!Part)
		return NULL;
	
	return *reinterpret_cast<std::uintptr_t*>(Part + 0x108);
}

bool Celestial::World::IsPlayerOnTeam(std::uintptr_t Player)
{
	if (!Player)
		return NULL;

	for (std::uintptr_t v : GetChildren(FindFirstChild(Celestial::Globals::LocalPlayer, "PlayerGui")))
	{

		if (GetName(v) == "NameGui")
		{
			if (IsDescendantOf(GetPartAdornee(v), Player))
				return true;
		}
	}
	return false;
}


// gotta feeling ITS GONNA CRASH.
