#include <EngineX.h>
#include <EngineX/Core/EntryPoint.h>

#include "EditorLayer.h"

class Sandbox : public EngineX::Application
{
public:
    Sandbox()
    {
        InsertLayer(new EditorLayer());
    }

    ~Sandbox() override = default;
};


EngineX::Application* EngineX::CreateApplication()
{
    return new Sandbox();
}
