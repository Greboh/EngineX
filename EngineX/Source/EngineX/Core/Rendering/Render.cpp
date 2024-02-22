#include "enxpch.h"
#include "Render.h"

#include "RenderCommand.h"

namespace EngineX
{
    void Render::BeginScene()
    {
        ENX_ENGINE_WARN("Render::BeginScene()   is currently not supported!");
    }

    void Render::EndScene()
    {
        ENX_ENGINE_WARN("Render::EndScene()     is currently not supported!");
    }

    void Render::Submit(const Ref<VertexArray>& vertexArray)
    {
        RenderCommand::DrawIndexed(vertexArray);
    }
}
