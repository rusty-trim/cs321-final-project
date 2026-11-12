#pragma once
#include "matrix_float4x4.hpp"
#include "vector_float3.hpp"

class Camera
{
  private:
    glm::vec3 m_position{0.0f};
    glm::vec3 m_front{0.0f, 0.0f, -1.0f};
    glm::vec3 m_up{0.0f, 1.0f, 0.0f};
    mutable glm::mat4 m_view_matrix, m_proj_matrix;
    mutable bool m_view_dirty = true;
    mutable bool m_proj_dirty = true;

    float m_fov;
    float m_aspect_ratio;
    float m_near_plane, m_far_plane;

  public:
    Camera(float fov, float aspect_ratio, float near_plane, float far_plane);

    void set_pose(const glm::vec3 &eye, float yaw_deg, float pitch_deg);

    glm::mat4 &get_view_matrix() const;
    glm::mat4 &get_proj_matrix() const;

    glm::vec3 get_position() const
    {
        return m_position;
    }

    glm::vec3 get_front() const
    {
        return m_front;
    }

    void set_aspect_ratio(float aspect_ratio);
    void set_fov(float fov);
};
