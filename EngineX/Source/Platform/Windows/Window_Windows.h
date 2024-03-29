﻿#pragma once

#include "EngineX/Core/Window.h"

#include <GLFW/glfw3.h>

#include "EngineX/Rendering/RenderingContext.h"

namespace EngineX
{
    class Window_Windows : public Window
    {
    public:
        Window_Windows(const WindowBaseVars& vars);
        virtual ~Window_Windows() override;

        inline virtual void* GetBaseWindow() const override { return m_Window; }
        
        void OnUpdate() override;

        inline unsigned GetWidth() const override { return m_Data.Width; }
        inline unsigned GetHeight() const override { return m_Data.Height; }

        inline void SetEventCallBack(const EventCallBackFn& callback) override {m_Data.EventCallback = callback;}
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

    private:
        
        /**
         * \brief Called when Initializing a Windows window
         * \param vars The base window Variables
         */
        virtual void Init(const WindowBaseVars& vars);
        
        /**
         * \brief Called when shutting down a Windows window .. Useful for cleaning up!
         */
        virtual void Shutdown();

        GLFWwindow* m_Window;

        RenderingContext* m_RenderingContext;

        // Store all data that GLFW might need ..
        struct GLFW_WindowData
        {
            std::string Name;
            unsigned int Width, Height;
            bool VSync;

            EventCallBackFn EventCallback;
        };

        GLFW_WindowData m_Data;
    };
}
