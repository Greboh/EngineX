#pragma once
#include "EngineX/Core/Rendering/RenderingContext.h"

struct GLFWwindow;

namespace EngineX
{
    class OpenGLRenderingContext : public RenderingContext
    {
    public:
        OpenGLRenderingContext(GLFWwindow* windowHandle);
        
        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}
