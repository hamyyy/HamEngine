#include "Ham/Core/Window.h"

#include "Ham/Core/Application.h"
#include "Ham/Core/Assert.h"
#include "Ham/Core/Log.h"
#include "Ham/Util/GLFWExtra.h"
#include "Ham/Events/Event.h"
#include "imgui.h"

#ifdef _MSC_VER
#define SSCANF sscanf_s
#else
#define SSCANF sscanf
#endif

namespace Ham {

static void glfw_error_callback(int error, const char *description)
{
  HAM_CORE_ERROR("GLFW Error ({0}): {1}", error, std::string(description));
}

static void opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
  HAM_CORE_ERROR("OpenGL Error ({0}): {1}", id, std::string(message));
}

Window::Window() {}
Window::~Window()
{
  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

void Window::Init(Application *app)
{
  m_Application = app;
  glfwSetErrorCallback(glfw_error_callback);

  HAM_CORE_ASSERT(glfwInit(), "Could not initialize GLFW!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // Debug
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

  // Create window with graphics context
  m_Window = glfwCreateWindow(GetSpecification().Width, GetSpecification().Height, GetSpecification().Name.c_str(), nullptr, nullptr);
  HAM_CORE_ASSERT(m_Window != nullptr, "Could not create GLFW window!");
  glfwMakeContextCurrent(m_Window);
  gladLoadGL(glfwGetProcAddress);  // Initialize OpenGL loader

  // glEnable(GL_DEBUG_OUTPUT);                              // Debug
  glDebugMessageCallback(opengl_error_callback, nullptr);

  // print version information
  HAM_CORE_INFO("OpenGL Version: {}", std::string((char *)glGetString(GL_VERSION)));
  HAM_CORE_INFO("GLSL Version: {}", std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));
  HAM_CORE_INFO("Dear ImGui Version: {}", std::string((char *)ImGui::GetVersion()));

  {
    static std::vector<Events::MouseDragged::DragData> dragData;  // TODO: combine these into a single struct
    static std::vector<Events::MouseClicked::ClickData> clickData;

    // Initialize drag data for each mouse button
    for (int i = 0; i < 6; i++) {
      dragData.emplace_back((Ham::MouseButton)i);
      clickData.emplace_back((Ham::MouseButton)i);
    }

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      app.GetSpecificationMutable().Width = width;
      app.GetSpecificationMutable().Height = height;
      Events::Emit<Events::WindowResized>(width, height);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::WindowClosed>();
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);

      switch (action) {
        case GLFW_PRESS: {
          Events::Emit<Events::KeyPressed>((Ham::KeyCode)key, 0);
          break;
        }
        case GLFW_RELEASE: {
          Events::Emit<Events::KeyReleased>((Ham::KeyCode)key);
          break;
        }
        case GLFW_REPEAT: {
          Events::Emit<Events::KeyPressed>((Ham::KeyCode)key, 1);
          break;
        }
      }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::KeyTyped>(keycode);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);

      double x, y;
      glfwGetCursorPos(window, &x, &y);

      switch (action) {
        case GLFW_PRESS: {
          Events::Emit<Events::MousePressed>((Ham::MouseButton)button);

          {
            // Clicked
            auto &data = clickData[button];
            data.pos = {x, y};
            data.isPressed = true;
            data.isDragging = false;
          }

          {
            // Dragging
            if (app.GetImGui().WantsCaptureMouse()) break;
            auto &data = dragData[button];
            data.isPressed = true;
            data.startPos = {x, y};
            data.pos = {x, y};
            data.delta = {0, 0};
            data.isDragging = false;
          }

          break;
        }
        case GLFW_RELEASE: {
          Events::Emit<Events::MouseReleased>((Ham::MouseButton)button);

          {
            // Clicked
            auto &data = clickData[button];
            if (data.isPressed && !data.isDragging) {
              uint32_t now = (uint32_t)(glfwGetTime() * 1000.0);
              data.timeSinceLastClick = now - data.timer;

              if (now > data.timer + data.doubleClickTime) {
                data.clickCount = 0;
              }
              data.timer = now;

              data.clickCount++;
              Events::Emit<Events::MouseClicked>(data);
              data.isPressed = false;
            }
          }

          {  // Dragging
            auto &data = dragData[button];
            data.isPressed = false;
            data.pos = {x, y};
            data.isDragging = false;
          }

          break;
        }
      }
    });

    auto value = glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::MouseScrolled>((float)xOffset, (float)yOffset);
    });
    HAM_CORE_INFO("Scroll callback set: {0}", (bool)value);

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::MouseMoved>((float)xPos, (float)yPos);

      {
        // Clicked
        for (auto &data : clickData) {
          if (data.isPressed && !data.isDragging) {
            math::vec2 newPos = {(float)xPos, (float)yPos};
            math::vec2 delta = newPos - data.pos;
            if (math::length(delta) > data.tollerance) {
              data.isDragging = true;
            }
          }
        }
      }

      {  // Dragging
        for (auto &data : dragData) {
          math::vec2 newPos = {(float)xPos, (float)yPos};
          math::vec2 delta = newPos - data.startPos;
          if (data.isDragging || data.isPressed && math::length(delta) > data.tollerance) {
            data.deltaThisFrame = newPos - data.pos;
            data.pos = newPos;
            data.delta = delta;
            data.isDragging = true;
            Events::Emit<Events::MouseDragged>(data);
          }
        }
      }
    });

    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow *window, int focused) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      if (!focused)
        Events::Emit<Events::WindowBlurred>();
      else
        Events::Emit<Events::WindowFocused>();
    });

    glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow *window, int maximized) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      if (maximized)
        Events::Emit<Events::WindowMaximized>();
      else
        Events::Emit<Events::WindowRestored>();
    });

    glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow *window, int iconified) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      if (iconified)
        Events::Emit<Events::WindowMinimized>();
      else
        Events::Emit<Events::WindowRestored>();
    });

    glfwSetWindowPosCallback(m_Window, [](GLFWwindow *window, int xPos, int yPos) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::WindowMoved>(xPos, yPos);
    });

    glfwSetDropCallback(m_Window, [](GLFWwindow *window, int count, const char **paths) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::FileDropped>(count, paths);
    });

    glfwSetWindowContentScaleCallback(m_Window, [](GLFWwindow *window, float xScale, float yScale) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::WindowContentScaled>(xScale, yScale);
    });

    glfwSetWindowRefreshCallback(m_Window, [](GLFWwindow *window) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::WindowRefreshed>();
    });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
      Application &app = *(Application *)glfwGetWindowUserPointer(window);
      Events::Emit<Events::FramebufferResized>(width, height);
    });

    glfwSetJoystickCallback([](int jid, int event) {
      if (event == GLFW_CONNECTED) {
        Events::Emit<Events::JoystickConnected>(jid);
      }
      else if (event == GLFW_DISCONNECTED) {
        Events::Emit<Events::JoystickDisconnected>(jid);
      }
    });

    glfwSetMonitorCallback([](GLFWmonitor *monitor, int event) {
      if (event == GLFW_CONNECTED) {
        Events::Emit<Events::MonitorConnected>(monitor);
      }
      else if (event == GLFW_DISCONNECTED) {
        Events::Emit<Events::MonitorDisconnected>(monitor);
      }
    });
  }
}

