#pragma once

#include "Ham/ImGui/ImGuiImpl.h"
#include "Ham/Core/Math.h"

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include <GLFW/glfw3.h>  // Will drag system OpenGL headers
#include <glad/gl.h>     // Initialize with gladLoadGL()
#include <imgui_internal.h>

#include <string>
#include <atomic>

namespace Ham {
struct ApplicationSpecification;
class Application;

class Window {
 public:
  Window();
  Window(const Window &other) = default;
  ~Window();

  void Init(Application *app);
  void Update();

  std::atomic_bool &IsRunning() { return m_IsRunning; }
  void SetIsRunning(bool isRunning) { m_IsRunning = isRunning; }
  bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
  void PollEvents() { glfwPollEvents(); }
  void Clear(GLbitfield mask)
  {
    glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
    glClear(mask);
  }
  void Present() { glfwSwapBuffers(m_Window); }
  void ApplySavedSettings();

  void SetTitle(const std::string &title);
  void SetSize(int width, int height);
  void SetPosition(int x, int y);
  void SetIcon(const std::string &path);
  void SetClearColor(const math::vec4 &color) { m_ClearColor = color; }
  void SetContextCurrent() { glfwMakeContextCurrent(m_Window); }
  void SetWindowed();
  void SetFullscreen();
  void SetFullscreenBorderless();
  void SetVSync(bool enabled);

  GLFWwindow *GetWindowHandle() const { return m_Window; }
  std::string GetGLSLVersion() const { return m_glsl_version; }
  math::vec2 GetSize() const;
  int GetWidth() const;
  int GetHeight() const;
  int GetXPos() const;
  int GetYPos() const;
  float GetAspectRatio() const;
  float GetTime() { return (float)glfwGetTime(); }
  auto GetFramebufferSize() const
  {
    int width, height;
    glfwGetFramebufferSize(m_Window, &width, &height);
    return math::ivec2(width, height);
  }
  math::vec4 &GetClearColor() { return m_ClearColor; }
  bool IsVSync() const;
  const std::string &GetTitle() const;
  const ApplicationSpecification &GetSpecification() const;

  operator GLFWwindow *() const { return m_Window; }
  operator GLFWwindow *() { return m_Window; }

  // void SetEventCallback(const EventCallbackFn &callback) { m_Application->GetSpecification().EventCallback = callback; }

 private:
  static void *ReadOpenCallback(ImGuiContext *, ImGuiSettingsHandler *handler, const char *name) { return handler->UserData; }
  static void ReadLineCallback(ImGuiContext *, ImGuiSettingsHandler *, void *entry, const char *line);
  static void WriteAllCallback(ImGuiContext *, ImGuiSettingsHandler *handler, ImGuiTextBuffer *buf);

 private:
  std::atomic_bool m_IsRunning = false;

  GLFWwindow *m_Window;
  Application *m_Application;

  math::vec4 m_ClearColor;
  std::string m_glsl_version = "#version 460";

  friend ::Ham::ImGuiImpl;
};

}  // namespace Ham