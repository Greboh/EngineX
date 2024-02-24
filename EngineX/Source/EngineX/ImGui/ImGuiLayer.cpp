#include "enxpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "EngineX/Core/Application.h"

#include "GLFW/glfw3.h"

namespace EngineX
{
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer() = default;

    void ImGuiLayer::OnAttach()
    {
        // Makes sure there is no version mismatch
        IMGUI_CHECKVERSION();
        
        // Setup ImGui context
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // Setup ImGui Style
        ImGui::StyleColorsDark();

        // Tweak WindowRounding/WindowBackground when viewports are enabled
        // so platform windows can look identical to regular ones
        ImGuiStyle& style = ImGui::GetStyle();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::GetInstance();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetBaseWindow());
        
        // Setup Platform / Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnImGuiRender()
    {
        
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        const Application& app = Application::GetInstance();

        const auto[x, y] = ENX_GET_SCREENSIZE(app.GetWindow());        
        io.DisplaySize = ImVec2(static_cast<float>(x), static_cast<float>(y));

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // This makes sure we properly clean up viewports
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
    }
}