void Window::Update()
{
}

void Window::SetTitle(const std::string &title)
{
  glfwSetWindowTitle(m_Window, title.c_str());
  m_Application->m_Specification.Name = title;
}

void Window::SetSize(int width, int height)
{
  glfwSetWindowSize(m_Window, width, height);
  m_Application->m_Specification.Width = width;
  m_Application->m_Specification.Height = height;
}

void Window::SetPosition(int x, int y)
{
  glfwSetWindowPos(m_Window, x, y);
  m_Application->m_Specification.XPos = x;
  m_Application->m_Specification.YPos = y;
}

void Window::SetIcon(const std::string &path)
{
}

math::vec2 Window::GetSize() const
{
  int width, height;
  glfwGetWindowSize(GetWindowHandle(), &width, &height);
  return {(float)width, (float)height};
}

int Window::GetWidth() const
{
  int width, height;
  glfwGetWindowSize(GetWindowHandle(), &width, &height);
  return width;
}

int Window::GetHeight() const
{
  int width, height;
  glfwGetWindowSize(GetWindowHandle(), &width, &height);
  return height;
}

int Window::GetXPos() const
{
  int x, y;
  glfwGetWindowPos(GetWindowHandle(), &x, &y);
  return x;
}

int Window::GetYPos() const
{
  int x, y;
  glfwGetWindowPos(GetWindowHandle(), &x, &y);
  return y;
}

