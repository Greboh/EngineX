#include "enxpch.h"
#include "Render.h"

#include "RenderCommand.h"

namespace EngineX
{
    bool m_LoggedOnce = false;

    void Render::BeginScene()
    {
        if (m_LoggedOnce)
        {
            return;
        }
        
        ENX_ENGINE_WARN("Render::BeginScene()   is currently not supported!");
    }

    void Render::EndScene()
    {
        if (m_LoggedOnce)
        {
            return;
        }
        
        ENX_ENGINE_WARN("Render::EndScene()     is currently not supported!");
        m_LoggedOnce = true;
    }

    void Render::Submit(const Ref<VertexArray>& vertexArray)
    {
        RenderCommand::DrawIndexed(vertexArray);
    }
}
