#pragma once
#include "EngineX/Rendering/RenderAPI.h"
#include "glad/glad.h"

namespace EngineX
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear(const BufferClearFlags flags) const override;
        void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

    private:
        const std::unordered_map<BufferClearFlags, GLbitfield> m_flagMap =
        {
            {NONE, GL_NONE},
            {COLOR_BUFFER, GL_COLOR_BUFFER_BIT},
            {DEPT_BUFFER, GL_DEPTH_BUFFER_BIT}
        };
    };
}
