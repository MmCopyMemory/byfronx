#pragma once

#include <Windows.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <string>

namespace Celestial
{
    class Framework
    {
    public:
		void CreateMainWindow(std::string WindowName, std::string Desc);
		bool CreateTab(const char* label, const char* icon, const bool selected, int space, const ImVec2& size_arg);

		void BeginTabChild(std::string Title);
		bool Checkmark(const char* label, const char* Desc, bool* v);
		void IntSlider(const char* label, int* v, int v_min, int v_max);
		void MainCombo(std::string Title, int* current_item, const char* items);
		void CreateSeperator(std::string Title);
		void EndTabChild();

		void EndWindow();
    };

}