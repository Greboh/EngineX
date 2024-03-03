#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "EngineX/Core/Timestep.h"
#include "EngineX/Events/Event.h"

#include "EngineX/Events/ApplicationEvent.h"


namespace EngineX
{
    enum class CameraMode
    {
        FLY,
        FPS,  // TODO: Temporary .. FPS Mode is only for the school project anyways.
        FOCUS
    };
    
    class EditorCamera
    {
    public:
        EditorCamera(float fov, float width, float height, float nearP, float farP, CameraMode mode = CameraMode::FLY);

        void Init();

        void OnUpdate(Timestep deltaTime);
        void OnEvent(Event& event);

        inline float GetDistance() const { return m_DistanceToFocus; }
        
        inline glm::vec3 GetDirection() const { return m_ForwardDirection; }

        inline float GetPitch() const { return m_Pitch; }
        
        inline float GetYaw() const { return m_Yaw; }

        const glm::vec3& GetFocusPoint() const { return m_FocusPoint; }
        
        const glm::vec2& GetMousePosition() const { return m_MousePosition; }

        inline const glm::mat4& GetView() const { return m_ViewMatrix; }
        
        inline const glm::mat4& GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

        inline const float& GetFOV() const { return m_VerticalFOV; }
        inline void SetFOV(const float& fov) { m_VerticalFOV = fov; RefreshProjectionMatrix(); }

        float GetNearClip() const { return m_NearClip; }
        inline void SetNearClip(const float& nearP) { m_NearClip = nearP; RefreshProjectionMatrix(); }

        float GetFarClip() const { return m_FarClip; }
        inline void SetFarClip(const float& farP) { m_FarClip = farP; RefreshProjectionMatrix(); }

        CameraMode GetCameraMode() const { return m_CameraMode; }

        inline void SetCameraMode (const CameraMode& mode) { m_CameraMode = mode; m_CameraOverideMode = m_CameraMode; }

        glm::vec3 GetWorldUpDirection() const;
        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;

        const glm::vec3& GetPosition() const { return m_Position; }

        glm::quat GetOrientation() const;
        void Rotate(float pitch, float yaw);

    private:
        void UpdateCameraView();
        inline void RefreshProjectionMatrix() { m_ProjectionMatrix = glm::perspectiveFov(glm::radians(m_VerticalFOV), m_Viewport.x, m_Viewport.y, m_NearClip, m_FarClip); }
        
        glm::vec3 GetPositionRelativeToFocus() const;

        // Events
        bool OnWindowResize(WindowResizeEvent& e);
        
        glm::mat4 m_ProjectionMatrix, m_ViewMatrix;

        glm::vec3 m_Position, m_ForwardDirection;
        
        // The point that the camera is pointing at. Useful making the camera focus a specific object!
        glm::vec3 m_FocusPoint;
        
        float m_Pitch, m_Yaw;
        
        // Perspective projection vars
        float m_VerticalFOV, m_AspectRatio, m_NearClip, m_FarClip;
        glm::vec2 m_Viewport;

        glm::vec2 m_MousePosition;

        CameraMode m_CameraMode;
        CameraMode m_CameraOverideMode;
        
        float m_DistanceToFocus;

        glm::vec3 m_PositionStep;
        float m_PitchStep, m_YawStep;
    };
}
