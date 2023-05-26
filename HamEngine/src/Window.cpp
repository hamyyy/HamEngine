#include "Ham/Window.h"

#include "imgui.h"

#include "Ham/Log.h"
#include "Ham/Assert.h"
#include "Ham/Application.h"

namespace Ham
{

    static void glfw_error_callback(int error, const char *description)
    {
        HAM_CORE_ERROR("GLFW Error ({0}): {1}", error, std::string(description));
    }

    Window::Window() {}
    Window::~Window() {}

    void Window::Init(ApplicationSpecification &specification)
    {
        glfwSetErrorCallback(glfw_error_callback);

        HAM_CORE_ASSERT(glfwInit(), "Could not initialize GLFW!");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create window with graphics context
        m_Window = glfwCreateWindow(specification.Width, specification.Height, specification.Name.c_str(), nullptr, nullptr);
        HAM_CORE_ASSERT(m_Window != nullptr, "Could not create GLFW window!");
        glfwMakeContextCurrent(m_Window);
        gladLoadGL(); // Initialize OpenGL loader

        // print version information
        HAM_CORE_INFO("OpenGL Version: {}", std::string((char *)glGetString(GL_VERSION)));
        HAM_CORE_INFO("GLSL Version: {}", std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));
        HAM_CORE_INFO("Dear ImGui Version: {}", std::string((char *)ImGui::GetVersion()));
    }

    void Window::Update()
    {
    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

} // namespace Ham