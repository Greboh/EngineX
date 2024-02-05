#pragma once
#include "Core.h"

namespace EngineX
{
    class ENGINEX_API Application
    {
    public:
        Application();
        virtual ~Application();

        static void Run();
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
