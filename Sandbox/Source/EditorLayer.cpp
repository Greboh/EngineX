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
    ImGui::Begin("Viewport");
    ImGui::Text("Hello Viewport!");
    
    static bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);
    
    m_ImGuiConsole->Draw("Console");
    ImGui::End();
}

void EditorLayer::OnEvent(EngineX::Event& e)
{
        
}    