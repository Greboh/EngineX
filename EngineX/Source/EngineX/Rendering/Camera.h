#pragma once

#include <glm/glm.hpp>

namespace EngineX
{
    class Camera
    {
    public:
        Camera(float fov, float aspectRatio, float zNear, float zFar);

    
        inline const glm::vec3& GetPosition() const { return m_Position;}
        inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
    
        inline const glm::mat4& GetRotation() const { return m_RotationMatrix;}
        inline void SetRotation(const glm::mat4& rotation) { m_RotationMatrix = rotation; RecalculateViewMatrix(); }

        inline const glm::vec3& GetRotationAngle() const { return m_RotationAngle; }
        inline void SetRotationAngle(const glm::vec3& rotationAngle) { m_RotationAngle = rotationAngle * m_MouseSensitivity; RecalculateViewMatrix(); }

        inline const float& GetFOV() const { return m_FOV; }
        inline void SetFOV(const float& fov) { m_FOV = fov; RecalculateProjectionMatrix(); }
        
        inline const float& GetAspectRatio() const { return m_AspectRatio; }
        inline void SetAspectRatio(const float& aspectRatio) { m_AspectRatio = aspectRatio; RecalculateProjectionMatrix(); }

        inline const glm::vec2& GetClippingPlanes() const {return m_ClippingPlanes; }
        inline void SetClippingPlanes(const glm::vec2& clippingPlanes) { m_ClippingPlanes = clippingPlanes; RecalculateProjectionMatrix(); }
        
        inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
        inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
        inline glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    private:
        void RecalculateViewMatrix();
        void RecalculateProjectionMatrix();

        float m_FOV;
        float m_AspectRatio;

        glm::vec2 m_ClippingPlanes;
    
        glm::mat4 m_RotationMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position;
        glm::vec3 m_RotationAngle;

        float m_MouseSensitivity = 0.5f;
    };
}
