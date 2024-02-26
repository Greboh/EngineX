#include "EditorLayer.h"

#include <backends/imgui_impl_opengl3_loader.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "EngineX/Core/Application.h"
#include "EngineX/Core/InputManager.h"
#include "EngineX/Rendering/RenderCommand.h"


static bool s_showConsole = true;
static bool s_DebuggerPanel = true;
static bool s_ObjectManipulationPanel = true;
static bool s_showDemo = false;

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_Camera(90.0f, 1980.0f / 1080.0f, 0.1f, 100.0f)
{
    m_VertexArray.reset(EngineX::VertexArray::Create());

    float vertices[24 * 9] =
    {
        // front
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 0
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 1
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 2
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 3

        // top
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 4
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 5
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 6
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 7

        // left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 8
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 9
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 10
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 11

        // right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 12
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 13
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 14
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.2f, 0.2f, 0.6f, 1.0f, // 15

        // back
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 16
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 17
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 18
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.6f, 0.2f, 0.2f, 1.0f, // 19

        // // bottom
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 20
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 21
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 22
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.6f, 0.2f, 1.0f, // 23
    };

    m_VertexBuffer.reset(EngineX::VertexBuffer::Create(sizeof(vertices), vertices));

    m_VertexBuffer->SetLayout
    ({
        {EngineX::ShaderDataType::Float3, "a_Position"},
        {EngineX::ShaderDataType::Float2, "a_Texture"},
        {EngineX::ShaderDataType::Float4, "a_Color"},
    });

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    // Define the indices for indexed rendering
    // These indices specify the order in which vertices are used to form primitives (e.g., triangles).
    uint32_t indices[36] =
    {
        // front
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle

        // top
        4, 5, 6, // first triangle
        6, 7, 4, // second triangle

        // left
        8, 9, 10, // first triangle
        10, 11, 8, // second triangle

        // right
        14, 13, 12, // first triangle
        12, 15, 14, // second triangle

        // back
        18, 17, 16, // first triangle
        16, 19, 18, // second triangle

        // bottom
        20, 21, 22, // first triangle
        22, 23, 20 //  second triangle
    };

    m_IndexBuffer.reset(EngineX::IndexBuffer::Create(std::size(indices), indices));

    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
}

