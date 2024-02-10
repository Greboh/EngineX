#include "enxpch.h"

#include "ImGuiLayer.h"
#include "imgui.h"
#include "EngineX/Core/Application.h"

#include "EngineX/Core/KeyCodes.h"
#include "EngineX/Events/KeyEvent.h"
#include "EngineX/Events/MouseEvent.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

namespace EngineX
{
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer() = default;

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags = ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags = ImGuiBackendFlags_HasSetMousePos;

        io.KeyMap[ImGuiKey_Tab] = Key::Tab;
        io.KeyMap[ImGuiKey_Enter] = Key::Enter;
        io.KeyMap[ImGuiKey_LeftArrow] = Key::Left;
        io.KeyMap[ImGuiKey_RightArrow] = Key::Right;
        io.KeyMap[ImGuiKey_UpArrow] = Key::Up;
        io.KeyMap[ImGuiKey_DownArrow] = Key::Down;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
    }

    void ImGuiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();

        // Set window size for ImGui
        const Application& app = Application::Get();
        const Window& window = app.GetWindow();
        io.DisplaySize = ImVec2(static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()));

        // Set Delta time for ImGui
        const auto time = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_DeltaTime > 0.0f ? time - m_DeltaTime : 1.0f / 60.0f;

        // Start a new ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // Temporary implementation to show a demo window .. 
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(ENX_BIND_EVENT_FN(OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(ENX_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(ENX_BIND_EVENT_FN(OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(ENX_BIND_EVENT_FN(OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(ENX_BIND_EVENT_FN(OnKeyPressedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(ENX_BIND_EVENT_FN(OnKeyTypedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(ENX_BIND_EVENT_FN(OnKeyReleasedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(ENX_BIND_EVENT_FN(OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(e.GetMouseButton(), true);

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(e.GetMouseButton(), false);

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(e.GetX(), e.GetY());
        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[Key::LeftControl] || io.KeysDown[Key::RightControl];
        io.KeyShift = io.KeysDown[Key::LeftShift] || io.KeysDown[Key::RightShift];
        io.KeyAlt = io.KeysDown[Key::LeftAlt] || io.KeysDown[Key::RightAlt];
        io.KeySuper = io.KeysDown[Key::LeftOS] || io.KeysDown[Key::RightOS];
        
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        const int keycode = e.GetKeyCode();
        
        if (keycode && keycode < 0x10000)
        {
            io.AddInputCharacter(e.GetKeyCode());            
        }
        
        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
        io.DisplayFramebufferScale = ImVec2(1, 1);
        glViewport(0, 0, static_cast<int>(e.GetWidth()), static_cast<int>(e.GetHeight()));
        return false;
    }
}
