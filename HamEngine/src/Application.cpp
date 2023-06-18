#include "Ham/Core/Application.h"

#include <imgui.h>

#include "Ham/Core/Log.h"
#include "Ham/Util/TimeStep.h"
#include "Ham/Util/GLFWExtra.h"
#include "Ham/Util/UUID.h"
#include "Ham/Input/Input.h"
#include "Ham/Util/Random.h"

#include "Ham/Scene/Scene.h"
#include "Ham/Renderer/ShaderLibrary.h"
#include "Ham/Renderer/Shader.h"
#include "Ham/Renderer/FrameBuffer.h"
#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Component.h"
#include "Ham/Script/CameraController.h"
#include "Ham/Scene/Systems.h"

namespace Ham
{
    Application *Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification &spec) : m_Specification(spec), m_Scene()
    {
        HAM_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Specification.DefaultWidth = m_Specification.Width;
        m_Specification.DefaultHeight = m_Specification.Height;
    }
    Application::~Application() { Shutdown(); }

    void Application::Init()
    {
        // HAM_PROFILE_SCOPE("Application Init");
        HAM_CORE_INFO("Ham Engine Version: 0.0.1");
        m_Window.Init(this);
        Random::Init();
        ShaderLibrary::Init();
        Input::Init();

        auto cameraEntity = m_Scene.CreateEntity("Camera");
        cameraEntity.AddComponent<Component::Camera>();
        cameraEntity.GetComponent<Component::Transform>() = Component::Transform(glm::inverse(glm::lookAt(glm::vec3(3, 3, 3), glm::zero<glm::vec3>(), glm::up()))); // glm::inverse(glm::camera()) * glm::translate(glm::mat4(1.0f), glm::forward() * 10.0f)

        auto &scriptList = cameraEntity.AddComponent<Component::NativeScriptList>();
        scriptList.AddScript<CameraController>("CameraController");

        auto &projection = cameraEntity.GetComponent<Component::Camera>().Projection;
        auto view = glm::inverse(cameraEntity.GetComponent<Component::Transform>().ToMatrix());
        HAM_CORE_WARN("Camera Aspect Ratio: {0}", GetWindow().GetAspectRatio());
        HAM_CORE_WARN("Camera Transform (View): {0}", glm::to_string(view));
        HAM_CORE_WARN("Camera Projection: {0}", glm::to_string(projection));

        HAM_CORE_INFO("Here are some random UUIDs:\n\t{0}\n\t{1}\n\t{2}", UUIDGenerator::Create(), UUIDGenerator::Create(), UUIDGenerator::Create());

        HAM_CORE_INFO("Up vector is: {0}", glm::to_string(glm::up()));
        HAM_CORE_INFO("Right vector is: {0}", glm::to_string(glm::right()));
        HAM_CORE_INFO("Forward vector is: {0}", glm::to_string(glm::forward()));

        {
            //  add dummy entities

            // auto cube = m_Scene.CreateEntity("Cube");
            // auto monkey = m_Scene.CreateEntity("Monkey");
            // auto sphere = m_Scene.CreateEntity("Sphere");
            // auto plane = m_Scene.CreateEntity("Plane");

            // plane.SetParent(cube);
            // sphere.SetParent(cube);

            // monkey.SetParent(plane);
        }
    }

    void Application::Shutdown()
    {
        Systems::DetachNativeScripts(m_Scene);
        m_Window.Shutdown();
        s_Instance = nullptr;
        // FileWatcher::Shutdown();
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

        {
            // pre-loop
            for (Layer *layer : m_LayerStack)
                layer->OnAttach();

            Systems::AttachNativeScripts(m_Scene);

            SetVSync(GetSpecification().VSync);
        }

        m_Window.SetIsRunning(true);
        m_FramebufferResized = true;

        FrameBufferSpecification frameBufferSpec;
        frameBufferSpec.Width = display.x;
        frameBufferSpec.Height = display.y;
        frameBufferSpec.Samples = 1;
        frameBufferSpec.ClearColor = {1.0f, 1.0f, 1.0f, 1.0f};
        frameBufferSpec.ColorAttachments = {TextureFormat::COLOR_RGBA8};
        frameBufferSpec.MinFilter = TextureFilter::NEAREST;
        frameBufferSpec.MagFilter = TextureFilter::NEAREST;
        FrameBuffer objectPickerFramebuffer(frameBufferSpec);

        while (m_Window.IsRunning())
        {
            // HAM_PROFILE_FRAME("Render Frame");

            {
                // HAM_PROFILE_SCOPE("Render Poll Events");
                m_Window.PollEvents();
                Input::BeginFrame();
            }

            if (m_Window.ShouldClose())
            {
                m_Window.SetIsRunning(false);
                break;
            }

            if (glfwGetCurrentContext() != m_Window.GetWindowHandle())
                m_Window.SetContextCurrent();

            if (m_FramebufferResized)
            {
                // HAM_PROFILE_SCOPE("Framebuffer Resized");
                display = m_Window.GetFramebufferSize();
                glViewport(0, 0, display.x, display.y);
                m_FramebufferResized = false;
                m_Scene.GetActiveCamera().GetComponent<Component::Camera>().Update((float)display.x, (float)display.y);

                objectPickerFramebuffer.Resize(display.x, display.y);
            }

            float time = GetTime();
            TimeStep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            {
                // HAM_PROFILE_SCOPE("LayerStack OnUpdate");
                Systems::UpdateNativeScripts(m_Scene, timestep);
                for (Layer *layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            m_imgui.NewFrame();
            {
                // HAM_PROFILE_SCOPE("LayerStack OnUIRender");
                m_imgui.SetupPreRender();
                Systems::UpdateNativeScriptsUI(m_Scene, timestep);
                for (Layer *layer : m_LayerStack)
                {
                    m_imgui.SetupPreRender();
                    layer->OnUIRender(timestep);
                }
            }

            {
                m_Window.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                Systems::RenderScene(*this, m_Scene, timestep);

                objectPickerFramebuffer.Bind();
                objectPickerFramebuffer.Clear(AttachmentType::COLOR | AttachmentType::DEPTH | AttachmentType::STENCIL);
                Systems::RenderObjectPickerFrame(*this, m_Scene, timestep);
                objectPickerFramebuffer.Unbind();

                ImGui::Begin("Object Picker");
                auto image = objectPickerFramebuffer.GetColorAttachmentID(0);
                auto windowWidth = ImGui::GetWindowSize().x;
                auto windowHeight = windowWidth * ((float)display.y / (float)display.x);
                ImGui::Image((void *)(intptr_t)image, {windowWidth, windowHeight}, ImVec2(0, 1), ImVec2(1, 0));

                std::vector<unsigned char> value = objectPickerFramebuffer.ReadPixels((int)Input::GetMousePosition().x, display.y - (int)Input::GetMousePosition().y, 1, 1);
                uint32_t r = value[0];
                uint32_t g = value[1];
                uint32_t b = value[2];
                uint32_t a = value[3];

                uint32_t id = r + (g << 8) + (b << 16); // + (a << 24);

                ImGui::Text("R: %d, G: %d, B: %d, A: %d", r, g, b, a);
                ImGui::Text("ID: %d", id);
                ImGui::End();

                auto view = m_Scene.m_Registry.view<Component::Mesh>();
                if (id == -1)
                    m_Scene.ClearHoveredEntity();
                else
                    m_Scene.SetHoveredEntity({view[id], &m_Scene});

                if (Input::IsMouseButtonUpThisFrame(MouseButton::LEFT))
                {
                    if (id == -1)
                        m_Scene.ClearSelectedEntity();
                    else
                        m_Scene.SetSelectedEntity({view[id], &m_Scene});
                }
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

            for (auto shader : Shader::s_Shaders)
            {
                shader->PerformReload();
            }

            Input::EndFrame();
        }

        m_imgui.Shutdown();
        m_Scene.Shutdown();
    }

    int Application::Run()
    {
        HAM_PROFILE_THREAD("Main Thread");

        glfwMakeContextCurrent(nullptr);
        m_RenderThread = std::thread(&Application::RenderThread, this);

        while (!m_Window.IsRunning())
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        m_Window.ApplySavedSettings();
        // Main loop
        while (m_Window.IsRunning())
        {
            // HAM_PROFILE_FRAME("Main Frame");

            {
                // HAM_PROFILE_SCOPE("Main Poll Events");
                m_Window.PollEvents();
                Input::Update();
            }

            if (m_Window.ShouldClose())
            {
                m_Window.SetIsRunning(false);
                break;
            }
        }

        if (m_RenderThread.joinable())
            m_RenderThread.join();

        return 0;
    }
} // namespace Ham
