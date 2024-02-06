#include "enxpch.h"
#include "Window_Windows.h"

namespace EngineX
{
    // Static to make sure glfw is only initialized once per life time!
    static bool s_GLFWInitialized = false;
    
    Window* Window::Create(const WindowBaseVars& vars)
    {
        return new Window_Windows(vars);
    }
    
    Window_Windows::Window_Windows(const WindowBaseVars& vars)
    {
        Window_Windows::Init(vars);
    }

    Window_Windows::~Window_Windows()
    {
        Window_Windows::Shutdown();
    }

    void Window_Windows::Init(const WindowBaseVars& vars)
    {
        m_Data.Name = vars.Name;
        m_Data.Width = vars.Width;
        m_Data.Height = vars.Height;

        ENX_ENGINE_INFO("Creating window > Name: {0} W{1} : H{2}", vars.Name, vars.Width, vars.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();

            // This does not execute in release versions.
            ENX_ENGINE_ASSERT(success, "Could not initialize GLFW!")

            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow
        (
            static_cast<int>(vars.Width),
            static_cast<int>(vars.Height),
            vars.Name.c_str(),
            nullptr,
            nullptr
        );
        glfwMakeContextCurrent(m_Window);
        // Use ampersand to pass the memory address of m_Data to associate it with the GLFW window.
        // This allows us to store custom data (m_Data) with the GLFW window for later retrieval.
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void Window_Windows::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void Window_Windows::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void Window_Windows::SetVSync(bool enabled)
    {
        const int useVSync = enabled ? 1 : 0;

        glfwSwapInterval(useVSync);

        m_Data.VSync = enabled;
    }

    bool Window_Windows::IsVSync() const
    {
        return m_Data.VSync;
    }
}
