#pragma once

#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace Ham
{
    class Window;
    class ImGuiImpl
    {
    public:
        ImGuiImpl();
        ~ImGuiImpl();

        void Init(Window *window);
        void NewFrame();
        void Render();
        void UpdateWindows();
        void Shutdown();

        bool WantsCaptureMouse() { return ImGui::GetIO().WantCaptureMouse || ImGuizmo::IsUsingAny(); }
        bool WantsCaptureKeyboard() { return ImGui::GetIO().WantCaptureKeyboard; }

    private:
        Window *m_Window;
        bool m_IsShutDown = false;
        ImGuiSettingsHandler m_ImGuiSettingsHandler;
    };
} // namespace Ham
