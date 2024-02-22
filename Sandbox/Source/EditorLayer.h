#pragma once
#include "EngineX/Core/Layer.h"
#include "EngineX/Core/Rendering/Buffer.h"
#include "EngineX/Core/Rendering/Shader.h"
#include "EngineX/Core/Rendering/VertexArray.h"

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
    void OnRender() override;

private:
    static void MakeDockSpace();

    EngineX::Ref<EngineX::ImGuiConsole> m_ImGuiConsole = EngineX::CreateRef<EngineX::ImGuiConsole>();
    EngineX::Ref<EngineX::Shader> m_Shader;
    EngineX::Ref<EngineX::VertexBuffer> m_VertexBuffer;
    EngineX::Ref<EngineX::IndexBuffer> m_IndexBuffer;
    EngineX::Ref<EngineX::VertexArray> m_VertexArray;
};
