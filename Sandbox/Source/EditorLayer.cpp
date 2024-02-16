#include "EditorLayer.h"

#include "EngineX/Core/InputManager.h"

EditorLayer::EditorLayer() : Layer("EditorLayer")
{
    
}

void EditorLayer::OnAttach()
{
    // First, create a shared pointer to an instance of ImGuiConsole using make_shared
    EngineX::Log::AttachImGuiConsoleSink(m_ImGuiConsole);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{
    if(EngineX::InputManager::IsKeyPressed(EngineX::Key::Tab))
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
    // Enable docking
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    
    static bool showConsole = true;
    m_ImGuiConsole->Draw("Console", &showConsole);
    
    static bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);
}

void EditorLayer::OnEvent(EngineX::Event& e)
{
        
}