float Window::GetAspectRatio() const
{
  int width, height;
  glfwGetWindowSize(GetWindowHandle(), &width, &height);
  return (float)width / (float)height;
}
bool Window::IsVSync() const { return m_Application->GetSpecification().VSync; }
const std::string &Window::GetTitle() const { return m_Application->GetSpecification().Name; }
const ApplicationSpecification &Window::GetSpecification() const { return m_Application->GetSpecification(); }

void Window::ApplySavedSettings()
{
  auto &settings = m_Application->m_Specification;

  int monitorCount;
  glfwGetMonitors(&monitorCount);
  auto monitor = glfwGetCurrentMonitor(GetWindowHandle());
  if (settings.Display < 0 || settings.Display >= monitorCount)
    settings.Display = 0;

  int monitorPosX, monitorPosY;
  glfwGetMonitorPos(monitor, &monitorPosX, &monitorPosY);

  if (settings.XPos >= 0 && settings.YPos >= 0)
    glfwSetWindowPos(GetWindowHandle(), settings.XPos + monitorPosX, settings.YPos + monitorPosY);
  glfwCenterWindow(GetWindowHandle());
  glfwGetWindowPos(GetWindowHandle(), &settings.XPos, &settings.YPos);

  if (settings.Maximized)
    glfwMaximizeWindow(GetWindowHandle());

  switch (settings.Fullscreen) {
    case APPLICATION_WINDOWED:
      SetWindowed();
      break;
    case APPLICATION_FULLSCREEN:
      SetFullscreen();
      break;
    case APPLICATION_FULLSCREEN_BORDERLESS:
      SetFullscreenBorderless();
      break;
    default:
      break;
  }

  glfwSetWindowSizeLimits(GetWindowHandle(), 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
  // glfwShowWindow(GetWindowHandle());
}

// Serialize Imgui
void Window::ReadLineCallback(ImGuiContext *, ImGuiSettingsHandler *, void *entry, const char *line)
{
  auto *app = (::Ham::Application *)entry;
  auto &spec = app->m_Specification;

  int x, y;
  if (SSCANF(line, "WindowSize=%i,%i", &x, &y) == 2) {
    spec.Width = x;
    spec.Height = y;
  }
  else if (SSCANF(line, "Maximized=%i", &x) == 1) {
    spec.Maximized = (bool)x;
  }
  else if (SSCANF(line, "WindowPos=%i,%i", &x, &y) == 2) {
    spec.XPos = x;
    spec.YPos = y;
  }
  else if (SSCANF(line, "Display=%i", &x) == 1) {
    spec.Display = x;
  }
  else if (SSCANF(line, "Fullscreen=%i", &x) == 1) {
    spec.Fullscreen = (FullscreenMode)x;
  }
  else if (SSCANF(line, "VSync=%i", &x) == 1) {
    spec.VSync = (bool)x;
  }
}

void Window::WriteAllCallback(ImGuiContext *ctx, ImGuiSettingsHandler *handler, ImGuiTextBuffer *buf)
{
  auto app = (::Ham::Application *)handler->UserData;
  auto &spec = app->GetSpecification();
  auto window = app->GetWindow().GetWindowHandle();

  buf->appendf("[%s][%s]\n", handler->TypeName, "WindowConfig");

  bool maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED) == GLFW_TRUE;
  buf->appendf("Maximized=%d\n", maximized);

  if (maximized || spec.Fullscreen > 0)
    buf->appendf("WindowSize=%d,%d\n", spec.Width, spec.Height);
  else {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    buf->appendf("WindowSize=%d,%d\n", width, height);
  }

  int xPos, yPos, monitorPosX, monitorPosY;
  glfwGetWindowPos(window, &xPos, &yPos);
  glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorPosX, &monitorPosY);
  buf->appendf("WindowPos=%d,%d\n", xPos - monitorPosX, yPos - monitorPosY);

  int monitorIndex;
  glfwGetCurrentMonitor(window, &monitorIndex);
  buf->appendf("Display=%d\n", monitorIndex);

  buf->appendf("Fullscreen=%d\n", spec.Fullscreen);

  buf->appendf("VSync=%d\n", (int)spec.VSync);

  buf->append("\n");
}

