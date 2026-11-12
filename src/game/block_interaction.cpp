#include "game/block_interaction.hpp"
#include "entity/player.hpp"
#include "vector_float3.hpp"
#include "vector_int3.hpp"
#include "world/block.hpp"
#include "world/raycast.hpp"
#include "world/world.hpp"
#include <limits>

static bool block_overlaps_player(glm::ivec3 block, const Player &player)
{
    glm::vec3 p = player.get_position();
    float half = Player::WIDTH / 2;

    glm::vec3 pmin(p.x - half, p.y, p.z - half);
    glm::vec3 pmax(p.x + half, p.y + Player::HEIGHT, p.z + half);

    glm::vec3 bmin(block);
    glm::vec3 bmax = bmin + glm::vec3(1.0f);

    return pmin.x < bmax.y && pmax.x > bmin.x && pmin.y < bmax.y && pmax.y > bmin.y &&
           pmin.z < bmax.z && pmax.z > bmin.z;
}

void update_block_breaking(BlockBreakState &state, WorldData &world, const Player &player,
                           bool held, float dt)
{
    if (state.cooldown > 0.0f)
        state.cooldown -= dt;

    auto reset = [&]
    {
        state.active = false;
        state.progress = 0.0f;
    };

    if (!held)
    {
        reset();
        state.cooldown = 0.0f;
        return;
    }

    auto hit = raycast(world, player.get_eye_position(), player.get_look_dir(), 4.5f);

    if (!hit)
    {
        reset();
        return;
    }

    if (!state.active || hit->block != state.target)
    {
        state.active = true;
        state.target = hit->block;
        state.progress = 0.0f;
    }

    if (state.cooldown > 0.0f)
        return;

    BlockID id = get_world_block(world, state.target.x, state.target.y, state.target.z);
    float break_time;

    switch (player.get_gamemode())
    {
    case GAMEMODE_SURVIVAL:
        break_time = get_break_time(id);
        break;
    case GAMEMODE_CREATIVE:
    default:
        break_time = 0.0f;
        break;
    }

    if (break_time == std::numeric_limits<float>().min())
        return;
    else
        state.progress = state.progress + dt / break_time;

    if (state.progress >= 1.0f)
    {
        set_world_block(world, state.target.x, state.target.y, state.target.z,
                        BlockType::BLOCK_AIR);
        reset();
        state.cooldown = 0.25f;
    }
}

void update_block_placing(BlockPlaceState &state, WorldData &world, const Player &player, bool held,
                          BlockID block_id, float dt)
{
    if (block_id == BLOCK_AIR)
        return;

    if (!held)
    {
        state.cooldown = 0.0f;
        return;
    }

    if (state.cooldown > 0.0f)
    {
        state.cooldown -= dt;
        return;
    }

    auto hit = raycast(world, player.get_eye_position(), player.get_look_dir(), 4.5f);

    if (!hit)
        return;

    glm::ivec3 place = hit->block + hit->normal;

    if (get_world_block(world, place.x, place.y, place.z) != BlockType::BLOCK_AIR)
        return;

    if (block_overlaps_player(place, player))
        return;

    if (set_world_block(world, place.x, place.y, place.z, block_id))
        state.cooldown = 0.2f;
}
