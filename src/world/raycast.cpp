#include "world/raycast.hpp"
#include "world/block.hpp"
#include <optional>

std::optional<RaycastHit> raycast(const WorldData &world, const glm::vec3 &origin, const glm::vec3 &dir, float max_dist)
{
    const float INF = std::numeric_limits<float>::infinity();

    glm::ivec3 pos = glm::ivec3(glm::floor(origin));

    glm::ivec3 step;
    glm::vec3 t_max, t_delta;

    for (unsigned int i = 0; i < 3; i++)
    {
        if (dir[i] > 0.0f)
        {
            step[i] = 1;
            t_delta[i] = 1.0f / dir[i];
            t_max[i] = (pos[i] + 1.0f - origin[i]) * t_delta[i]; // distance to the boundary ahead
        }
        else if (dir[i] < 0.0f)
        {
            step[i] = -1;
            t_delta[i] = -1.0f / dir[i];
            t_max[i] = (origin[i] - pos[i]) * t_delta[i];
        }
        else
        {
            step[i] = 0; // Not moving on this axis, so never cross its boundaries
            t_delta[i] = INF;
            t_max[i] = INF;
        }
    }

    glm::ivec3 normal(0);
    float t = 0.0f;

    while (t <= max_dist)
    {
        if (get_world_block(world, pos.x, pos.y, pos.z) != BlockType::BLOCK_AIR)
            return RaycastHit{pos, normal};

        int axis;
        if (t_max.x < t_max.y)
            axis = (t_max.x < t_max.z) ? 0 : 2;
        else
            axis = (t_max.y < t_max.z) ? 1 : 2;

        t = t_max[axis];
        pos[axis] += step[axis];
        t_max[axis] += t_delta[axis];

        normal = glm::ivec3(0);
        normal[axis] = -step[axis];
    }

    return std::nullopt;
}
