
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef _CAMERA_H_
#define _CAMERA_H_

namespace common {

enum Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
  public:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;
    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;

  public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH):
            m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
            m_MovementSpeed(SPEED),
            m_MouseSensitivity(SENSITIVITY),
            m_Zoom(ZOOM)
    {
        m_Position = position;
        m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
        Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch);
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(m_Position, m_Position+m_Front, m_Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float speed = m_MovementSpeed * deltaTime;
        if (direction == FORWARD) m_Position += m_Front*speed;
        if (direction == BACKWARD) m_Position -= m_Front*speed;
        if (direction == LEFT) m_Position -= m_Right*speed;
        if (direction == RIGHT) m_Position += m_Right*speed;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (constrainPitch) {
            if (m_Pitch >= 89.0f) m_Pitch = 89.0f;
            if (m_Pitch <= -89.0f) m_Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        m_Zoom -= yoffset;
        if (m_Zoom < 1.0f) m_Zoom = 1.0f;
        if (m_Zoom > 45.0f) m_Zoom = 45.0f;
    }

  private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
        front.y = sin(glm::radians(m_Pitch));
        front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
        m_Front = glm::normalize(front);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
};


}

#endif
