#pragma once
#include "EngineX/Core/Layer.h"
#include "EngineX/Core/Timestep.h"
#include "EngineX/Rendering/Buffer.h"
#include "EngineX/Rendering/Camera.h"
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
    void OnUpdate(EngineX::Timestep deltaTime) override;
    void OnEvent(EngineX::Event& e) override;
    void OnImGuiRender() override;

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

    EngineX::Camera m_Camera;

    glm::mat4 m_ModelTransform = glm::mat4(1.0f);
    glm::vec3 m_ModelTransformPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_ModelTransformRotationRate = glm::vec3(40.0f, 40.0f, 0.0f);
    glm::vec3 m_ModelTransformScale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec4 m_ModelAdditionalColor = {0.25f, 0.25f, 0.25f, 1.0f};

    EngineX::RenderAPI::BufferClearFlags m_ClearFlags;
};
