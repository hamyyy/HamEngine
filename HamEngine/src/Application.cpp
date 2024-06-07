#include "Ham/Core/Application.h"

#include "Ham/Core/Log.h"
#include "Ham/Editor/EditorLayer.h"
#include "Ham/Input/Input.h"
#include "Ham/Renderer/Shader.h"
#include "Ham/Renderer/ShaderLibrary.h"
#include "Ham/Scene/Component.h"
#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Systems.h"
#include "Ham/Script/CameraController.h"
#include "Ham/Util/GLFWExtra.h"
#include "Ham/Util/Random.h"
#include "Ham/Util/TimeStep.h"
#include "Ham/Util/UUID.h"
#include "Ham/Util/Watcher.h"
#include "Ham/Events/Event.h"

#include <imgui.h>

#include <ranges>

namespace Ham {
Application *Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification &spec) : m_Specification(spec), m_Scene()
{
  HAM_CORE_ASSERT(!s_Instance, "Application already exists!");
  s_Instance = this;
  m_Specification.DefaultWidth = m_Specification.Width;
  m_Specification.DefaultHeight = m_Specification.Height;
}

Application::~Application()
{
  // FIXME: App doesnt like this for some reason
  // Probably because shaders arent being destroyed properly (TODO: fix shader destruction).
  // Currently this is causing a memory leak, but its okay since its a single pointer and system will free it on exit.
  // Once this is fixed, change FileWatcher::s_FileListener to a value (non-pointer) or unique_ptr
  // and uncomment the following line
  // FileWatcher::Shutdown();

  s_Instance = nullptr;
}

void Application::Init()
{
  HAM_PROFILE_SCOPE_NAMED("Application Init");
  HAM_CORE_INFO("Ham Engine Version: 0.0.1");
  FileWatcher::Init();
  m_Window.Init(this);
  Random::Init();
  ShaderLibrary::Init();
  Input::Init();

  m_LuaState.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);

  try {
    m_LuaState.safe_script_file(SCRIPTS_PATH "add.lua");
  }
  catch (const sol::error &err) {
    HAM_CORE_ERROR("Error loading script.lua: {0}", err.what());
  }

  sol::protected_function func = m_LuaState["add"];
  if (func) {
    auto result = func(10.0f, 133.0f);
    if (result.valid()) {
      HAM_CORE_INFO("Result: {}", result.get<float>());
    }
    else {
      sol::error err = result;
      HAM_CORE_ERROR("Unable to run lua script:\n{}", err.what());
    }
  }

  m_EditorLayer = std::make_shared<EditorLayer>(this);
  // PushLayer(m_EditorLayer.get());

  auto display = m_Window.GetFramebufferSize();
  FrameBufferSpecification frameBufferSpec;
  frameBufferSpec.Width = display.x;
  frameBufferSpec.Height = display.y;
  frameBufferSpec.Samples = 1;
  frameBufferSpec.ClearColor = {1.0f, 1.0f, 1.0f, 1.0f};
  frameBufferSpec.ColorAttachments = {TextureFormat::COLOR_RGBA8};
  frameBufferSpec.MinFilter = TextureFilter::NEAREST;
  frameBufferSpec.MagFilter = TextureFilter::NEAREST;
  m_ObjectPickerFramebuffer.Init(frameBufferSpec);

  auto cameraEntity = m_Scene.CreateEntity("Camera");
  cameraEntity.AddComponent<Component::Camera>();
  cameraEntity.GetComponent<Component::Transform>() = Component::Transform(math::inverse(math::lookAt(math::vec3(3.0f, 3.0f, 3.0f), math::zero<math::vec3>())));  // Component::Transform(math::inverse(math::camera()) * math::translate((math::camera() * math::vec4(math::backward(), 0.0f)).xyz * 10.0f));

  auto &scriptList = cameraEntity.AddComponent<Component::NativeScriptList>();
  scriptList.AddScript<CameraController>("CameraController");

  // auto &projection = cameraEntity.GetComponent<Component::Camera>().Projection;
  // auto view = math::inverse(cameraEntity.GetComponent<Component::Transform>().ToMatrix());

  // HAM_CORE_WARN("Camera Aspect Ratio: {0}", GetWindow().GetAspectRatio());
  // HAM_CORE_WARN("Camera Transform (View): {0}", math::to_string(view));
  // HAM_CORE_WARN("Camera Projection: {0}", math::to_string(projection));

  // HAM_CORE_INFO("Here are some random UUIDs:\n\t{0}\n\t{1}\n\t{2}", UUIDGenerator::Create(), UUIDGenerator::Create(), UUIDGenerator::Create());

  // HAM_CORE_INFO("Up vector is: {0}", math::to_string(math::up()));
  // HAM_CORE_INFO("Right vector is: {0}", math::to_string(math::right()));
  // HAM_CORE_INFO("Forward vector is: {0}", math::to_string(math::forward()));
}

