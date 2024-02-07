#pragma once
#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "EngineX/Events/ApplicationEvent.h"

namespace EngineX
{
    class ENGINEX_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void InsertLayer(Layer* layer);
        void InsertOverlay(Layer* layer);

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        
        // Unique because only this class should own the window!
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;

        LayerStack m_Layerstack;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
