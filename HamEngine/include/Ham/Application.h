#pragma once

#include "Ham/Layer.h"
#include "Ham/Window.h"
#include "Ham/ImGui/ImGuiImpl.h"
#include "Ham/Assert.h"
#include "Ham/LayerStack.h"

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

    struct ApplicationSpecification
    {
        std::string Name = "Ham Engine Application";
        int Width = 1280;
        int Height = 720;
        std::string WorkingDirectory;
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

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *layer);

        Window &GetWindow() { return m_Window; }
        ImGuiImpl &GetImGui() { return m_imgui; }
        float GetTime() { return m_Window.GetTime(); }
        const ApplicationSpecification &GetSpecification() const { return m_Specification; }

    protected:
        Window m_Window;
        ImGuiImpl m_imgui;

    private:
        ApplicationSpecification m_Specification;
        LayerStack m_LayerStack;
        float m_LastFrameTime;
    };

    // To be defined in CLIENT
    Application *CreateApplication(ApplicationCommandLineArgs args);
}