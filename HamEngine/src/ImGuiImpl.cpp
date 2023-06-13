#include "Ham/ImGui/ImGuiImpl.h"

#include "Ham/Core/Log.h"
#include "Ham/Core/Assert.h"
#include "Ham/Core/Window.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <ImGuizmo.h>

namespace Ham
{
    ImGuiImpl::ImGuiImpl() {}
    ImGuiImpl::~ImGuiImpl() { Shutdown(); }

    void ImGuiImpl::Init(Window *window)
    {
        HAM_CORE_ASSERT(window != nullptr, "Window is nullptr!");
        m_Window = window;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = "settings.ini";

        m_ImGuiSettingsHandler.UserData = m_Window->m_Application;
        m_ImGuiSettingsHandler.TypeName = "AppSettings";
        m_ImGuiSettingsHandler.TypeHash = ImHashStr(m_ImGuiSettingsHandler.TypeName);
        m_ImGuiSettingsHandler.ReadOpenFn = &Window::ReadOpenCallback;
        m_ImGuiSettingsHandler.ReadLineFn = &Window::ReadLineCallback;
        m_ImGuiSettingsHandler.WriteAllFn = &Window::WriteAllCallback;
        GImGui->SettingsHandlers.push_back(m_ImGuiSettingsHandler);
        ImGui::LoadIniSettingsFromDisk(io.IniFilename);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
                                                              // io.ConfigViewportsNoAutoMerge = true;
                                                              // io.ConfigViewportsNoTaskBarIcon = true;

#if defined(HAM_PLATFORM_LINUX) || defined(HAM_PLATFORM_MACOS)
        io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable; // FIXME: Viewports are not currently working on Linux/MacOS
#endif

        io.ConfigWindowsMoveFromTitleBarOnly = true;

        {
            ImGuiStyle *style = &ImGui::GetStyle();
            ImVec4 *colors = style->Colors;
            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsLight();

            colors[ImGuiCol_WindowBg].w = 200.0f / 255.0f;
        }

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetWindowHandle(), true);
        ImGui_ImplOpenGL3_Init(m_Window->GetGLSLVersion().c_str());

        ImGuizmo::AllowAxisFlip(true);
        auto &gizmoStyle = ImGuizmo::GetStyle();
        gizmoStyle.TranslationLineArrowSize = 8.0f;
        gizmoStyle.ScaleLineCircleSize = 8.0f;

        gizmoStyle.TranslationLineThickness = 4.0f;
        gizmoStyle.RotationLineThickness = 3.0f;
        gizmoStyle.RotationOuterLineThickness = 3.0f;
        gizmoStyle.ScaleLineThickness = 4.0f;

        gizmoStyle.HatchedAxisLineThickness = 4.0f;

        // gizmoStyle.CenterCircleSize = 10.0f;

        gizmoStyle.Colors[ImGuizmo::COLOR::SELECTION].w = 1.0f;
        gizmoStyle.Colors[ImGuizmo::COLOR::HATCHED_AXIS_LINES].w = 0.5f;
    }

    void ImGuiImpl::NewFrame()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiImpl::SetupPreRender()
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            ImGuizmo::SetRect((float)m_Window->GetXPos(), (float)m_Window->GetYPos(), (float)m_Window->GetWidth(), (float)m_Window->GetHeight());
        else
            ImGuizmo::SetRect(0.0f, 0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight());
    }

    void ImGuiImpl::Render()
    {
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiImpl::UpdateWindows()
    {
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        auto &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            // GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            // glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiImpl::Shutdown()
    {
        if (m_IsShutDown)
            return;
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        m_IsShutDown = true;
    }

} // namespace Ham
