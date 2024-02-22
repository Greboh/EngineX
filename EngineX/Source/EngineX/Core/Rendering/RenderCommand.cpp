#include "enxpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace EngineX
{
    RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI;
}