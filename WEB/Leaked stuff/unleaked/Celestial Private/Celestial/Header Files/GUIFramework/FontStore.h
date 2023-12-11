#pragma once

#include <Windows.h>
#include <ImGui/imgui.h>

namespace Celestial
{
    struct Fonts
    {
        static ImFont* TitleFont;
        static ImFont* IconFont;
        static ImFont* SmallTitle;
        static ImFont* SmallPoppins;
        static ImFont* CheckFont;
        static ImFont* RenderFont;
        static ImFont* BigRenderFont;
    };

}