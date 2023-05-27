#include "Ham/Core/Application.h"

#include "imgui.h"

#include "Ham/Core/Log.h"
#include "Ham/Util/TimeStep.h"
#include "Ham/Util/GLFWExtra.h"

namespace Ham
{
    Application::Application(const ApplicationSpecification &spec) : m_Specification(spec) {
        m_Specification.DefaultWidth = m_Specification.Width;
        m_Specification.DefaultHeight = m_Specification.Height;
    }
    Application::~Application() { Shutdown(); }

    void Application::Init()
    {
        m_Window.Init(this);
        m_imgui.Init(&m_Window);
    }

    void Application::Shutdown()
    {
        m_imgui.Shutdown();
        m_Window.Shutdown();
    }

    void Application::PushLayer(Layer *layer)
    {
        // HZ_PROFILE_FUNCTION();
        m_LayerStack.PushLayer(layer);
        if (m_Window.IsRunning())
            layer->OnAttach();
    }

    void Application::PushOverlay(Layer *layer)
    {
        // HZ_PROFILE_FUNCTION();
        m_LayerStack.PushOverlay(layer);
        if (m_Window.IsRunning())
            layer->OnAttach();
    }

    int Application::Run()
    {
        HAM_CORE_INFO("Ham Engine Version: 0.0.1");
        
        m_Window.SetIsRunning(true);
        for (Layer *layer : m_LayerStack)
            layer->OnAttach();

        m_Window.SetClearColor({0.f, 0.0f, 0.0f, 1.0f});

        // Main loop
        while (m_Window.IsRunning())
        {
            float time = GetTime();
            TimeStep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_Window.PollEvents();

            if (m_Window.ShouldClose())
                m_Window.SetIsRunning(false);

            {
                // HZ_PROFILE_SCOPE("LayerStack OnUpdate");
                for (Layer *layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            m_Window.SetContextCurrent();
            auto display = m_Window.GetFramebufferSize();
            glViewport(0, 0, display.x, display.y);
            m_Window.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_imgui.NewFrame();
            {
                // HZ_PROFILE_SCOPE("LayerStack OnUIRender");
                for (Layer *layer : m_LayerStack)
                    layer->OnUIRender(timestep);
            }
            m_imgui.Render();
            m_Window.Present();
        }

        return 0;
    }
} // namespace Ham
