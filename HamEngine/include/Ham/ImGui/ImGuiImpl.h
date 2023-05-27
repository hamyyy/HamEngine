#pragma once


#include <imgui_internal.h>

namespace Ham
{
    class Window;
    class ImGuiImpl
    {
    public:
        ImGuiImpl();
        ~ImGuiImpl();

        void Init(Window* window);
        void NewFrame();
        void Render();
        void Shutdown();

    private:
        Window* m_Window;
        bool m_IsShutDown = false;
        ImGuiSettingsHandler m_ImGuiSettingsHandler;
    };
} // namespace Ham
