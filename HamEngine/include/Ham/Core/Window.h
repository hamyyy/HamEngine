#pragma once

#include "Ham/ImGui/ImGuiImpl.h"

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glad/glad.h>  // Initialize with gladLoadGL()
#include <imgui_internal.h>
#include <glm/glm.hpp>

#include <string>

namespace Ham
{
    struct ApplicationSpecification;
    class Application;

    class Window
    {
    public:
        Window();
        Window(const Window &other) = default;
        ~Window();

        void Init(Application *app);
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
        void ApplySavedSettings();

        void SetTitle(const std::string &title);
        void SetSize(int width, int height);
        void SetPosition(int x, int y);
        void SetIcon(const std::string &path);
        void SetClearColor(const glm::vec4 &color) { m_ClearColor = color; }
        void SetContextCurrent() { glfwMakeContextCurrent(m_Window); }
        void SetWindowed();
        void SetFullscreen();
        void SetFullscreenBorderless();
        void SetVSync(bool enabled);

        GLFWwindow *GetWindowHandle() const { return m_Window; }
        std::string GetGLSLVersion() const { return m_glsl_version; }
        int GetWidth() const;
        int GetHeight() const;
        float GetAspectRatio() const;
        float GetTime() { return (float)glfwGetTime(); }
        auto GetFramebufferSize() const
        {
            int width, height;
            glfwGetFramebufferSize(m_Window, &width, &height);
            return glm::ivec2(width, height);
        }
        glm::vec4 &GetClearColor() { return m_ClearColor; }
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
        bool m_IsRunning = false;

        GLFWwindow *m_Window;
        Application *m_Application;

        glm::vec4 m_ClearColor;
        std::string m_glsl_version = "#version 460";

        friend ::Ham::ImGuiImpl;
    };

} // namespace Ham