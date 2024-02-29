#pragma once

#include "EngineX/Core/Layer.h"

namespace EngineX
{
    
    /**
     * \brief Responsible for initializing all ImGui Layers and handling ImGui frames etc in the background
     */
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;
        void Begin();
        void End();
    };
}
