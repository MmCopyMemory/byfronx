#include <Windows.h>
#include <vector>
#include <string>

#include <D3DX10.h>
#include <D3DX10math.h>

namespace Celestial
{
	class World 
	{
	public:

		/*World Functions*/
		std::vector<std::uintptr_t> GetChildren(const std::uintptr_t Instance);
		std::vector<std::uintptr_t> GetAttributes(const std::uintptr_t Instance, std::string Attribute);
		std::string GetClassType(const std::uintptr_t Name);
		std::uintptr_t GetService(const std::uintptr_t Instance, std::string Class);
		std::uintptr_t FindFirstChild(const std::uintptr_t Instance, std::string Child);
		std::uintptr_t FindFirstChildFromVector(std::vector<std::uintptr_t> Instance, std::string Child);
		std::uintptr_t GetWorkspace(const std::uintptr_t Instance);
		std::uintptr_t GetPlayers(const std::uintptr_t Instance);
		float* GetProjection(const std::uintptr_t VisualEngine);
		D3DXVECTOR2 GetRobloxDimensions(const std::uintptr_t VisualEngine);
		bool WorldToScreen(D3DXVECTOR3 Position, D3DXVECTOR2& ToScreen);
		D3DXVECTOR3 GetCameraPosition(const std::uintptr_t Instance);
		std::uintptr_t GetCreatorId(const std::uintptr_t Instance);
		bool IsLoaded(const std::uintptr_t Instance);
		float GetVector3Magnitude(D3DXVECTOR3 vector);
		float GetVector2Magnitude(D3DXVECTOR2 vector);

		/*Player Functions*/
		std::uintptr_t GetLocalPlayer(const std::uintptr_t Players);
		std::uintptr_t GetCharacter(const std::uintptr_t LocalPlayer);
		std::uintptr_t GetTeam(const std::uintptr_t Player);
		std::uintptr_t GetBrickColorID(const std::uintptr_t Team);
		std::string GetName(const std::uintptr_t Player);
		std::uintptr_t GetPlayerHumanoid(const std::uintptr_t Player);
		float GetPlayerHealth(const std::uintptr_t Humanoid);
		float GetMaxPlayerHealth(const std::uintptr_t Humanoid);
		D3DXVECTOR3 GetPartPosition(const std::uintptr_t Part);
		D3DXVECTOR3 GetPrimitiveVelocity(const std::uintptr_t Part);
		D3DXVECTOR3 GetLookVector(const std::uintptr_t Instance);
		D3DXVECTOR3 GetCFrame(const std::uintptr_t Camera);
		std::uintptr_t GetInstanceParent(const std::uintptr_t Instance);
		bool IsDescendantOf(const std::uintptr_t Instance, const std::uintptr_t Old);
		std::uintptr_t GetPartAdornee(const std::uintptr_t Part);
		std::uintptr_t IsLocalCharacter();
		std::uintptr_t GetPlayerFromCharacter(const std::uintptr_t Character);
		std::int32_t GetRigType(const std::uintptr_t Humanoid);
		std::uintptr_t GetPrimaryPart(const std::uintptr_t Part);
		
		bool IsPlayerOnTeam(std::uintptr_t Player);
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
		float matrixprojection[16];
	};
}
