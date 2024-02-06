#pragma once
#include "Core.h"
#include "Window.h"

namespace EngineX
{
    class ENGINEX_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        // Unique because only this class owns it!
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
