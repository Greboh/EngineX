#pragma once

#include "EngineX/Core/Application.h"

#ifdef ENX_PLATFORM_WINDOWS

extern EngineX::Application* EngineX::CreateApplication();

int main(int argc, char** arv)
{
    EngineX::Log::Init();

    auto app = EngineX::CreateApplication();

    app->Run();

    // This releases the memory allocation on the heap by using the 'delete' keyword
    delete app;

    return 0;
}
#endif
