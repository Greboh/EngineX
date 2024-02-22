#include "EditorLayer.h"

#include "EngineX/Core/InputManager.h"
#include "EngineX/Core/Rendering/RenderCommand.h"
#include "EngineX/Core/Rendering/Render.h"

#include "glm/glm.hpp"

EditorLayer::EditorLayer() : Layer("EditorLayer")
{
    // -------------------------------Rendering A Triangle-------------------------------
    
    m_VertexArray.reset(EngineX::VertexArray::Create());

    // Define the vertex data
    // This array contains the vertex positions for a triangle.
    // Each vertex has three components: x, y, and z coordinates.
    // It also contains rgba values for a given indices
    float vertices[3 * 7] =
    {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.4f, 1.0f, // Lower left with Redish color
        0.5f, -0.5f, 0.0f, 0.4f, 0.8f, 0.0f, 1.0f, // Lower right with greenish color
        0.0f, 0.5f, 0.0f, 0.0f, 0.4f, 0.8f, 1.0f // Upper middle with blueish color
    };

    m_VertexBuffer.reset(EngineX::VertexBuffer::Create(sizeof(vertices), vertices));

    m_VertexBuffer->SetLayout
    ({
        {EngineX::ShaderDataType::Float3, "a_Position"},
        {EngineX::ShaderDataType::Float4, "a_Color"},
    });


    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    // Define the indices for indexed rendering
    // These indices specify the order in which vertices are used to form primitives (e.g., triangles).
    uint32_t indices[3] = {0, 1, 2};

    m_IndexBuffer.reset(EngineX::IndexBuffer::Create(std::size(indices), indices));

    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    // --------------------------------------------------------------------------------------------

    // -----------------------------------Shading the triangle-------------------------------------

    std::string vertexSource = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }    
        )";

    std::string fragmentSource = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = v_Color;
            }    
        )";

    m_Shader = EngineX::CreateScope<EngineX::Shader>(vertexSource, fragmentSource);
}

void EditorLayer::OnAttach()
{
    // First, create a shared pointer to an instance of ImGuiConsole using make_shared
    EngineX::Log::AttachImGuiConsoleSink(m_ImGuiConsole);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnRender()
{
    EngineX::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    EngineX::RenderCommand::Clear();

    EngineX::Render::BeginScene();
    {
        m_Shader->Bind();
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

    static bool showConsole = true;
    m_ImGuiConsole->Draw("Console", &showConsole);

    static bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);
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


    ImGui::End();
}

void EditorLayer::OnEvent(EngineX::Event& e)
{
}
