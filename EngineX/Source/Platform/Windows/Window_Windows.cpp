#include "enxpch.h"
#include "Window_Windows.h"

#include "EngineX/Core/Core.h"
#include "EngineX/Events/ApplicationEvent.h"
#include "EngineX/Events/KeyEvent.h"
#include "EngineX/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLRenderingContext.h"

namespace EngineX
{
    // Static to make sure glfw is only initialized once per engine life time!
    static bool s_GLFWInitialized = false;

    // Callback used to log any errors received from GLFW   
    // TODO: Look into not having this static perhaps? And possibly move it somewhere else.
    static void GLFW_ErrorCallback(int error, const char* description)
    {
        ENX_ENGINE_ERROR("GLFW Error({0}): {1}", error, description);
    }

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
        
        if (!s_GLFWInitialized)
        {
            const int success = glfwInit();

            // This does not execute in release versions.
            ENX_ENGINE_ASSERT(success, "Could not initialize GLFW!")

            // Set error callback
            glfwSetErrorCallback(GLFW_ErrorCallback);

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

        // TODO: This should probably be defined by some property. Since the idea is to support multiple graphics APIS
        m_RenderingContext = new OpenGLRenderingContext(m_Window);
        
        m_RenderingContext->Init();
        
        // Use ampersand to pass the memory address of m_Data to associate it with the GLFW window.
        // This allows us to store custom data (m_Data) within the GLFW window for later retrieval.
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // -------------------------------Setup Event Callbacks for GLFW-------------------------------

        // Window Resize
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            // Get the data we stored earlier in the window
            // First Dereference it to retrieve the pointers value
            // The pointer is void therefore we cast it to our GLFW_WindowData class
            // Lastly we tell it where to find the actual data
            GLFW_WindowData& data = *static_cast<GLFW_WindowData*>(glfwGetWindowUserPointer(window));
            data.Width = width;
            data.Height = height;

            ENX_ENGINE_INFO("Window Resized! {0}:{1}", width, height);
            
            // Make the Event 
            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        // Window Close
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            GLFW_WindowData& data = *static_cast<GLFW_WindowData*>(glfwGetWindowUserPointer(window));

            WindowCloseEvent event;
            data.EventCallback(event);
        });

        // Key Input
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods)
        {
            GLFW_WindowData& data = *static_cast<GLFW_WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
                {
                    KeyPressedEvent event(keycode, 0);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    KeyPressedEvent event(keycode, 1);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(keycode);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            GLFW_WindowData& data = *static_cast<GLFW_WindowData*>(glfwGetWindowUserPointer(window));

            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        // Mouse-Click Event
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            GLFW_WindowData& data = *static_cast<GLFW_WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        // Mouse-Scroll Event
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            GLFW_WindowData& data = *static_cast<GLFW_WindowData*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.EventCallback(event);
        });

        // Mouse-Movement Event
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            GLFW_WindowData& data = *static_cast<GLFW_WindowData*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data.EventCallback(event);
        });

        // --------------------------------------------------------------------------------------------
    }

    void Window_Windows::OnUpdate()
    {
        glfwPollEvents();
        m_RenderingContext->SwapBuffers();
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
