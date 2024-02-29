#pragma once

#include "RenderAPI.h"

namespace EngineX
{
    class RenderCommand
    {
    public:
        
        inline static void  SetClearColor(const glm::vec4& color)
        {
            s_RenderAPI->SetClearColor(color);
        }
        
        inline static void Clear(const RenderAPI::BufferClearFlags flags)
        {
            s_RenderAPI->Clear(flags);
        }
        
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RenderAPI->DrawIndexed(vertexArray);
        }

    private:
        static RenderAPI* s_RenderAPI;
    };
}
