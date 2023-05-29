#pragma once

#include "Ham/Core/Base.h"
#include "Ham/Core/Window.h"
#include "Ham/Core/Layer.h"
#include "Ham/Core/LayerStack.h"
#include "Ham/ImGui/ImGuiImpl.h"
#include "Ham/Scene/Scene.h"

#include <thread>
#include <atomic>

namespace Ham
{

    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char **Args = nullptr;

        const char *operator[](int index) const
        {
            HAM_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };

    enum FullscreenMode
    {
        APPLICATION_WINDOWED,
        APPLICATION_FULLSCREEN,
        APPLICATION_FULLSCREEN_BORDERLESS
    };

    struct ApplicationSpecification
    {
        std::string Name = "Ham Engine Application";

        // settings
        int DefaultWidth = 1280;
        int DefaultHeight = 720;

        int Width = DefaultWidth;
        int Height = DefaultHeight;
        int XPos = -1;
        int YPos = -1;
        bool VSync = true;
        bool Maximized = false;
        int Display = 0;
        FullscreenMode Fullscreen = APPLICATION_WINDOWED;

        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification &specification);
        ~Application();

        int Run();
        void Init();
        void Shutdown();
        void RenderThread();

        static Application &Get() { return *s_Instance; }

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *layer);

        Window &GetWindow() { return m_Window; }
        GLFWwindow *GetWindowHandle() { return m_Window.GetWindowHandle(); }
        Scene &GetScene() { return m_Scene; }
        ImGuiImpl &GetImGui() { return m_imgui; }
        float GetTime() { return m_Window.GetTime(); }
        const ApplicationSpecification &GetSpecification() const { return m_Specification; }

        void SetWindowed() { m_Window.SetWindowed(); }
        void SetFullscreen() { m_Window.SetFullscreen(); }
        void SetFullscreenBorderless() { m_Window.SetFullscreenBorderless(); }
        void SetVSync(bool enabled) { m_Window.SetVSync(enabled); }

        bool IsVSync() const { return m_Window.IsVSync(); }

    protected:
        Window m_Window;
        ImGuiImpl m_imgui;

    private:
        static Application *s_Instance;
        Scene m_Scene;

        ApplicationSpecification m_Specification;
        LayerStack m_LayerStack;
        float m_LastFrameTime;

        std::thread m_RenderThread;

        std::atomic_bool m_FramebufferResized = false;

        friend ::Ham::Window;
    };

    // To be defined in CLIENT
    Application *CreateApplication(ApplicationCommandLineArgs args);
}