#include <EngineX.h>
#include <EngineX/Core/EntryPoint.h>

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
            ENX_INFO("The tab key is pressed (poll)!");
        }
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
        InsertOverlay(new EngineX::ImGuiLayer());
    }

    ~Sandbox() override = default;
};


EngineX::Application* EngineX::CreateApplication()
{
    return new Sandbox();
}
