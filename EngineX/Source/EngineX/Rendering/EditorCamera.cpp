#include "enxpch.h"
#include "EditorCamera.h"

#include "EngineX/Core/InputManager.h"

namespace EngineX
{
    EditorCamera::EditorCamera(const float fov, const float width, const float height, const float nearP,
                               const float farP, CameraMode mode)
        : m_ProjectionMatrix(glm::perspectiveFov(glm::radians(fov), width, height, nearP, farP)),
          m_FocusPoint(0.0f),
          m_VerticalFOV(fov),
          m_AspectRatio(width / height),
          m_NearClip(nearP),
          m_FarClip(farP),
          m_Viewport(width, height),
          m_CameraMode(mode)

    {
        Init();
    }

    void EditorCamera::Init()
    {
        m_DistanceToFocus = distance(m_Position, m_FocusPoint);
        m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
        m_PositionStep = glm::vec3(0.0f, 0.0f, 0.0f);
        m_MousePosition = glm::vec2(0);

        m_Pitch = 0;
        m_Yaw = 0;
        m_PitchStep = 0.0f;
        m_YawStep = 0.0f;

        UpdateCameraView();
    }

    void EditorCamera::OnUpdate(Timestep deltaTime)
    {
        // TODO: The camera currently always rotates as if its up direction is world up.
        // This means if the cameras up direction is -1.0f the yaw rotation is inverted!

        const glm::vec2& mouse{InputManager::GetMouseX(), InputManager::GetMouseY()};

        // NOTE:: GetMouseX & Y gets the mouse position based on screen resolution (1920 x 1080)
        // NOTE:: Which means its crazy high. Therefor it gets multiplied by an extremely low number
        const glm::vec2 mouseStep = (mouse - m_MousePosition) * 0.002f;

        // TODO: Refactor this to Mouse Sensitivity * Camera Speed
        const float speed = 2.0f;
        const float rotationSpeed = 0.55f;

        // Input to control the camera
        if (InputManager::IsMouseButtonPressed(Mouse::RightClick) && !InputManager::IsKeyPressed(Key::LeftAlt))
        {
            m_CameraMode = m_CameraOverideMode == CameraMode::FPS ? CameraMode::FPS : CameraMode::FLY;

            // NOTE: In order to move smoothly we calculate everything in steps that gets added onto our actual value
            // NOTE: This adds the benefit of being able to manipulate the steps without directly affecting the actual value

            if (InputManager::IsKeyPressed(Key::Q))
                m_PositionStep -= deltaTime * speed * GetWorldUpDirection();
            if (InputManager::IsKeyPressed(Key::E))
                m_PositionStep += deltaTime * speed * GetWorldUpDirection();
            if (InputManager::IsKeyPressed(Key::S))
                m_PositionStep -= deltaTime * speed * GetForwardDirection();
            if (InputManager::IsKeyPressed(Key::W))
                m_PositionStep += deltaTime * speed * GetForwardDirection();
            if (InputManager::IsKeyPressed(Key::A))
                m_PositionStep -= deltaTime * speed * GetRightDirection();
            if (InputManager::IsKeyPressed(Key::D))
                m_PositionStep += deltaTime * speed * GetRightDirection();

            // Get the new distance for the focus point relative 
            const float distanceStep = distance(m_FocusPoint, m_Position);
            m_FocusPoint = m_Position + GetForwardDirection() * distanceStep;
            m_DistanceToFocus = distanceStep;

            m_YawStep += GetWorldUpDirection().y * mouseStep.x * rotationSpeed;
            m_PitchStep += mouseStep.y * rotationSpeed;
        }
        else if (InputManager::IsKeyPressed(Key::LeftAlt))
        {
            m_CameraMode = CameraMode::FOCUS;

            if (InputManager::IsMouseButtonPressed(Mouse::LeftClick))
            {
                m_YawStep += GetWorldUpDirection().y * mouseStep.x * rotationSpeed;
                m_PitchStep += mouseStep.y * rotationSpeed;
            }
        }
        else if (InputManager::IsKeyPressed(Key::F))
        {
            m_CameraMode = CameraMode::FLY;

            // NOTE: Can't select an object in the view yet. So just reset the focus point to vec3(0)
            m_FocusPoint = {0.0f, 0.0f, 0.0f};

            // TODO: Should probably not reset the rotation to 0 but instead get the rotation between the cameras and the focus point and if its an objects, its front direction
            // NOTE: If the focus point is on an object with a rot of 0
            // NOTE: This does focus on the front direction. But perhaps it should do this no matter what ? ..  
            m_Yaw = 0.0f;
            m_Pitch = 0.0f;

            // Set the cameras position relative to the focus point
            m_Position = GetPositionRelativeToFocus();
        }

        // TODO: Rotate testing .. to be removed
        if (InputManager::IsKeyPressed(Key::X))
        {
            Rotate(5 * deltaTime, 0);
        }
        else if (InputManager::IsKeyPressed(Key::Z))
            Rotate(0, 5 * deltaTime);


        // Update all vars based on steps
        m_MousePosition = mouse;
        m_Position += m_PositionStep;
        m_Yaw += m_YawStep;
        m_Pitch += m_PitchStep;

        // NOTE: If the camera is in focus mode .. Make sure it always sets it position based on the focus point
        if (m_CameraMode == CameraMode::FOCUS)
        {
            m_Position = GetPositionRelativeToFocus();
        }

        UpdateCameraView();
    }

    void EditorCamera::UpdateCameraView()
    {
        // Calculate the cameras forward pointing direction using the cameras position and orientation.
        // We use the default forward direction z: -1.0f and rotate this vector based on our orientation  
        m_ForwardDirection = m_Position + rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));

        // Recalculate the distance to our focus point
        m_DistanceToFocus = distance(m_Position, m_FocusPoint);

        // Generate a View Matrix 
        m_ViewMatrix = lookAt(m_Position, m_ForwardDirection, GetUpDirection());

        // Smooth camera using damping
        m_YawStep *= 0.6f;
        m_PitchStep *= 0.6f;
        m_PositionStep *= 0.8f;
    }

    void EditorCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowResizeEvent>(ENX_BIND_EVENT_FN(OnWindowResize));

    }

    glm::vec3 EditorCamera::GetWorldUpDirection() const
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        if (m_CameraMode == CameraMode::FPS)
        {
            glm::vec3 dir = rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
            dir.y = 0.0f;
            return dir;
        }

        return rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        //NOTE: Construct a quat using euler angles. We subtract pitch and yaw otherwise rotation would be inverted!.
        return glm::quat(glm::vec3(-m_Pitch - m_PitchStep, -m_Yaw - m_YawStep, 0.0f));
    }

    glm::vec3 EditorCamera::GetPositionRelativeToFocus() const
    {
        return m_FocusPoint - GetForwardDirection() * m_DistanceToFocus + m_PositionStep;
    }

    bool EditorCamera::OnWindowResize(WindowResizeEvent& e)
    {
        m_Viewport = {e.GetWidth(), e.GetHeight()};
        m_AspectRatio = m_Viewport.x / m_Viewport.y;

        RefreshProjectionMatrix();

        return false;
    }

    void EditorCamera::Rotate(float pitch, float yaw)
    {
        m_Pitch += pitch;
        m_Yaw += yaw;

        UpdateCameraView();
    }
}
