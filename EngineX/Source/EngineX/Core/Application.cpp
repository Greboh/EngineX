#include "enxpch.h"

#include "Application.h"

namespace EngineX
{
    Application::Application()
    {
        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallBack(ENX_BIND_EVENT_FN(Application::OnEvent));
    }
    Application::~Application() = default;

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENX_BIND_EVENT_FN(OnWindowClose));
        
        ENX_ENGINE_TRACE("{0}", e);

        // Handle events from the last index -> first index (See LayerStack.h for information!)
        for (auto iterator = m_Layerstack.end(); iterator != m_Layerstack.begin();)
        {
            // We begin at the index just beyond the last, so we have to subtract one to call the last
            (*--iterator)->OnEvent(e);

            // If the event gets handled break out of the loop. So we don't chain the event
            if(e.Handled)
            {
                break;
            }
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // NOTE: Currently we are updating our layers first and then our window .. Might change in the future

            // Update our layers from first index -> last index
            for (Layer* layer : m_Layerstack)
            {
                layer->OnUpdate();
            }
            
            m_Window->OnUpdate();
        }

    

        ENX_ENGINE_TRACE("Shutting down ...");
    }

    void Application::InsertLayer(Layer* layer)
    {
        m_Layerstack.InsertLayer(layer);
    }

    void Application::InsertOverlay(Layer* layer)
    {
        m_Layerstack.InsertOverlay(layer);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}