void Application::PushLayer(Layer *layer)
{
  HAM_PROFILE_SCOPE();
  m_LayerStack.PushLayer(layer);
  if (m_Window.IsRunning())
    layer->OnAttach();
}

void Application::PushLayerUnique(Layer *layer)
{
  HAM_PROFILE_SCOPE();
  m_LayerStack.PushLayerUnique(layer);
  if (m_Window.IsRunning())
    layer->OnAttach();
}

void Application::PushOverlay(Layer *layer)
{
  HAM_PROFILE_SCOPE();
  m_LayerStack.PushOverlay(layer);
  if (m_Window.IsRunning())
    layer->OnAttach();
}

void Application::PushOverlayUnique(Layer *layer)
{
  HAM_PROFILE_SCOPE();
  m_LayerStack.PushOverlayUnique(layer);
  if (m_Window.IsRunning())
    layer->OnAttach();
}

void Application::PopLayer(Layer *layer)
{
  HAM_PROFILE_SCOPE();
  m_LayerStack.PopLayer(layer);
  if (m_Window.IsRunning())
    layer->OnDetach();
}

void Application::PopOverlay(Layer *layer)
{
  HAM_PROFILE_SCOPE();
  m_LayerStack.PopOverlay(layer);
  if (m_Window.IsRunning())
    layer->OnDetach();
}

void Application::EnableEditor()
{
  PushOverlayUnique(m_EditorLayer.get());
  m_EditorEnabled = true;
}

void Application::DisableEditor()
{
  PopOverlay(m_EditorLayer.get());
  m_EditorEnabled = false;
}

void Application::ToggleEditor()
{
  if (m_EditorEnabled) {
    DisableEditor();
  }
  else {
    EnableEditor();
  }
}

