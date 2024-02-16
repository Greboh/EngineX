#pragma once
#include "EngineX/Core/Layer.h"

class EditorLayer : public EngineX::Layer
{
public:
    EditorLayer();

    virtual ~EditorLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(EngineX::Event& e) override;
    void OnImGuiRender() override;

private:
    static void MakeDockSpace();
    
    EngineX::Ref<EngineX::ImGuiConsole> m_ImGuiConsole = EngineX::CreateRef<EngineX::ImGuiConsole>();
};
