#include "game/physics.hpp"
#include "vector_float3.hpp"
#include "world/world.hpp"
#include <algorithm>
#include <cmath>

constexpr float EPS = 0.001f;

static bool overlaps_solid(const PhysicsBody &body, const WorldData &world)
{
    float hx = body.size.x * 0.5f,
          hz = body.size.z * 0.5f;

    glm::vec3 min(body.pos.x - hx, body.pos.y, body.pos.z - hz);
    glm::vec3 max(body.pos.x + hx, body.pos.y + body.size.y, body.pos.z + hz);

    for (int x = (int)std::floor(min.x); x <= (int)std::floor(max.x - EPS); x++)
    {
        for (int y = (int)std::floor(min.y); y <= (int)std::floor(max.y - EPS); y++)
        {
            for (int z = (int)std::floor(min.z); z <= (int)std::floor(max.z - EPS); z++)
            {
                if (is_world_block_solid(world, x, y, z))
                    return true;
            }
        }
    }

    return false;
}

static void move_axis(PhysicsBody &body, const WorldData &world, unsigned int axis, float delta)
{
    body.pos[axis] += delta;

    if (!overlaps_solid(body, world))
        return;

    float lo = (axis == 1) ? 0.0f : body.size[axis] * 0.5f;
    float hi = (axis == 1) ? body.size.y : body.size[axis] * 0.5f;

    if (delta > 0.0f)
    {
        float face = std::floor(body.pos[axis] + hi);
        body.pos[axis] = face - hi - EPS;
    }
    else
    {
        float face = std::floor(body.pos[axis] - lo) + 1.0f;
        body.pos[axis] = face + lo + EPS;

        if (axis == 1)
            body.on_ground = true;
    }

    body.vel[axis] = 0.0f;
}

void move_and_collide(PhysicsBody &body, const WorldData &world, float dt)
{
    if (!body.flying)
    {
        body.vel.y -= GRAVITY * dt;
        body.vel.y = std::max(body.vel.y, -MAX_FALL_SPEED);
    }

    body.on_ground = false;

    move_axis(body, world, 0, body.vel.x * dt);
    move_axis(body, world, 1, body.vel.y * dt);
    move_axis(body, world, 2, body.vel.z * dt);
}
