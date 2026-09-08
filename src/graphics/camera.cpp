#include "graphics/camera.hpp"
#include "geometric.hpp"
#include "trigonometric.hpp"
#include "vector_float3.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &up)
{
    m_position = position;
    m_front = front;
    m_up = up;
}

Camera::~Camera()
{
    // TODO: I do not believe there is any needed cleanup,
    // but I will keep this here just in case.
}

void Camera::handle_mouse(float dx, float dy)
{
    m_yaw += dx * m_sensitivity;
    m_pitch += -dy * m_sensitivity;
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f); // Prevents gimbal clip where you can look behind yourself and things become upside down.

    glm::vec3 dir;
    dir.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    dir.y = sin(glm::radians(m_pitch));
    dir.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(dir);

    glm::vec3 world_up(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(m_front, world_up));
    m_up = glm::normalize(glm::cross(right, m_front));
}

void Camera::handle_keyboard(CameraDirection direction, float dt, bool sprint)
{
    float current_speed = sprint ? m_speed * 2 : m_speed;
    float velocity = current_speed * dt;

    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 planar_front = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
    glm::vec3 right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));

    switch (direction)
    {
    case FORWARD:
        m_position += planar_front * velocity;
        break;
    case BACKWARD:
        m_position -= planar_front * velocity;
        break;
    case LEFT:
        m_position -= right * velocity;
        break;
    case RIGHT:
        m_position += right * velocity;
        break;
    case DOWN:
        m_position -= world_up * velocity;
        break;
    case UP:
        m_position += world_up * velocity;
        break;
    }
}
