#pragma once

namespace Ham
{
    class Application
    {
    public:
        Application();
        ~Application();

        virtual int Run() = 0;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}