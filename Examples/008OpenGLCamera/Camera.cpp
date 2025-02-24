#include "Camera.h"

void Camera::update(float deltaTime)
{
    // copy delta time to private variable
    m_deltaTime = deltaTime;

    // set mouse position according to the sensitivity multiplier
    m_mouseX *= m_mouseSensitivity;
    m_mouseY *= m_mouseSensitivity;

    // set pitch and yaw
    m_yaw += m_mouseX;
    m_pitch += m_mouseY;

    // keep pitch in bounds
    if(m_pitch > m_pitchMax)
    {
        m_pitch = m_pitchMax;
    }
    if(m_pitch < -m_pitchMax)
    {
        m_pitch = -m_pitchMax;
    }

    // set camera direction using trigonometry
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraFront = glm::normalize(direction);

    // keep field of view in bounds
    if (m_fieldOfView < m_fieldOfViewMin)
    {
        m_fieldOfView = m_fieldOfViewMin;
    }

    if (m_fieldOfView > m_fieldOfViewMax)
    {
        m_fieldOfView = m_fieldOfViewMax;
    }

    // set the final view matrix per frame
    m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::setCameraMovement(CameraDirection direction)
{
    // depending on direction provided, move the camera
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
        case CAMERA_UP:
            m_cameraPos += m_deltaTime * m_cameraUp * m_speed;
            break;
        case CAMERA_DOWN:
            m_cameraPos -= m_deltaTime * m_cameraUp * m_speed;
            break;
    }
}
