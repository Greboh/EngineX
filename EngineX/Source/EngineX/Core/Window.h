#pragma once

#include "enxpch.h"

#include "Core.h"
#include "../Events/Event.h"

namespace EngineX
{
    /**
     * \brief The base information needed to create a window
     */
    struct WindowBaseVars
    {
        std::string Name;
        unsigned int Width;
        unsigned int Height;

        WindowBaseVars
        (
            const std::string& name = "EngineX",
            unsigned int width = 1280,
            unsigned int height = 720
        ): Name(name), Width(width), Height(height)
        {}
    };
    
    class ENGINEX_API Window
    {
    public:
        // Creates a type alias for a function pointer that takes Event& as a parameter
        using EventCallBackFn = std::function<void(Event&)>;
        
        virtual ~Window() {}

        virtual void OnUpdate() = 0;
        
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallBack(const EventCallBackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static Window* Create(const WindowBaseVars & props = WindowBaseVars());
    };
}
