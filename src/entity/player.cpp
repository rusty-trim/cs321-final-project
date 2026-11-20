#include "entity/player.hpp"
#include "common.hpp"
#include "game/physics.hpp"
#include "geometric.hpp"
#include "trigonometric.hpp"
#include "vector_float3.hpp"
#include "world/world.hpp"
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

void Player::update(const MoveInput &in, const WorldData &world, float dt)
{
    float yaw = glm::radians(m_yaw);
    glm::vec3 forward(std::cos(yaw), 0.0f, std::sin(yaw));
    glm::vec3 right(-forward.z, 0.0f, forward.x);
    glm::vec3 dir = forward * in.forward + right * in.right;

    if (glm::dot(dir, dir) > 0.0f)
        dir = glm::normalize(dir);

    float speed = in.sprint ? m_speed * m_sprint_mult : m_speed;

    m_body.vel.x = speed * dir.x;
    m_body.vel.z = speed * dir.z;

    // m_pos += dir * speed * dt;

    if (in.up > 0 && m_body.on_ground)
    {
        m_body.vel.y = JUMP_VEL;
        m_body.on_ground = false;
    }

    move_and_collide(m_body, world, dt);
}
