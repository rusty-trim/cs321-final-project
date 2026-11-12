#pragma once
#include "vector_float3.hpp"
#include "vector_int3.hpp"
#include "world/world.hpp"
#include <optional>
struct RaycastHit
{
    glm::ivec3 block;
    glm::ivec3 normal;
};

std::optional<RaycastHit> raycast(const WorldData &world, const glm::vec3 &origin, const glm::vec3 &dir, float max_dist);
