#include "EngineX.h"

class Sandbox : public EngineX::Application
{
public:
    Sandbox() = default;

    ~Sandbox() override = default;
};


EngineX::Application* EngineX::CreateApplication()
{
    return new Sandbox();
}
