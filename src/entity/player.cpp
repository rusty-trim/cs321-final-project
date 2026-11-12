#include "entity/player.hpp"
#include "common.hpp"
#include "geometric.hpp"
#include "trigonometric.hpp"
#include "vector_float3.hpp"
#include <cmath>

void Player::look(float dx, float dy)
{
    m_yaw += dx * m_sensitivity;
    m_pitch -= dy * m_sensitivity;
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
}

glm::vec3 Player::get_look_dir() const
{
    float yaw = glm::radians(m_yaw);
    float pitch = glm::radians(m_pitch);

    return glm::normalize(glm::vec3(std::cos(yaw) * std::cos(pitch), std::sin(pitch), std::sin(yaw) * std::cos(pitch)));
}

void Player::update(const MoveInput &in, float dt)
{
    float yaw = glm::radians(m_yaw);
    glm::vec3 forward(std::cos(yaw), 0.0f, std::sin(yaw));
    glm::vec3 right(-forward.z, 0.0f, forward.x);

    glm::vec3 dir = forward * in.forward + right * in.right;

    if (glm::dot(dir, dir) > 0.0f)
        dir = glm::normalize(dir);

    dir.y = in.up;

    float speed = in.sprint ? m_speed * m_sprint_mult : m_speed;
    m_pos += dir * speed * dt;
}
