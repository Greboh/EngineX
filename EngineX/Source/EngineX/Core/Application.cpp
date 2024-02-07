#include "enxpch.h"

#include "Application.h"

namespace EngineX
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallBack(ENX_BIND_EVENT_FN(Application::OnEvent));
    }
    Application::~Application() = default;

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENX_BIND_EVENT_FN(OnWindowClose));
        
        ENX_ENGINE_TRACE("{0}", e);
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->OnUpdate();
        }

        ENX_ENGINE_TRACE("Shutting down ...");
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}
