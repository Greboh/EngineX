#include "EditorLayer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "EngineX/Core/Application.h"
#include "EngineX/Core/InputManager.h"
#include "EngineX/Core/Rendering/RenderCommand.h"
#include "EngineX/Core/Rendering/Render.h"

#include "glm/glm.hpp"

static bool s_showConsole = true;
static bool s_DebuggerPanel = true;
static bool s_ObjectManipulationPanel = true;
static bool s_showDemo = false;

EditorLayer::EditorLayer() : Layer("EditorLayer")
{
    // -------------------------------Rendering A Triangle-------------------------------

    m_VertexArray.reset(EngineX::VertexArray::Create());

    // Define the vertex data
    // This array contains the vertex positions for a triangle.
    // Each vertex has three components: x, y, and z coordinates.
    // It also contains rgba values for a given indices
    // float vertices[4 * 7] =
    // {
    //     -0.5f, -0.5f, 0.0f,       0.8f,0.0f, 0.4f, 1.0f, // Lower left with Redish color
    //     0.5f, -0.5f, 0.0f,        0.4f, 0.8f, 0.0f, 1.0f, // Lower right with greenish color
    //     0.5f, 0.5f, 0.0f,         0.0f, 0.4f, 0.8f, 1.0f, // Upper right with blueish color
    //     -0.5f, 0.5f, 0.0f,        0.5f, 0.1f, 0.4f, 1.0f // Upper left with blueish color
    // };
    float vertices[4 * 5] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Lower left with Redish color
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Lower right with greenish color
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // Upper right with blueish color
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f // Upper left with blueish color
    };


    m_VertexBuffer.reset(EngineX::VertexBuffer::Create(sizeof(vertices), vertices));

    // m_VertexBuffer->SetLayout
    // ({
    //     {EngineX::ShaderDataType::Float3, "a_Position"},
    //     {EngineX::ShaderDataType::Float4, "a_Color"},
    // });

    m_VertexBuffer->SetLayout
    ({
        {EngineX::ShaderDataType::Float3, "a_Position"},
        {EngineX::ShaderDataType::Float2, "a_Texture"},
    });

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    // Define the indices for indexed rendering
    // These indices specify the order in which vertices are used to form primitives (e.g., triangles).
    uint32_t indices[6] =
    {
        0, 1, 2,
        2, 3, 0
    };

    m_IndexBuffer.reset(EngineX::IndexBuffer::Create(std::size(indices), indices));

    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_Shader.reset(EngineX::Shader::Create(std::string(ASSETS_DIR) + "Shaders/3DTextureShader.glsl"));
    // m_Shader.reset(EngineX::Shader::Create(std::string(ASSETS_DIR) + "Shaders/3DShader.glsl"));

    m_Model = rotate(m_Model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    m_Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_UseVSync = EngineX::Application::GetInstance().GetWindow().IsVSync();
}

void EditorLayer::OnAttach()
{
    EngineX::Log::AttachImGuiConsoleSink(m_ImGuiConsole);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnRender()
{
    EngineX::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    EngineX::RenderCommand::Clear();

    m_View = translate(glm::mat4(1.0f), m_TransformCoords);
    
    EngineX::Render::BeginScene();
    {
        m_Shader->Bind();

        m_Shader->UploadUniform("u_Model", m_Model);
        m_Shader->UploadUniform("u_View", m_View);
        m_Shader->UploadUniform("u_Projection", m_Projection);

        EngineX::Render::Submit(m_VertexArray);
    }
    EngineX::Render::EndScene();
}

void EditorLayer::OnUpdate()
{
    if (EngineX::InputManager::IsKeyPressed(EngineX::Key::Tab))
    {
        ENX_TRACE("The tab key is pressed (poll)!");
        ENX_INFO("The tab key is pressed (poll)!");
        ENX_WARN("The tab key is pressed (poll)!");
        ENX_ERROR("The tab key is pressed (poll)!");
        ENX_ENGINE_INFO("The tab key is pressed (poll)!");
    }
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

void EditorLayer::ObjectManipulationPanel(bool* open)
{
    if (!ImGui::Begin("Object Manipulation Panel", open))
    {
        ImGui::End();
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (ImGui::TreeNode("Transform"))
    {
        // Position
       { 
           ImGui::CustomSpacing(ImVec2(0.0f, 5.0f)); // Add a separator for visual clarity

           ImGui::Columns(4);

           ImGui::Text("Position:");
           ImGui::NextColumn();
        
           // Display labels for XYZ coordinates
           ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
           ImGui::Text("X:");
           ImGui::SameLine(); // Move next item to the same line
           ImGui::DragFloat("##X", &m_TransformCoords.x, 0.1f); // Drag input for X coordinate
           ImGui::NextColumn();

           ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
           ImGui::Text("Y:");
           ImGui::SameLine(); // Move next item to the same line
           ImGui::DragFloat("##Y", &m_TransformCoords.y, 0.1f); // Drag input for X coordinate
           ImGui::NextColumn();

           ImGui::AlignTextToFramePadding(); // Align text to top of frame padding
           ImGui::Text("Z:");
           ImGui::SameLine(); // Move next item to the same line
           ImGui::DragFloat("##Z", &m_TransformCoords.z, 0.1f); // Drag input for X coordinate

           ImGui::Columns();
       }

        ImGui::TreePop();
    }

    ImGui::End();
}


void EditorLayer::OnEvent(EngineX::Event& e)
{
}
