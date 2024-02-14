#include "enxpch.h"
#include "EngineX/Core/InputManager.h"
#include "EngineX/Core/Application.h"
#include <GLFW/glfw3.h>


namespace EngineX
{
    bool InputManager::IsKeyPressed(KeyCode key)
    {
        // In order to call glfw to get a key, we need the glfw window. For we retrieve our BaseWindow
        const auto window = Application::Get().GetWindow().GetBaseWindow();

        // Since this is inside Input_Windows, we know it's for Windows
        // which currently only supports a GLFW Window therefor we can safely cast it.
        // Then retrieve the keyState
        const auto keyState = glfwGetKey(static_cast<GLFWwindow*>(window), key);
        
        return keyState == GLFW_PRESS;
    }

    bool InputManager::IsMouseButtonPressed(MouseCode button)
    {
        const auto window = Application::Get().GetWindow().GetBaseWindow();

        const auto buttonState = glfwGetMouseButton(static_cast<GLFWwindow*>(window), button);

        return buttonState == GLFW_PRESS;
    }

    std::pair<float, float> InputManager::GetMousePosition()
    {
         auto window = Application::Get().GetWindow().GetBaseWindow();

        double xpos, ypos;
        glfwGetCursorPos(static_cast<GLFWwindow*>(window), &xpos, &ypos);

        return { static_cast<float>(xpos), static_cast<float>(ypos) };
    }

    float InputManager::GetMouseX()
    {
        // first = x
        return GetMousePosition().first;
    }

    float InputManager::GetMouseY()
    {
        // second = y
        return GetMousePosition().second;
    }
}
