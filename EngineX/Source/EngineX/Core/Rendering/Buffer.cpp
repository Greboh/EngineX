#include "enxpch.h"
#include "Buffer.h"

#include "Render.h"
#include "EngineX/Core/Core.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace EngineX
{
    VertexBuffer* VertexBuffer::Create(uint32_t size, float* vertices)
    {
        switch (Render::GetAPI())
        {
            case RenderAPI::API::NONE:               ENX_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!")
               
            case RenderAPI::API::OPENGL:             return new OpenGLVertexBuffer(size, vertices);
        }

        ENX_ENGINE_ASSERT(false, "Unknown RenderAPI")
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t size, uint32_t* indices)
    {
        switch (Render::GetAPI())
        {
            case RenderAPI::API::NONE:               ENX_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!")
               
            case RenderAPI::API::OPENGL:             return new OpenGLIndexBuffer(size, indices);
        }

        return nullptr;
    }
}
