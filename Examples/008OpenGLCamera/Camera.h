#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraDirection
{
    CAMERA_FORWARD = 0,
    CAMERA_BACKWARD = 1,
    CAMERA_LEFT = 2,
    CAMERA_RIGHT = 3,
    CAMERA_UP = 4,
    CAMERA_DOWN = 5
};

enum CameraAxis
{
    CAMERA_AXIS_X = 0,
    CAMERA_AXIS_Y = 1,
    CAMERA_AXIS_Z = 2
};

class Camera {
public:
    Camera() = default;
    void update(float deltaTime);

    float getFieldOfView() const { return m_fieldOfView; }
    void setFieldOfView(float fieldOfView) { m_fieldOfView = fieldOfView; }

    void setFieldOfViewMin(float fieldOfViewMin) { m_fieldOfViewMin = fieldOfViewMin; }
    void setFieldOfViewMax(float fieldOfViewMax) { m_fieldOfViewMax = fieldOfViewMax; }

    glm::mat4 getViewMatrix() const { return m_viewMatrix; }

    void setMouseX(float x) { m_mouseX = x; }
    void setMouseY(float y) { m_mouseY = y; }

    glm::vec3 getCameraPos() const { return m_cameraPos; }
    void setCameraPos(glm::vec3 position) { m_cameraPos = position; }

    void setCameraMovement(CameraDirection direction);

    glm::vec3 getCameraFront() const { return m_cameraFront; }
    void setCameraFront(glm::vec3 front) { m_cameraFront = front; }

    glm::vec3 getCameraUp() const { return m_cameraUp; }
    void setCameraUp(glm::vec3 up) { m_cameraUp = up; }

    void setPitchMax(float pitchMax) { m_pitchMax = pitchMax; }

    float getMouseSensitivity() const { return m_mouseSensitivity; }
    void setMouseSensitivity(float mouseSensitivity) { m_mouseSensitivity = mouseSensitivity; }

    float getCameraSpeed() const { return m_speed; }
    void setCameraSpeed(float cameraSpeed) { m_speed = cameraSpeed; }
private:
    // this is the position in world space, the target to look at, and we get the direction vector by subtracting the target from the position, creating our z-axis
    glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    // our field of view variables to control the bounds
    float m_fieldOfView = 45.0f;
    float m_fieldOfViewMin = 1.0f;
    float m_fieldOfViewMax = 45.0f;

    // our view matrix to be passed to shaders
    glm::mat4 m_viewMatrix = glm::mat4(1.0f);

    // private mouse positions
    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;

    // default camera rotation
    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    // bounds for pitch
    float m_pitchMax = 89.0f;

    // private mouse sensitivity
    float m_mouseSensitivity = 0.1f;

    // variables to control the rate of movement independent of FPS
    float m_speed = 7.5f;
    float m_deltaTime = 0.0f;
};

#endif
