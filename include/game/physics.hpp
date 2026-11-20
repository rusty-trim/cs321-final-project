#pragma once

#include "vector_float3.hpp"
#include "world/world.hpp"

inline const float MAX_FALL_SPEED = 9.8f;
// inline const float GRAVITY = 9.8f;
inline const float GRAVITY = 32.0f;
inline const float JUMP_HEIGHT = 1.25f;
inline const float JUMP_VEL = sqrt(2 * GRAVITY * JUMP_HEIGHT);

struct PhysicsBody
{
    glm::vec3 pos;
    glm::vec3 vel = glm::vec3(0, 0, 0);
    glm::vec3 size;

    bool on_ground = false;
    bool flying = false;
};

void move_and_collide(PhysicsBody &body, const WorldData &world, float dt);
