#include "enxpch.h"
#include "RenderAPI.h"

namespace EngineX
{
    RenderAPI::API RenderAPI::s_RenderAPI = API::OPENGL;

    void RenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
    }
}
