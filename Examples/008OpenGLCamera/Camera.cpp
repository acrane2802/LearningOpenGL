#include "Camera.h"

void Camera::update(float deltaTime)
{
    m_deltaTime = deltaTime;
    m_mouseX *= m_mouseSensitivity;
    m_mouseY *= m_mouseSensitivity;

    m_yaw += m_mouseX;
    m_pitch += m_mouseY;

    if(m_pitch > m_pitchMax)
    {
        m_pitch = m_pitchMax;
    }
    if(m_pitch < -m_pitchMax)
    {
        m_pitch = -m_pitchMax;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraFront = glm::normalize(direction);

    if (m_fieldOfView < m_fieldOfViewMin)
    {
        m_fieldOfView = m_fieldOfViewMin;
    }

    if (m_fieldOfView > m_fieldOfViewMax)
    {
        m_fieldOfView = m_fieldOfViewMax;
    }

    m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::setCameraMovement(CameraDirection direction)
{
    switch (direction)
    {
        case CAMERA_FORWARD:
            m_cameraPos += m_deltaTime * m_cameraFront * m_speed;
            break;
        case CAMERA_BACKWARD:
            m_cameraPos -= m_deltaTime * m_cameraFront * m_speed;
            break;
        case CAMERA_LEFT:
            m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_deltaTime * m_speed;
            break;
        case CAMERA_RIGHT:
            m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_deltaTime * m_speed;
            break;
    }
}

void Camera::lockAxis(CameraAxis axis, float value, bool isLocked)
{
    if (isLocked)
    {
        m_cameraPos[axis] = value;
    }
}
