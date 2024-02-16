#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "EngineX/Events/Event.h"
#include "EngineX/Events/ApplicationEvent.h"
#include "EngineX/ImGui/ImGuiLayer.h"

namespace EngineX
{
    class Application
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
        
        static Application* s_Instance;
        
        // Unique because only this class should own the window!
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;

        LayerStack m_Layerstack;
        
        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