void Window::SetWindowed()
{
  int x, y, width, height;
  auto videoMode = glfwGetVideoMode(glfwGetCurrentMonitor(GetWindowHandle()));
  width = videoMode->width;
  height = videoMode->height;
  glfwGetMonitorPos(glfwGetCurrentMonitor(GetWindowHandle()), &x, &y);

  glfwSetWindowAttrib(GetWindowHandle(), GLFW_DECORATED, GLFW_TRUE);
  auto isMaximized = glfwGetWindowAttrib(GetWindowHandle(), GLFW_MAXIMIZED);

  auto wasFullscreen = glfwGetWindowMonitor(GetWindowHandle());
  if (wasFullscreen) {
    // exit fullscreen
    glfwSetWindowMonitor(GetWindowHandle(), nullptr, x - width / 2 - GetSpecification().Width / 2, y - height / 2 - GetSpecification().Height / 2, GetSpecification().Width, GetSpecification().Height, GLFW_DONT_CARE);
    glfwRestoreWindow(GetWindowHandle());
  }

  if (!isMaximized) {
    glfwRestoreWindow(GetWindowHandle());
  }
  else {
    glfwMaximizeWindow(GetWindowHandle());
  }

  int windowWidth, windowHeight;
  glfwGetWindowSize(GetWindowHandle(), &windowWidth, &windowHeight);
  if ((windowWidth >= width || windowHeight >= height) && !isMaximized) {
    glfwSetWindowSize(GetWindowHandle(), GetSpecification().DefaultWidth, GetSpecification().DefaultHeight);
    m_Application->m_Specification.Width = m_Application->m_Specification.DefaultWidth;
    m_Application->m_Specification.Height = m_Application->m_Specification.DefaultHeight;
    glfwCenterWindow(GetWindowHandle());
  }

  if (glfwIsWindowOffScreen(GetWindowHandle())) {
    glfwCenterWindow(GetWindowHandle());
  }

  m_Application->m_Specification.Fullscreen = APPLICATION_WINDOWED;
}

void Window::SetFullscreen()
{
  int width, height;
  auto monitor = glfwGetCurrentMonitor(GetWindowHandle());
  auto videoMode = glfwGetVideoMode(glfwGetCurrentMonitor(GetWindowHandle()));
  width = videoMode->width;
  height = videoMode->height;

  glfwSetWindowMonitor(GetWindowHandle(), monitor, 0, 0, width, height, GLFW_DONT_CARE);

  m_Application->m_Specification.Fullscreen = APPLICATION_FULLSCREEN;
}

void Window::SetFullscreenBorderless()
{
  int x, y, width, height;
  auto videoMode = glfwGetVideoMode(glfwGetCurrentMonitor(GetWindowHandle()));
  width = videoMode->width;
  height = videoMode->height;
  glfwGetMonitorPos(glfwGetCurrentMonitor(GetWindowHandle()), &x, &y);

  glfwSetWindowAttrib(GetWindowHandle(), GLFW_DECORATED, GLFW_FALSE);

  if (glfwGetWindowMonitor(GetWindowHandle())) {
    // exit fullscreen
    glfwSetWindowMonitor(GetWindowHandle(), nullptr, 0, 0, width, height, GLFW_DONT_CARE);
  }
  else {
    glfwSetWindowPos(GetWindowHandle(), x, y);
    glfwSetWindowSize(GetWindowHandle(), width, height);
  }

  m_Application->m_Specification.Fullscreen = APPLICATION_FULLSCREEN_BORDERLESS;
}

void Window::SetVSync(bool enabled)
{
  glfwSwapInterval(enabled);
  m_Application->m_Specification.VSync = enabled;
}

}  // namespace Ham