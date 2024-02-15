#include <EngineX.h>
#include <EngineX/Core/EntryPoint.h>

#include "imgui/imgui.h"

class TestLayer : public EngineX::Layer
{
public:
    TestLayer() : Layer("TestLayer")
    {
        
    }

    void OnUpdate() override
    {
        if(EngineX::InputManager::IsKeyPressed(EngineX::Key::Tab))
        {
            ENX_TRACE("The tab key is pressed (poll)!");
            ENX_INFO("The tab key is pressed (poll)!");
            ENX_WARN("The tab key is pressed (poll)!");
            ENX_ERROR("The tab key is pressed (poll)!");
            ENX_ENGINE_INFO("The tab key is pressed (poll)!");
        }
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Viewport");
        ImGui::Text("Hello Viewport!");
        ImGui::End();
    }

    void OnEvent(EngineX::Event& e) override
    {
        
    }
};

class Sandbox : public EngineX::Application
{
public:
    Sandbox()
    {
        InsertLayer(new TestLayer());
    }

    ~Sandbox() override = default;
};


EngineX::Application* EngineX::CreateApplication()
{
    return new Sandbox();
}
