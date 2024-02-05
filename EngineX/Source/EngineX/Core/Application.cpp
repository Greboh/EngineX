#include "Application.h"

#include <iostream>

#include "EngineX/Core/Log.h"
#include "EngineX/Events/ApplicationEvent.h"


namespace EngineX
{
    Application::Application() = default;
    Application::~Application() = default;

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        ENX_TRACE(e.ToString());
        while (true) {}
    }
}
