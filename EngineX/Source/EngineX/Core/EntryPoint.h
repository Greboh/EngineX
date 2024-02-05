#pragma once

#ifdef ENX_PLATFORM_WINDOWS

extern EngineX::Application* EngineX::CreateApplication();

int main(int argc, char** arv)
{
    EngineX::Log::Init();

    ENX_ENGINE_WARN("Intialized Log!");
    ENX_INFO("Hello!");

    const auto app = EngineX::CreateApplication();

    // This requests memory allocation on the heap by using the 'new' keyword 
    app->Run();

    // This releases the memory allocation on the heap by using the 'delete' keyword
    delete app;
}
#endif
