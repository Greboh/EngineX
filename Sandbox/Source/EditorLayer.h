#pragma once
#include "EngineX/Core/Layer.h"
#include "EngineX/Core/Timestep.h"
#include "EngineX/Rendering/RenderAPI.h"
#include "EngineX/Rendering/Buffer.h"
#include "EngineX/Rendering/EditorCamera.h"
#include "EngineX/Rendering/Shader.h"
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

    EngineX::Ref<EngineX::Shader> m_TextureShader;
    EngineX::Ref<EngineX::Texture2D> m_Texture;

    EngineX::Ref<EngineX::VertexBuffer> m_VertexBuffer;
    EngineX::Ref<EngineX::IndexBuffer> m_IndexBuffer;
    EngineX::Ref<EngineX::VertexArray> m_VertexArray;
    EngineX::ShaderLibrary m_ShaderLibrary;
    
    EngineX::EditorCamera m_Camera;

    glm::mat4 m_ModelTransform = glm::mat4(1.0f);
    glm::vec3 m_ModelTransformPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_ModelTransformRotation = glm::vec3(45.0, 45.0f, 0.0);
    glm::vec3 m_ModelTransformScale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec4 m_ModelAdditionalColor = {0.25f, 0.25f, 0.25f, 1.0f};

    EngineX::RenderAPI::BufferClearFlags m_ClearFlags;

    // NOTE: This should be moved to an UI class ..
    void DisplayVec3Control
    (
        const char* label,
        glm::vec3 value,
        float speed = 1.0f,
        float minValue = -FLT_MAX,
        float maxValue = FLT_MAX,
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0
    )
    {
        ImGui::CustomSpacing(ImVec2(0.0f, 5.0f));

        ImGui::Columns(4);

        ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
        ImGui::Text(label);
        ImGui::NextColumn();

        // X
        ImGui::AlignTextToFramePadding();
        ImGui::Text("X:");
        ImGui::SameLine();
        ImGui::DragFloat("##X", &value.x, speed, minValue, maxValue, format, flags);
        ImGui::NextColumn();

        // Y
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Y:");
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &value.y, speed, minValue, maxValue, format, flags);
        ImGui::NextColumn();

        // Z
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Z:");
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &value.z, speed, minValue, maxValue, format, flags);

        ImGui::Columns();
    }

    void DisplayVec3Control
    (
        const char* label,
        const char* xLabel,
        const char* yLabel,
        const char* zLabel,
        glm::vec3 value,
        float speed = 1.0f,
        float minValue = -FLT_MAX,
        float maxValue = FLT_MAX,
        const char* format = "%.3f",
        ImGuiInputTextFlags flags = 0
    )
    {
        ImGui::CustomSpacing(ImVec2(0.0f, 5.0f));

        ImGui::Columns(4);

        ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
        ImGui::Text(label);
        ImGui::NextColumn();

        // X
        ImGui::AlignTextToFramePadding();
        ImGui::Text(xLabel);
        ImGui::SameLine();
        ImGui::DragFloat("##X", &value.x, speed, minValue, maxValue, format, flags);
        ImGui::NextColumn();

        // Y
        ImGui::AlignTextToFramePadding();
        ImGui::Text(yLabel);
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &value.y, speed, minValue, maxValue, format, flags);
        ImGui::NextColumn();

        // Z
        ImGui::AlignTextToFramePadding();
        ImGui::Text(zLabel);
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &value.z, speed, minValue, maxValue, format, flags);

        ImGui::Columns();
    }
};
