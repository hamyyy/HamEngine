#pragma once

#include "Ham/Window.h"

namespace Ham
{
    class ImGuiImpl
    {
    public:
        ImGuiImpl();
        ~ImGuiImpl();

        void Init(Window* window);
        void NewFrame();
        void Render();
        void Shutdown();

    private:
        Window* m_Window;
        bool m_IsShutDown = false;
    };
} // namespace Ham
