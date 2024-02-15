#pragma once

#include "EngineX/Core/Layer.h"

namespace EngineX
{
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

    private:
        float m_DeltaTime;
    };
}
