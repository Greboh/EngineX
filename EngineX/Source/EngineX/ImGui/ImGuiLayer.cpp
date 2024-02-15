#include "enxpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "ImGuiConsoleSink.h"
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

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetBaseWindow());

        // Setup Platform / Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        // First, create a shared pointer to an instance of ImGuiConsole using make_shared
        m_MyImGuiConsole = std::make_shared<ImGuiConsole>();

        // Then, create an instance of ImGuiConsoleSink using the shared pointer to myImGuiConsole
        auto const sinkInstance = std::make_shared<ImGuiConsoleSink>(m_MyImGuiConsole);
        sinkInstance->set_pattern("%^[%T] %n: %v%$");

        spdlog::get("Application")->sinks().push_back(sinkInstance);
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnImGuiRender()
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        m_MyImGuiConsole->Draw("Console");
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
        const Application& app = Application::Get();

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