void EditorLayer::OnAttach()
{
    EngineX::Log::AttachImGuiConsoleSink(m_ImGuiConsole);
    glEnable(GL_DEPTH_TEST);

    m_UseVSync = EngineX::Application::GetInstance().GetWindow().IsVSync();

    m_Shader.reset(EngineX::Shader::Create(std::string(ASSETS_DIR) + "Shaders/3DTextureShader.glsl"));

    m_Texture = EngineX::Texture2D::Create(std::string(ASSETS_DIR) + "Textures/Checkerboard.png");

    m_ClearFlags = static_cast<EngineX::RenderAPI::BufferClearFlags>(
        EngineX::RenderAPI::BufferClearFlags::COLOR_BUFFER |
        EngineX::RenderAPI::BufferClearFlags::DEPT_BUFFER);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnRender()
{
    EngineX::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    EngineX::RenderCommand::Clear(m_ClearFlags);

    const auto modelRotationMatrix =
        rotate(glm::mat4(1.0f), glm::radians(m_ModelTransformRotationRate.x), glm::vec3(1.0f, 0.0f, 0.0f))
        * rotate(glm::mat4(1.0f), glm::radians(m_ModelTransformRotationRate.y), glm::vec3(0.0f, 1.0f, 0.0f))
        * rotate(glm::mat4(1.0f), glm::radians(m_ModelTransformRotationRate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    const auto modelScaleMatrix = scale(glm::mat4(1.0f), m_ModelTransformScale);

    m_ModelTransform = translate(glm::mat4(1), m_ModelTransformPosition) * modelRotationMatrix * modelScaleMatrix;

    auto modelViewProjection = m_Camera.GetViewProjectionMatrix() * m_ModelTransform;
    
    EngineX::Render::BeginScene(m_Camera);
    {
        m_Shader->Bind();
        m_Texture->Bind();
        m_Shader->UploadUniform("u_Texture", 0);
        m_Shader->UploadUniform("u_ExtraColor", m_ModelAdditionalColor);
        m_Shader->UploadUniform("u_ModelViewProjection", modelViewProjection);

        EngineX::Render::Submit(m_VertexArray, m_Shader);
    }
    EngineX::Render::EndScene();
}

void EditorLayer::OnUpdate()
{
    auto movementModifiers = glm::vec3(0.0f, 0.0f, 0.0f);

    if (EngineX::InputManager::IsMouseButtonPressed(EngineX::Mouse::Button1))
    {
        if (EngineX::InputManager::IsKeyPressed(EngineX::Key::A))
        {
            movementModifiers.x += 0.1f;
        }
        if (EngineX::InputManager::IsKeyPressed(EngineX::Key::D))
        {
            movementModifiers.x -= 0.1f;
        }
        if (EngineX::InputManager::IsKeyPressed(EngineX::Key::E))
        {
            movementModifiers.y -= 0.1f;
        }
        if (EngineX::InputManager::IsKeyPressed(EngineX::Key::Q))
        {
            movementModifiers.y += 0.1f;
        }
        if (EngineX::InputManager::IsKeyPressed(EngineX::Key::W))
        {
            movementModifiers.z += 0.1f;
        }
        if (EngineX::InputManager::IsKeyPressed(EngineX::Key::S))
        {
            movementModifiers.z -= 0.1f;
        }
    }

    m_Camera.SetPosition(m_Camera.GetPosition() - movementModifiers);
}

void EditorLayer::OnImGuiRender()
{
    MakeDockSpace();

    // Panel checking
    if (s_DebuggerPanel)
    {
        DebuggerPanel(&s_DebuggerPanel);
    }
    if (s_ObjectManipulationPanel)
    {
        ObjectManipulationPanel(&s_ObjectManipulationPanel);
    }
    if (s_showConsole)
    {
        m_ImGuiConsole->Draw("Console", &s_showConsole);
    }
    if (s_showDemo)
    {
        ImGui::ShowDemoWindow(&s_showDemo);
    }
}

void EditorLayer::MakeDockSpace()
{
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Settings"))
        {
            // TODO: Maybe make these into flags for default panels? .. To avoid bools and make the toggle prettier
            if (ImGui::MenuItem(s_showConsole ? "Hide Console Panel" : "Show Console Panel", nullptr, nullptr, true))
            {
                s_showConsole = !s_showConsole;
            }
            if (ImGui::MenuItem(s_DebuggerPanel ? "Hide Debugger Panel" : "Show Debugger Panel", nullptr, nullptr,
                                true))
            {
                s_DebuggerPanel = !s_DebuggerPanel;
            }
            if (ImGui::MenuItem(s_showDemo ? "Hide Demo Panel" : "Show Demo Panel", nullptr, nullptr, true))
            {
                s_showDemo = !s_showDemo;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Manipulation"))
        {
            if (ImGui::MenuItem(
                s_ObjectManipulationPanel ? "Hide Objet Manipulation Panel" : "Show Objet Manipulation Panel", nullptr,
                nullptr, true))
            {
                s_ObjectManipulationPanel = !s_ObjectManipulationPanel;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void EditorLayer::DebuggerPanel(bool* open)
{
    if (!ImGui::Begin("Debugger Panel", open))
    {
        ImGui::End();
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Close Debugger"))
            *open = false;
        ImGui::EndPopup();
    }

    ImGui::SeparatorText("Stats");
    ImGui::Text("Editor average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    ImGui::CustomSpacing(ImVec2(0.0f, 10.0f));

    if (ImGui::CollapsingHeader("Settings"))
    {
        if (ImGui::TreeNode("Engine"))
        {
            ImGui::SeparatorText("General");

            if (ImGui::Checkbox("Use VSync", &m_UseVSync))
            {
                EngineX::Application::GetInstance().GetWindow().SetVSync(m_UseVSync);
            }

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//TODO: This whole panel should be reworked once its possible to click on a scene object and get its data and / or when i start rendering multiple objects ..
void EditorLayer::ObjectManipulationPanel(bool* open)
{
    if (!ImGui::Begin("Object Manipulation Panel", open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginChild("Inspector", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

        ImGui::CustomSpacing(ImVec2(0.0f, 5.0f));

        if (ImGui::TreeNode("Object Properties"))
        {
            if (ImGui::TreeNode("Transform"))
            {
                if (ImGui::Button("Reset"))
                {
                    m_ModelTransformPosition = {0.0f, 0.0f, 0.0f};
                    m_ModelTransformRotationRate = {0.0f, 0.0f, 0.0f};
                    m_ModelTransformScale = {1.0f, 1.0f, 1.0f};
                    m_ModelAdditionalColor = {0.25f, 0.25f, 0.25f, 1.0f};
                }

                // Position
                {
                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f)); // Add a separator for visual clarity

                    ImGui::Columns(4);

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Position:");
                    ImGui::NextColumn();

                    // Display labels for XYZ coordinates
                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("X:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##PositionX", &m_ModelTransformPosition.x, 0.1f); // Drag input for X coordinate
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Y:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##PositionY", &m_ModelTransformPosition.y, 0.1f); // Drag input for X coordinate
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Z:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##PositionZ", &m_ModelTransformPosition.z, 0.1f); // Drag input for X coordinate

                    ImGui::Columns();
                }

                // Rotation
                {
                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f)); // Add a separator for visual clarity

                    ImGui::Columns(4);

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Rotation:");
                    ImGui::NextColumn();

                    // Display labels for XYZ coordinates
                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("X:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##RotationX", &m_ModelTransformRotationRate.x, 1.0f, -360.0f, 360.0f);
                    // Drag input for X coordinate
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Y:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##RotationY", &m_ModelTransformRotationRate.y, 1.0f, -360.0f, 360.0f);
                    // Drag input for X coordinate
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Z:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##RotationZ", &m_ModelTransformRotationRate.z, 1.0f, -360.0f, 360.0f);
                    // Drag input for X coordinate

                    ImGui::Columns();
                }

                // Scale
                {
                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f)); // Add a separator for visual clarity

                    ImGui::Columns(4);

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Scale:");
                    ImGui::NextColumn();

                    // Display labels for XYZ coordinates
                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("X:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##ScaleX", &m_ModelTransformScale.x, 0.1f, 0.1f, FLT_MAX, "%.1f");
                    // Drag input for X scale
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Y:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##ScaleY", &m_ModelTransformScale.y, 0.1f, 0.1f, FLT_MAX, "%.1f");
                    // Drag input for Y scale
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
                    ImGui::Text("Z:");
                    ImGui::SameLine(); // Move next item to the same line
                    ImGui::DragFloat("##ScaleZ", &m_ModelTransformScale.z, 0.1f, 0.1f, FLT_MAX, "%.1f");
                    // Drag input for Z scale

                    ImGui::Columns();
                }

                // Color
                {
                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f));
                    float color[4] = {
                        m_ModelAdditionalColor.x, m_ModelAdditionalColor.y, m_ModelAdditionalColor.z,
                        m_ModelAdditionalColor.a
                    };

                    ImGui::Text("Additional Color:");

                    ImGui::ColorPicker4("##Color", color);

                    m_ModelAdditionalColor = {color[0], color[1], color[2], color[3]};
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::CustomSpacing(ImVec2(0.0f, 10.0f));

        if (ImGui::TreeNode("Camera Properties"))
        {
            if (ImGui::TreeNode("Settings"))
            {
                // Fov
                {
                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f)); // Add a separator for visual clarity
                    auto cameraFov = m_Camera.GetFOV();

                    ImGui::Text("Field of view (FOV):");
                    ImGui::SliderFloat("##fov", &cameraFov, 0.1f, 90.0f, "%.1f");

                    if (fabs(m_Camera.GetFOV() - cameraFov) > 0.00001f)
                    {
                        m_Camera.SetFOV(cameraFov);
                    }
                }

                // Clipping planes
                {
                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f)); // Add a separator for visual clarity
                    float clippingPlanes[2] = {m_Camera.GetClippingPlanes().x, m_Camera.GetClippingPlanes().y};

                    ImGui::Text("Clipping planes:");
                    ImGui::SliderFloat2("##clippingplanes", clippingPlanes, 0.1f, 100.0f, "%.1f");

                    if (fabs(m_Camera.GetClippingPlanes().x - clippingPlanes[0]) > 0.00001f
                        || fabs(m_Camera.GetClippingPlanes().y - clippingPlanes[1]) > 0.00001f)
                    {
                        m_Camera.SetClippingPlanes(glm::vec2(clippingPlanes[0], clippingPlanes[1]));
                    }
                }

                ImGui::TreePop();
            }

            ImGui::CustomSpacing(ImVec2(0.0f, 5.0f));

            if (ImGui::TreeNode("Transform [READ-ONLY]"))
            {
                // Position
                {
                    auto position = m_Camera.GetPosition();

                    // Add a separator for visual clarity
                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f));

                    ImGui::Columns(4);

                    // Align text to top of frame padding
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Position:");
                    ImGui::NextColumn();


                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("X:");
                    ImGui::SameLine();
                    ImGui::DragFloat("##PositionX", &position.x, 0.1f, FLT_MIN, FLT_MAX, "%.1f",
                                     ImGuiInputTextFlags_ReadOnly); // Drag input for X coordinate
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Y:");
                    ImGui::SameLine();
                    ImGui::DragFloat("##PositionY", &position.y, 0.1f, FLT_MIN, FLT_MAX, "%.1f",
                                     ImGuiInputTextFlags_ReadOnly); // Drag input for X coordinate
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Z:");
                    ImGui::SameLine();
                    ImGui::DragFloat("##PositionZ", &position.z, 0.1f, FLT_MIN, FLT_MAX, "%.1f",
                                     ImGuiInputTextFlags_ReadOnly);

                    ImGui::Columns();
                }

                // Rotation
                {
                    auto rotationAngle = m_Camera.GetRotationAngle();

                    ImGui::CustomSpacing(ImVec2(0.0f, 5.0f)); // Add a separator for visual clarity

                    ImGui::Columns(4);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Rotation:");
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("X:");
                    ImGui::SameLine();
                    ImGui::DragFloat("##RotationX", &rotationAngle.x, 1.0f, 0.0f, 360.0f, "%.0f",
                                     ImGuiInputTextFlags_ReadOnly);
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Y:");
                    ImGui::SameLine();
                    ImGui::DragFloat("##RotationY", &rotationAngle.y, 1.0f, 0.0f, 360.0f, "%.0f",
                                     ImGuiInputTextFlags_ReadOnly);
                    ImGui::NextColumn();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Z:");
                    ImGui::SameLine();
                    ImGui::DragFloat("##RotationZ", &rotationAngle.z, 1.0f, 0.0f, 360.0f, "%.0f",
                                     ImGuiInputTextFlags_ReadOnly);

                    ImGui::Columns();
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::EndChild();
    }

    ImGui::End();
}

void EditorLayer::OnEvent(EngineX::Event& e)
{
}