void Application::RenderThread()
{
  HAM_PROFILE_THREAD("Render Thread");
  m_Window.SetContextCurrent();
  glfwSetWindowUserPointer(m_Window.GetWindowHandle(), (void *)this);
  glfwSetFramebufferSizeCallback(m_Window.GetWindowHandle(),
                                 [](GLFWwindow *window, int width, int height) {
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

  while (m_Window.IsRunning()) {
    {
      HAM_PROFILE_SCOPE_NAMED("Render Poll Events");
      m_Window.PollEvents();
      Input::BeginFrame();
    }

    if (m_Window.ShouldClose()) {
      m_Window.SetIsRunning(false);
      break;
    }

    if (Input::IsKeyDownThisFrame(KeyCode::F3)) {
      ToggleEditor();
    }

    if (glfwGetCurrentContext() != m_Window.GetWindowHandle())
      m_Window.SetContextCurrent();

    if (m_FramebufferResized) {
      HAM_PROFILE_SCOPE_NAMED("Framebuffer Resized");
      display = m_Window.GetFramebufferSize();
      glViewport(0, 0, display.x, display.y);
      m_FramebufferResized = false;
      if (m_Scene.GetActiveCamera()) {
        m_Scene.GetActiveCamera().GetComponent<Component::Camera>().Update((float)display.x, (float)display.y);
      }
      m_ObjectPickerFramebuffer.Resize(display.x, display.y);
    }

    float time = GetTime();
    TimeStep timestep = time - m_LastFrameTime;
    m_LastFrameTime = time;

    {
      HAM_PROFILE_SCOPE_NAMED("LayerStack OnUpdate");
      Systems::UpdateNativeScripts(m_Scene, timestep);
      for (Layer *layer : m_LayerStack)
        layer->OnUpdate(timestep);
    }

    m_imgui.NewFrame();
    {
      HAM_PROFILE_SCOPE_NAMED("LayerStack OnUIRender");
      m_imgui.SetupPreRender();
      Systems::UpdateNativeScriptsUI(m_Scene, timestep);
      for (Layer *layer : m_LayerStack) {
        m_imgui.SetupPreRender();
        layer->OnUIRender(timestep);
      }
    }

    {
      m_Window.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      Systems::RenderScene(*this, m_Scene, timestep);
      Systems::HandleObjectPicker(*this, m_Scene, m_ObjectPickerFramebuffer, timestep);
    }

    {
      HAM_PROFILE_SCOPE_NAMED("ImGui Render");
      m_imgui.Render();
    }

    {
      HAM_PROFILE_SCOPE_NAMED("ImGui Update Windows");
      m_imgui.UpdateWindows();
    }

    {
      HAM_PROFILE_SCOPE_NAMED("Present");
      m_Window.Present();
    }

    for (auto shader : Shader::s_Shaders) {
      shader->PerformReload();
    }

    Input::EndFrame();
    HAM_PROFILE_FRAME("Render Frame");
  }

  // make sure the main thread knows we're done
  m_Window.SetIsRunning(false);

  {
    // post-loop
    Systems::DetachNativeScripts(m_Scene);

    for (int i = (int)m_LayerStack.GetSize() - 1; i >= 0; i--) {
      HAM_CORE_INFO("Detaching layer {0}", i);
      Layer *layer = m_LayerStack[i];
      layer->OnDetach();  // manually call OnDetach for all layers since is_running is false
      m_LayerStack.PopLayer(layer);
    }
  }
}

void Application::Run()
{
  HAM_PROFILE_THREAD("Main Thread");

  glfwMakeContextCurrent(nullptr);
  m_RenderThread = std::thread(&Application::RenderThread, this);

  auto start = std::chrono::high_resolution_clock::now();
  while (!m_Window.IsRunning()) {
    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() > 5) {
      throw std::runtime_error("Failed to start render thread. Timeout reached.");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  m_Window.ApplySavedSettings();
  // Main loop
  while (m_Window.IsRunning()) {
    {
      HAM_PROFILE_SCOPE_NAMED("Main Poll Events");
      m_Window.PollEvents();
      // Input::Update();
      FileWatcher::Update();

      std::shared_ptr<Events::Event> event;
      do {
        try {
          event = Events::PollEvent();
        }
        catch ([[maybe_unused]] const std::runtime_error &e) {
          break;
        }

        Events::EventHandler handler(event);
        handler.Dispatch<Events::WindowClosed>([this](Events::WindowClosed &e) {
          m_Window.SetIsRunning(false);
          return true;
        });
        handler.Dispatch<Events::WindowResized>([this](Events::WindowResized &e) {
          m_FramebufferResized = true;
          return true;
        });
        handler.Dispatch<Events::KeyPressed>([](Events::KeyPressed &e) {
          return true;
        });
        handler.Dispatch<Events::KeyReleased>([](Events::KeyReleased &e) {
          return true;
        });
        handler.Dispatch<Events::KeyTyped>([](Events::KeyTyped &e) {
          // HAM_CORE_INFO("Key Typed: {0} {1}", (char)e.GetASCII(), e.GetASCII());
          return true;
        });
        handler.Dispatch<Events::MouseMoved>([](Events::MouseMoved &e) {
          return true;
        });
        handler.Dispatch<Events::MouseButtonPressed>([](Events::MouseButtonPressed &e) {
          return true;
        });
        handler.Dispatch<Events::MouseButtonReleased>([](Events::MouseButtonReleased &e) {
          return true;
        });
        handler.Dispatch<Events::MouseScrolled>([this](Events::MouseScrolled &e) {
          Input::OnScroll(this, e.GetXOffset(), e.GetYOffset());
          return true;
        });

        for (Layer *layer : m_LayerStack) {
          if (event->Handled)
            break;
          if (layer->OnEvent(event))
            break;
        }

        if (!event->Handled) {
          HAM_CORE_ERROR("Unhandled Event: {0}", event->GetName());
          event->Handled = true;
        }

      } while (event);

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (m_Window.ShouldClose()) {
      m_Window.SetIsRunning(false);
      break;
    }

    HAM_PROFILE_FRAME("Main Frame");
  }

  m_Window.SetIsRunning(false);

  if (m_RenderThread.joinable())
    m_RenderThread.join();
}
}  // namespace Ham
