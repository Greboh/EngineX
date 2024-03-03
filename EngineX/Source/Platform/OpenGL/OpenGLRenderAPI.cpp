#include "enxpch.h"
#include "OpenGLRenderAPI.h"

namespace EngineX
{
    void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderAPI::Clear(const BufferClearFlags flags) const
    {
        GLbitfield mask = 0;

        for (auto& pair : m_flagMap)
        {
            if (static_cast<int>(flags) & static_cast<int>(pair.first))
            {
                mask |= pair.second;
            }
        }
        
        glClear(mask);
    }

    void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        glDrawElements
        (
            GL_TRIANGLES,
            vertexArray->GetIndexBuffer()->GetCount(),
            GL_UNSIGNED_INT,
            nullptr
        );
        
    }
}
