#include "enxpch.h"
#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace EngineX
{
    Camera::Camera(const float fov, const float aspectRatio, const float zNear, const float zFar)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_ClippingPlanes(zNear, zFar),
          m_RotationMatrix(1.0f),
          m_ViewMatrix(1.0f),
          m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar)),
          m_Position(0.0f, 0.0f, 3.0f), m_RotationAngle(0.0f, 0.0f, 0.0f)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::RecalculateViewMatrix()
    {
        m_RotationMatrix =
            rotate(glm::mat4(1.0f), glm::radians(m_RotationAngle.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * rotate(glm::mat4(1.0f), glm::radians(m_RotationAngle.y), glm::vec3(0.0f, 1.0f, 0.0f))
            * rotate(glm::mat4(1.0f), glm::radians(m_RotationAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 transform = translate(glm::mat4(1.0f), m_Position) * m_RotationMatrix;

        m_ViewMatrix = inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::RecalculateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_ClippingPlanes.x,
                                              m_ClippingPlanes.y);
        RecalculateViewMatrix();
    }
}
