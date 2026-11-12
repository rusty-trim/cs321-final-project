#pragma once
#include "entity/player.hpp"
#include "vector_int3.hpp"
#include "world/block.hpp"
#include "world/world.hpp"

struct BlockBreakState
{
    bool active = false;
    glm::ivec3 target{0};
    float progress = 0.0f;
    float cooldown = 0.0f;
};

struct BlockPlaceState
{
    float cooldown = 0.0f;
};

void update_block_breaking(BlockBreakState &state, WorldData &world, const Player &player, bool held, float dt);
void update_block_placing(BlockPlaceState &state, WorldData &world, const Player &player, bool held, BlockID block_id, float dt);
