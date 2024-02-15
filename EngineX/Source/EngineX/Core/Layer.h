#pragma once

#include "Core.h"
#include "EngineX/Events/Event.h"

namespace EngineX
{
    class Layer
    {
    public:
        // Default layer name .. Names are primarly for debugging purpose 
        Layer(const std::string& debugName = "Layer");
        virtual ~Layer();
        
        /**
         * \brief Called when pushing a layer onto the layer-stack
         */
        virtual void OnAttach() {}

        /**
         * \brief Called when removing a layer from the layer-stack
         */
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& e) {}
        virtual void OnImGuiRender() {}

        inline const std::string& GetName() const {return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}
