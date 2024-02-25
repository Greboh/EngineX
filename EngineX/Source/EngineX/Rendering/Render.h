#pragma once
#include "RenderAPI.h"

namespace EngineX
{
    class Render
    {
    public:
        static void BeginScene();
        static void EndScene();

        static void Submit(const Ref<VertexArray>& vertexArray);
        
        inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
    };
}
