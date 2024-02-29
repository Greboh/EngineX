#include "enxpch.h"
#include "VertexArray.h"

#include "Render.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace EngineX
{
    VertexArray* VertexArray::Create()
    {
        switch (Render::GetAPI())
        {
            case RenderAPI::API::NONE: ENX_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
            case RenderAPI::API::OPENGL: return new OpenGLVertexArray();
            case RenderAPI::API::VULKAN: ENX_ENGINE_ASSERT(false, "RenderAPI::VULKAN is currently not supported!");
            case RenderAPI::API::DIRECT3D: ENX_ENGINE_ASSERT(false, "RenderAPI::DIRECT3D is currently not supported!");
            case RenderAPI::API::METAL: ENX_ENGINE_ASSERT(false, "RenderAPI::METAL is currently not supported!");
        }

        ENX_ENGINE_ASSERT(false, "Unknown RenderAPI")
        return nullptr;
    }
}
