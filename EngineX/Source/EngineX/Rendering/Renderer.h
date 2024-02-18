#pragma once

namespace EngineX
{
    enum class RenderAPI
    {
        NONE = 0,     // Headless
        OPENGL,      // Windows & Linux
        VULKAN,     // Windows & Linux & Android & Switch etc.. 
        DIRECT3D,  // Windows
        METAL     // Mac
    };

    class Renderer
    {
    public:
        inline static void SetAPI(const RenderAPI api = RenderAPI::OPENGL) { s_RenderAPI = api; }
        inline static RenderAPI GetAPI() { return s_RenderAPI; }

    private:
        static RenderAPI s_RenderAPI;
    };
}
