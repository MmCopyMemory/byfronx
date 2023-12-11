
/*RYLEIGH Renderer Engine For Celestial | By Nicholas*/

#include <Windows.h>
#include <iostream>
#include <ImGui/imgui.h>
#include <string>

#include "../../GUIFramework/FontStore.h"

class Ryleigh
{
public:
	explicit Ryleigh();
	void DrawString(const char* Name, int X, int Y, const ImU32& color, bool outline, bool big) const;
	void DrawDistance(float Dist, int X, int Y, int W, int H, const ImU32& color, bool outline, int thickness) const;
	void DrawRect(float X, float Y, float Width, float Height, const ImU32& Color, int Thickness, int FilledBox) const;
	void DrawCornerRect(int X, int Y, int W, int H, const ImU32& color, int thickness, bool outline) const;
private:
	ImDrawList* Draw;

	void Output(std::string msg)
	{
		std::cout << "[RYLEIGH]: " << msg << std::endl;
	}
};