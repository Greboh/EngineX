#pragma once
#include "EngineX/Core/Rendering/RenderAPI.h"

namespace EngineX
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
    
}
