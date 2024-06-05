#include "Ham/Input/Input.h"

#include "Ham/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Ham {

std::map<KeyCode, std::pair<bool, bool>> Input::m_KeyStates;
std::map<MouseButton, std::pair<bool, bool>> Input::m_MouseButtonStates;
int Input::s_MouseWheelDelta = 0;
int Input::s_MouseWheelChanged = 0;
CursorMode Input::s_CurrentCursorMode = CursorMode::NORMAL;
CursorMode Input::s_DesiredCursorMode = CursorMode::NORMAL;
math::vec2 Input::s_PreviousMousePosition = {0, 0};

void Input::Init()
{
  //  mouse wheel callback
  auto &app = Application::Get();
  GLFWwindow *windowHandle = app.GetWindowHandle();
  // glfwSetScrollCallback(windowHandle, [](GLFWwindow *window, double xOffset, double yOffset) {
  //   auto &app = Application::Get();
  //   if (app.GetImGui().WantsCaptureMouse())
  //     return;

  //   if (yOffset > 0)
  //     Input::s_MouseWheelChanged = 1;
  //   else if (yOffset < 0)
  //     Input::s_MouseWheelChanged = -1;
  //   else
  //     Input::s_MouseWheelChanged = 0;
  //   //
  // });

  glfwSetWindowFocusCallback(windowHandle, [](GLFWwindow *window, int focused) {
    auto &app = Application::Get();
    if (!focused)
      Input::s_DesiredCursorMode = CursorMode::NORMAL;
    //
  });
}

void Input::OnScroll(Application* app, float xOffset, float yOffset)
{
  if (app->GetImGui().WantsCaptureMouse())
    return;

  if (yOffset > 0)
    Input::s_MouseWheelChanged = 1;
  else if (yOffset < 0)
    Input::s_MouseWheelChanged = -1;
  else
    Input::s_MouseWheelChanged = 0;
}

bool Input::IsKeyDown(KeyCode keycode)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureKey(keycode))
    return false;

  if (keycode == KeyCode::ANY) {
    for (auto &key : m_KeyStates) {
      if (key.second.first)
        return true;
    }
    return false;
  }

  if (m_KeyStates.find(keycode) == m_KeyStates.end())
    m_KeyStates[keycode] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetKey(windowHandle, (int)keycode);
  m_KeyStates[keycode].first = state == GLFW_PRESS || state == GLFW_REPEAT;
  return m_KeyStates[keycode].first;
}

bool Input::IsKeyDownThisFrame(KeyCode keycode)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureKey(keycode))
    return false;

  if (keycode == KeyCode::ANY) {
    for (auto &key : m_KeyStates) {
      if (key.second.first && !key.second.second)
        return true;
    }
    return false;
  }

  if (m_KeyStates.find(keycode) == m_KeyStates.end())
    m_KeyStates[keycode] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetKey(windowHandle, (int)keycode);
  m_KeyStates[keycode].first = state == GLFW_PRESS || state == GLFW_REPEAT;
  return m_KeyStates[keycode].first && !m_KeyStates[keycode].second;
}

bool Input::IsKeyUp(KeyCode keycode)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureKey(keycode))
    return false;

  if (keycode == KeyCode::ANY) {
    for (auto &key : m_KeyStates) {
      if (!key.second.first)
        return true;
    }
    return false;
  }

  if (m_KeyStates.find(keycode) == m_KeyStates.end())
    m_KeyStates[keycode] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetKey(windowHandle, (int)keycode);
  m_KeyStates[keycode].first = state == GLFW_RELEASE || state == GLFW_REPEAT;
  return !m_KeyStates[keycode].first;
}

bool Input::IsKeyUpThisFrame(KeyCode keycode)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureKey(keycode))
    return false;

  if (keycode == KeyCode::ANY) {
    for (auto &key : m_KeyStates) {
      if (!key.second.first && key.second.second)
        return true;
    }
    return false;
  }

  if (m_KeyStates.find(keycode) == m_KeyStates.end())
    m_KeyStates[keycode] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetKey(windowHandle, (int)keycode);
  m_KeyStates[keycode].first = state == GLFW_PRESS || state == GLFW_REPEAT;
  return !m_KeyStates[keycode].first && m_KeyStates[keycode].second;
}

