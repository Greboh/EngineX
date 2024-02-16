#include "enxpch.h"
#include "OpenGLRenderingContext.h"

#include "EngineX/Core/Core.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace EngineX
{
    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        ENX_ENGINE_ASSERT(windowHandle,"WindowHandle is null!");
    }

    void OpenGLRenderingContext::Init()
    {
            glfwMakeContextCurrent(m_WindowHandle);
    
            // Initialize glad .. We use reinterpret_cast because we have to cast the pointer to another type
            const int success = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
            ENX_ENGINE_ASSERT(success, "Could not initialize GLFW!")
            
            ENX_ENGINE_INFO("OpenGL Renderer:");
            ENX_ENGINE_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
            ENX_ENGINE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
            ENX_ENGINE_INFO("Version: {0}", (const char*)glGetString(GL_VERSION));
    }

    void OpenGLRenderingContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}