#include "enxpch.h"

#include "Application.h"

namespace EngineX
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }
    Application::~Application() = default;

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }
}