bool Input::IsMouseButtonDown(MouseButton button)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureMouse())
    return false;

  if (button == MouseButton::ANY) {
    for (auto &button : m_MouseButtonStates) {
      if (button.second.first)
        return true;
    }
    return false;
  }

  if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
    m_MouseButtonStates[button] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetMouseButton(windowHandle, (int)button);
  m_MouseButtonStates[button].first = state == GLFW_PRESS;
  return m_MouseButtonStates[button].first;
}

bool Input::IsMouseButtonDownThisFrame(MouseButton button)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureMouse())
    return false;

  if (button == MouseButton::ANY) {
    for (auto &button : m_MouseButtonStates) {
      if (button.second.first && !button.second.second)
        return true;
    }
    return false;
  }

  if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
    m_MouseButtonStates[button] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetMouseButton(windowHandle, (int)button);
  m_MouseButtonStates[button].first = state == GLFW_PRESS;
  return m_MouseButtonStates[button].first && !m_MouseButtonStates[button].second;
}

bool Input::IsMouseButtonUp(MouseButton button)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureMouse() && button != MouseButton::ANY)
    return false;

  if (button == MouseButton::ANY) {
    for (auto &button : m_MouseButtonStates) {
      if (!button.second.first)
        return true;
    }
    return false;
  }

  if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
    m_MouseButtonStates[button] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetMouseButton(windowHandle, (int)button);
  m_MouseButtonStates[button].first = state == GLFW_PRESS;
  return !m_MouseButtonStates[button].first;
}

bool Input::IsMouseButtonUpThisFrame(MouseButton button)
{
  auto &app = Application::Get();

  if (app.GetImGui().WantsCaptureMouse() && button != MouseButton::ANY)
    return false;

  if (button == MouseButton::ANY) {
    for (auto &button : m_MouseButtonStates) {
      if (!button.second.first && button.second.second)
        return true;
    }
    return false;
  }

  if (m_MouseButtonStates.find(button) == m_MouseButtonStates.end())
    m_MouseButtonStates[button] = {false, false};

  GLFWwindow *windowHandle = app.GetWindowHandle();
  int state = glfwGetMouseButton(windowHandle, (int)button);
  m_MouseButtonStates[button].first = state == GLFW_PRESS;
  return !m_MouseButtonStates[button].first && m_MouseButtonStates[button].second;
}

int Input::GetMouseWheelDelta()
{
  return s_MouseWheelDelta;
}

math::vec2 Input::GetMouseDelta()
{
  return GetMousePosition() - GetPreviousMousePosition();
}

math::vec2 Input::GetPreviousMousePosition()
{
  return s_PreviousMousePosition;
}

math::vec2 Input::GetMousePosition()
{
  GLFWwindow *windowHandle = Application::Get().GetWindowHandle();

  double x, y;
  glfwGetCursorPos(windowHandle, &x, &y);
  return {(float)x, (float)y};
}

void Input::SetCursorMode(CursorMode mode)
{
  GLFWwindow *windowHandle = Application::Get().GetWindowHandle();
  s_DesiredCursorMode = mode;
}

void Input::Update()
{
  GLFWwindow *windowHandle = Application::Get().GetWindowHandle();
  if (s_DesiredCursorMode != s_CurrentCursorMode) {
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)s_DesiredCursorMode);
    s_CurrentCursorMode = s_DesiredCursorMode;
  }
}

void Input::BeginFrame()
{
  s_MouseWheelDelta = s_MouseWheelChanged;
  s_MouseWheelChanged = 0;
}

void Input::EndFrame()
{
  for (auto &key : m_KeyStates) {
    key.second.second = key.second.first;
  }

  for (auto &button : m_MouseButtonStates) {
    button.second.second = button.second.first;
  }

  s_MouseWheelDelta = 0;

  s_PreviousMousePosition = GetMousePosition();
}

}  // namespace Ham