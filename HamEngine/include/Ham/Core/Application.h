#pragma once

#include "Ham/Core/Base.h"
#include "Ham/Core/Window.h"
#include "Ham/Core/Layer.h"
#include "Ham/Core/LayerStack.h"
#include "Ham/ImGui/ImGuiImpl.h"
#include "Ham/Scene/Scene.h"
#include "Ham/Renderer/FrameBuffer.h"
#include "Ham/Events/EventBase.h"

#include <sol/sol.hpp>

#include <thread>
#include <atomic>

namespace Ham {
class EditorLayer;

struct ApplicationCommandLineArgs {
  int Count = 0;
  char **Args = nullptr;

  const char *operator[](int index) const
  {
    HAM_CORE_ASSERT(index < Count);
    return Args[index];
  }
};

enum FullscreenMode {
  APPLICATION_WINDOWED,
  APPLICATION_FULLSCREEN,
  APPLICATION_FULLSCREEN_BORDERLESS
};

struct ApplicationSpecification {
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

class Application {
 public:
  Application(const ApplicationSpecification &specification);
  ~Application();

  void Run();
  void Init();
  void RenderThread();

  static Application &Get() { return *s_Instance; }

  void PushLayer(Layer *layer);
  void PushLayerUnique(Layer *layer);
  void PushOverlay(Layer *layer);
  void PushOverlayUnique(Layer *layer);
  void PopLayer(Layer *layer);
  void PopOverlay(Layer *layer);

  void EnableEditor();
  void DisableEditor();
  void ToggleEditor();
  bool IsEditorEnabled() { return m_EditorEnabled; }

  void TriggerCameraUpdate() { m_FramebufferResized = true; }

  Window &GetWindow() { return m_Window; }
  GLFWwindow *GetWindowHandle() { return m_Window.GetWindowHandle(); }
  Scene &GetScene() { return m_Scene; }
  ImGuiImpl &GetImGui() { return m_imgui; }
  float GetTime() { return m_Window.GetTime(); }
  const ApplicationSpecification &GetSpecification() const { return m_Specification; }
  FrameBuffer &GetObjectPickerFramebuffer() { return m_ObjectPickerFramebuffer; }

  void SetWindowed() { m_Window.SetWindowed(); }
  void SetFullscreen() { m_Window.SetFullscreen(); }
  void SetFullscreenBorderless() { m_Window.SetFullscreenBorderless(); }
  void SetVSync(bool enabled) { m_Window.SetVSync(enabled); }

  bool IsVSync() const { return m_Window.IsVSync(); }

  sol::state &GetLua() { return m_LuaState; }

  template <typename T>
  std::shared_ptr<Events::Subscriber<T>> Subscribe(const std::function<void(T &)> &func)
  {
    return m_SubscriberPool.Add(func);
  }

  template <typename T>
  void Unsubscribe(std::shared_ptr<Events::Subscriber<T>> subscriber)
  {
    m_SubscriberPool.Remove(subscriber);
  }

  template <typename T, typename... Args>
  void Emit(Args &&...args)
  {
    Events::Emit<T>(std::forward<Args>(args)...);
  }

 private:
  ApplicationSpecification &GetSpecificationMutable() { return m_Specification; }

 protected:
  Window m_Window;
  ImGuiImpl m_imgui;

 private:
  static Application *s_Instance;
  Scene m_Scene;
  std::shared_ptr<EditorLayer> m_EditorLayer;

  ApplicationSpecification m_Specification;
  LayerStack m_LayerStack;
  float m_LastFrameTime;

  std::thread m_RenderThread;

  bool m_EditorEnabled = false;

  std::atomic_bool m_FramebufferResized = false;

  friend ::Ham::Window;

  FrameBuffer m_ObjectPickerFramebuffer;

  sol::state m_LuaState;

  Events::SubscriberPool m_SubscriberPool;
};

// To be defined in CLIENT
Application *CreateApplication(ApplicationCommandLineArgs args);
}  // namespace Ham