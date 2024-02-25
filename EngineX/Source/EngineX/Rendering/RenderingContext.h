#pragma once
namespace EngineX
{
    class RenderingContext
    {
    public:
        RenderingContext() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}
