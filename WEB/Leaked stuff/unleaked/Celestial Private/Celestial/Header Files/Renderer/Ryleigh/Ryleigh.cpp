#include "Ryleigh.h"

Ryleigh::Ryleigh()
{
	this->Output("Ready!");
}

void Ryleigh::DrawString(const char* Name, int X, int Y, const ImU32& color, bool outline, bool big) const
{
	const ImVec2 StringSize = ImGui::CalcTextSize(Name, NULL, true);
	
	if (outline)
	{
		if (!big)
		{
			ImGui::PushFont(Celestial::Fonts::RenderFont);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 + 0, Y - 1), ImColor(0, 0, 0), Name); //up
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 + 0, Y + 1), ImColor(0, 0, 0), Name); //down
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 + 1, Y + 0), ImColor(0, 0, 0), Name); //right
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 - 1, Y + 0), ImColor(0, 0, 0), Name); //left

			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 + 1, Y + 1), ImColor(0, 0, 0), Name); //down right
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 - 1, Y + 1), ImColor(0, 0, 0), Name); //down right

			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 + 1, Y - 1), ImColor(0, 0, 0), Name); //up right
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2 - 1, Y - 1), ImColor(0, 0, 0), Name); //up left
			ImGui::PopFont();
		}
	}

	if (big)
	{
		ImGui::PushFont(Celestial::Fonts::BigRenderFont);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2, Y), color, Name);
		ImGui::PopFont();
	}

	if (!big)
	{
		ImGui::PushFont(Celestial::Fonts::RenderFont);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(X - StringSize.x / 2, Y), color, Name);
		ImGui::PopFont();
	}
}

void Ryleigh::DrawDistance(float Dist, int X, int Y, int W, int H, const ImU32& color, bool outline, int thickness) const
{
	float lineW = (W / 5);
	float lineH = (H / 6);

	//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W + 10, Y - 16), ImColor(0, 0, 0), 2);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W + 10, Y - 16), ImGui::GetColorU32(color), thickness);

	//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W + 30, Y - 15), ImVec2(X + W + 9, Y - 15), ImColor(0, 0, 0), 2);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W + 30, Y - 15), ImVec2(X + W + 9, Y - 15), ImGui::GetColorU32(color), thickness);

	this->DrawString(std::to_string((int)Dist).c_str(), X + W + 20, Y - 26, ImGui::GetColorU32(color), false, false);
}

void Ryleigh::DrawRect(float x, float y, float width, float height, const ImU32& Color, int Thickness, int FilledBox) const
{
	if (!FilledBox)
	{
		ImGui::GetBackgroundDrawList()->AddLine({ x + width, y - height - (height) }, { x + width, y + height + (height / 2.f) }, ImGui::GetColorU32(Color), Thickness);
		ImGui::GetBackgroundDrawList()->AddLine({ x - width, y - height - (height) }, { x - width, y + height + (height / 2.f) }, ImGui::GetColorU32(Color), Thickness);
		ImGui::GetBackgroundDrawList()->AddLine({ x + width, y + height + (height / 2.f) }, { x - width, y + height + (height / 2.f) }, ImGui::GetColorU32(Color), Thickness);
		ImGui::GetBackgroundDrawList()->AddLine({ x + width, y - height - (height) }, { x - width, y - height - (height) }, ImGui::GetColorU32(Color), Thickness);
	}

	if (FilledBox) {

		int r = (Color >> 16) & 0xFF;
		int g = (Color >> 8) & 0xFF;
		int b = (Color) & 0xFF;

		ImGui::GetBackgroundDrawList()->AddRectFilled({ x - width, y - height - (height) }, { x + width, y + height + (height / 2.f) }, ImGui::GetColorU32(Color), 0, 0);
	}
}


void Ryleigh::DrawCornerRect(int X, int Y, int W, int H, const ImU32& color, int thickness, bool outline) const
{
	float lineW = (W / 5);
	float lineH = (H / 6);

	if (outline)
	{
		//black outlines
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImColor(0, 0, 0), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImColor(0, 0, 0), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImColor(0, 0, 0), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImColor(0, 0, 0), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImColor(0, 0, 0), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImColor(0, 0, 0), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImColor(0, 0, 0), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImColor(0, 0, 0), 2);
	}

	//corners
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}