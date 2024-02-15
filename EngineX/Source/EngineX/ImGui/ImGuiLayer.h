#pragma once

#include "EngineX/Core/Layer.h"

namespace EngineX
{
    struct ImGuiConsole;

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
        std::shared_ptr<ImGuiConsole> m_MyImGuiConsole;
    };
}
