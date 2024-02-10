#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "EngineX/Events/Event.h"
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

        inline static Application& Get() {return *s_Instance; }
        inline Window& GetWindow() const { return *m_Window; }
        
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        
        // Unique because only this class should own the window!
        Scope<Window> m_Window;
        bool m_Running = true;

        LayerStack m_Layerstack;

        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
