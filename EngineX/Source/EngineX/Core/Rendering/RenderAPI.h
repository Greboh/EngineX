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

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray);

        static inline API GetAPI() { return s_RenderAPI; }
    private:
        static API s_RenderAPI;
    };
}
