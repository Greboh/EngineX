#pragma once
#include "KeyCodes.h"
#include "MouseCodes.h"

namespace EngineX
{
    // TODO: Reserach if it makes sense to instead of having a lot of different methods there could just be two which returns a struct of KeyData or MouseData
    // ^ This could make it easier to enable key mods
    
    class InputManager
    {
    public:
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}
