#include "Camera.h"

void Camera::update(double deltaTime)
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

void Camera::setTransformation(CameraTransformationAxes axisType, float axis)
{
    switch (axisType)
    {
        case CAMERA_AXIS_X:
            m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * static_cast<float>(m_deltaTime) * (axis * m_speed);
            break;
        case CAMERA_AXIS_Y:
            m_cameraPos += static_cast<float>(m_deltaTime) * m_cameraFront * (axis * m_speed);
            break;
        case CAMERA_AXIS_Z:
            m_cameraPos += static_cast<float>(m_deltaTime) * m_cameraUp * (axis * m_speed);
            break;
        case CAMERA_LOOK_AXIS_X:
            m_mouseX = (axis * m_speed);
            break;
        case CAMERA_LOOK_AXIS_Y:
            m_mouseY = (axis * m_speed);
            break;

    }
}
