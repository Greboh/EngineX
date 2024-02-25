#pragma once
#include "EngineX/Core/Layer.h"
#include "EngineX/Rendering/Buffer.h"
#include "EngineX/Rendering/Shader.h"
#include "EngineX/Rendering/Render.h"
#include "EngineX/Rendering/Texture.h"


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
    void MakeDockSpace();
    
    void DebuggerPanel(bool* open);
    void ObjectManipulationPanel(bool* open);
    bool m_UseVSync;
    
    EngineX::Ref<EngineX::ImGuiConsole> m_ImGuiConsole = EngineX::CreateRef<EngineX::ImGuiConsole>();
    
    EngineX::Ref<EngineX::Shader> m_Shader;
    EngineX::Ref<EngineX::Texture2D> m_Texture;
    
    EngineX::Ref<EngineX::VertexBuffer> m_VertexBuffer;
    EngineX::Ref<EngineX::IndexBuffer> m_IndexBuffer;
    EngineX::Ref<EngineX::VertexArray> m_VertexArray;

    glm::vec3 m_TransformCoords = glm::vec3(0.0f, 0.0f, -5.0f);
    glm::vec3 m_RotationRadians = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 m_RotationMatrix;
    
    glm::mat4 m_Model = glm::mat4(1.0f);
    glm::mat4 m_View = glm::mat4(1.0f);
    glm::mat4 m_Projection;

    EngineX::RenderAPI::BufferClearFlags m_ClearFlags;
};
