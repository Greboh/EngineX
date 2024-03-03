#pragma once
#include <glm/vec4.hpp>

#include "EngineX/Core/Base.h"
#include "VertexArray.h"

namespace EngineX
{
    class RenderAPI
    {
    public:
        enum class API
        {
            NONE = 0,     // Headless
            OPENGL,      // Windows & Linux
            VULKAN,     // Windows & Linux & Android & Switch etc.. 
            DIRECT3D,  // Windows
            METAL     // Mac
        };

        enum BufferClearFlags
        {
            NONE = 0,
            COLOR_BUFFER = 1,
            DEPT_BUFFER = 2,
        };
        
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear (const BufferClearFlags flags) const = 0 ;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray);

        static inline API GetAPI() { return s_RenderAPI; }
    private:
        static API s_RenderAPI;
    };
}
