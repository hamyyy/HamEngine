#pragma once

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glad/glad.h>  // Initialize with gladLoadGL()

#include <glm/glm.hpp>

#include <string>

namespace Ham
{
    struct ApplicationSpecification;

    class Window
    {
    public:
        Window();
        Window(const Window &other) = default;
        ~Window();

        void Init(ApplicationSpecification &specification);
        void Update();
        void Shutdown();

        bool IsRunning() const { return m_IsRunning; }
        void SetIsRunning(bool isRunning) { m_IsRunning = isRunning; }
        bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
        void PollEvents() { glfwPollEvents(); }
        void Clear(GLbitfield mask)
        {
            glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
            glClear(mask);
        }
        void Present() { glfwSwapBuffers(m_Window); }

        void SetTitle(const std::string &title) { glfwSetWindowTitle(m_Window, title.c_str()); }
        void SetSize(int width, int height) { glfwSetWindowSize(m_Window, width, height); }
        void SetPosition(int x, int y) { glfwSetWindowPos(m_Window, x, y); }
        void SetVSync(bool enabled) { glfwSwapInterval(enabled); }
        void SetIcon(const std::string &path);
        void SetClearColor(const glm::vec4 &color) { m_ClearColor = color; }
        void SetContextCurrent() { glfwMakeContextCurrent(m_Window); }

        GLFWwindow *GetGLFWWindow() const { return m_Window; }
        std::string GetGLSLVersion() const { return m_glsl_version; }
        int GetWidth() const { return m_Data.Width; }
        int GetHeight() const { return m_Data.Height; }
        int GetX() const { return m_Data.X; }
        int GetY() const { return m_Data.Y; }
        float GetTime() { return (float)glfwGetTime(); }
        auto GetFramebufferSize() const
        {
            int width, height;
            glfwGetFramebufferSize(m_Window, &width, &height);
            return glm::ivec2(width, height);
        }
        glm::vec4 &GetClearColor() { return m_ClearColor; }
        bool IsVSync() const { return m_Data.VSync; }
        const std::string &GetTitle() const { return m_Data.Title; }

        // void SetEventCallback(const EventCallbackFn &callback) { m_Data.EventCallback = callback; }

    private:
        GLFWwindow *m_Window;
        bool m_IsRunning = false;
        glm::vec4 m_ClearColor;

        std::string m_glsl_version = "#version 460";

        struct WindowData
        {
            std::string Title;
            int Width, Height;
            int X, Y;
            bool VSync;
            // EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

} // namespace Ham