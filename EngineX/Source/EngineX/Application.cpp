#include "Application.h"

#include <iostream>


namespace EngineX
{
    Application::Application() = default;
    Application::~Application() = default;

    void Application::Run()
    {
        std::cout << "Run called!\n";
        std::cin.get();
    }
}
