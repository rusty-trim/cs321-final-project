#include "graphics/camera.hpp"
#include "geometric.hpp"
#include "matrix_transform.hpp"
#include "trigonometric.hpp"
#include "vector_float3.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float aspect_ratio, float near_plane, float far_plane)
    : m_fov(fov), m_aspect_ratio(aspect_ratio), m_near_plane(near_plane), m_far_plane(far_plane) {};

void Camera::set_pose(const glm::vec3 &eye, float yaw_deg, float pitch_deg)
{
    float yaw = glm::radians(yaw_deg);
    float pitch = glm::radians(pitch_deg);

    m_position = eye;
    m_front = glm::normalize(glm::vec3(std::cos(yaw) * std::cos(pitch), std::sin(pitch),
                                       std::sin(yaw) * std::cos(pitch)));

    glm::vec3 right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(right, m_front));
    m_view_dirty = true;
}

glm::mat4 &Camera::get_view_matrix() const
{
    if (m_view_dirty)
    {
        m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
        m_view_dirty = false;
    }

    return m_view_matrix;
}

glm::mat4 &Camera::get_proj_matrix() const
{
    if (m_proj_dirty)
    {
        m_proj_matrix = glm::perspective(m_fov, m_aspect_ratio, m_near_plane, m_far_plane);
        m_proj_dirty = false;
    }

    return m_proj_matrix;
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
    m_aspect_ratio = aspect_ratio;
    m_proj_dirty = true;
}

void Camera::set_fov(float fov)
{
    m_fov = fov;
    m_proj_dirty = true;
}
