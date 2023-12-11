#pragma once

#include <Roblox/World/World.h>

namespace Celestial
{
	class Renderer
	{
	public:
		void Hook();
		void Update();
	private:

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

		D3DXVECTOR2 ClientDimensions;
		D3DXVECTOR2 Center;
		D3DXVECTOR3 Camera;
		POINT CursorPosition;
		std::vector<std::uintptr_t> Players;

		std::uintptr_t LocalPlayer;
	};
	class Aimbot
	{
	public:
		void Hook();
		D3DXVECTOR2 GrabClosestToCursor();
		D3DXVECTOR2 GetPlayerRelativeToCursor(D3DXVECTOR2 Position);
		void Update();
	private:
		D3DXVECTOR2 ClientDimensions;
		D3DXVECTOR2 Center;
		D3DXVECTOR3 Camera;
		POINT CursorPosition;
		std::vector<std::uintptr_t> Players;

		std::uintptr_t LocalPlayer;

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

		D3DXVECTOR3 AddVec3(D3DXVECTOR3 operand1, D3DXVECTOR3 operand2) {
			return {
				operand1.x + operand2.x,
				operand1.y + operand2.y,
				operand1.z + operand2.z
			};
		}
	};

	namespace Main
	{
		void LoadAim();
	}
}
