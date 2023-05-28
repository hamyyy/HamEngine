#include "Ham/Core/Application.h"

#include <imgui.h>

#include "Ham/Core/Log.h"
#include "Ham/Util/TimeStep.h"
#include "Ham/Util/GLFWExtra.h"

namespace Ham
{
    Application *Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification &spec) : m_Specification(spec)
    {
        HAM_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Specification.DefaultWidth = m_Specification.Width;
        m_Specification.DefaultHeight = m_Specification.Height;
    }
    Application::~Application() { Shutdown(); }

    void Application::Init()
    {
        HAM_PROFILE_SCOPE("Application Init");
        m_Window.Init(this);
    }

    void Application::Shutdown()
    {
        m_Window.Shutdown();
        s_Instance = nullptr;
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

    void Application::RenderThread()
    {
        // HAM_PROFILE_THREAD("Render Thread");
        m_Window.SetContextCurrent();
        glfwSetWindowUserPointer(m_Window.GetWindowHandle(), (void *)this);
        glfwSetFramebufferSizeCallback(m_Window.GetWindowHandle(),
                                       [](GLFWwindow *window, int width, int height)
                                       {
                                           auto app = (::Ham::Application *)glfwGetWindowUserPointer(window);
                                           app->m_FramebufferResized = true;
                                       });
        auto display = m_Window.GetFramebufferSize();
        glViewport(0, 0, display.x, display.y);
        m_Window.SetClearColor({0.44f, 0.51f, 0.43f, 1.0f});

        // {0.95f, 0.88f, 0.77f, 1.0f} // light
        // {0.79f, 0.72f, 0.61f, 1.0f} // darker but still light
        // {0.84f, 0.16f, 0.22f, 1.0f} // red
        // {0.44f, 0.51f, 0.43f, 1.0f} // greenish grey
        // {0.2f, 0.16f, 0.13f, 1.0f} // brown (dark)

        m_imgui.Init(&m_Window);

        for (Layer *layer : m_LayerStack)
            layer->OnAttach();

        SetVSync(GetSpecification().VSync);
        m_Window.SetIsRunning(true);
        while (m_Window.IsRunning())
        {
            // HAM_PROFILE_FRAME("Render Frame");

            {
                // HAM_PROFILE_SCOPE("Render Poll Events");
                m_Window.PollEvents();
            }

            if (m_Window.ShouldClose())
                m_Window.SetIsRunning(false);

            if (glfwGetCurrentContext() != m_Window.GetWindowHandle())
                m_Window.SetContextCurrent();

            if (m_FramebufferResized)
            {
                // HAM_PROFILE_SCOPE("Framebuffer Resized");
                auto display = m_Window.GetFramebufferSize();
                glViewport(0, 0, display.x, display.y);
                m_FramebufferResized = false;
            }

            float time = GetTime();
            TimeStep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_Window.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            {
                // HAM_PROFILE_SCOPE("LayerStack OnUpdate");
                for (Layer *layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            m_imgui.NewFrame();
            {
                // HAM_PROFILE_SCOPE("LayerStack OnUIRender");
                for (Layer *layer : m_LayerStack)
                    layer->OnUIRender(timestep);
            }

            {
                // HAM_PROFILE_SCOPE("ImGui Render");
                m_imgui.Render();
            }
            {
                // HAM_PROFILE_SCOPE("ImGui Update Windows");
                m_imgui.UpdateWindows();
            }

            {
                // HAM_PROFILE_SCOPE("Present");
                m_Window.Present();
            }
        }

        m_imgui.Shutdown();
    }

    int Application::Run()
    {
        // HAM_PROFILE_THREAD("Main Thread");
        HAM_CORE_INFO("Ham Engine Version: 0.0.1");

        glfwMakeContextCurrent(nullptr);
        m_RenderThread = std::thread(&Application::RenderThread, this);

        while (!m_Window.IsRunning())
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        m_Window.ApplySavedSettings();
        // Main loop
        while (m_Window.IsRunning())
        {
            HAM_PROFILE_FRAME("Main Frame");

            {
                HAM_PROFILE_SCOPE("Main Poll Events");
                m_Window.PollEvents();
            }

            if (m_Window.ShouldClose())
                m_Window.SetIsRunning(false);
        }

        if (m_RenderThread.joinable())
            m_RenderThread.join();

        return 0;
    }
} // namespace Ham
