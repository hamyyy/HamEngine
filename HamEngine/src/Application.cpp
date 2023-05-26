#include "Ham/Application.h"

#include <iostream>

namespace Ham
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    int Application::Run()
    {
        std::cout << "Ham Engine Version: 0.0.1" << std::endl;
        return 0;
    }
} // namespace Ham
