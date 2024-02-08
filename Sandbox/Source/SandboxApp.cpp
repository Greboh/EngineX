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
        // ENX_INFO("{0}::OnUpdate", m_DebugName);
    }
    
    void OnEvent(EngineX::Event& e) override
    {
        ENX_TRACE("{0}:({1})",m_DebugName, e);
    }
};

class TestOverlay : public EngineX::Layer
{
public:
    TestOverlay(const std::string& name) : Layer(name)
    {
        
    }

    void OnUpdate() override
    {
        // ENX_INFO("{0}::OnUpdate", m_DebugName);        
    }
    void OnEvent(EngineX::Event& e) override
    {
        ENX_TRACE("{0}:({1})",m_DebugName, e);
    }
};

class Sandbox : public EngineX::Application
{
public:
    Sandbox()
    {
        InsertLayer(new TestLayer());
        InsertOverlay(new TestOverlay("TestOverlay"));
    }

    ~Sandbox() override = default;
};


EngineX::Application* EngineX::CreateApplication()
{
    return new Sandbox();
}
