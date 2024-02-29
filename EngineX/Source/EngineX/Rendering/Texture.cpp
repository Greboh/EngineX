#include "enxpch.h"
#include "Texture.h"

#include "Render.h"
#include "RenderAPI.h"
#include "EngineX/Core/Core.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace EngineX
{
    Ref<Texture2D> Texture2D::Create(const std::string& filePath)
    {
        switch (Render::GetAPI())
        {
            case RenderAPI::API::NONE: ENX_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
            case RenderAPI::API::OPENGL: return CreateRef<OpenGLTexture>(filePath);
            case RenderAPI::API::VULKAN: ENX_ENGINE_ASSERT(false, "RenderAPI::VULKAN is currently not supported!");
            case RenderAPI::API::DIRECT3D: ENX_ENGINE_ASSERT(false, "RenderAPI::DIRECT3D is currently not supported!");
            case RenderAPI::API::METAL: ENX_ENGINE_ASSERT(false, "RenderAPI::METAL is currently not supported!");
        }

        ENX_ENGINE_ASSERT(false, "Unknown RenderAPI")
        return nullptr;
    }
}
