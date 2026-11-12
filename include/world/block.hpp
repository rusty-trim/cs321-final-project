#pragma once
#include "vector_int3.hpp"
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <vector>

using BlockID = uint8_t;

enum BlockType : BlockID
{
    BLOCK_AIR = 0,
    BLOCK_STONE,
    BLOCK_DIRT,
    BLOCK_GRASS,
    BLOCK_OAK_LOG,
    BLOCK_OAK_PLANKS,
    BLOCK_BEDROCK,
    BLOCK_TYPE_COUNT
};

enum TextureLayer : unsigned int
{
    LAYER_BEDROCK = 0,
    LAYER_STONE,
    LAYER_DIRT,
    LAYER_GRASS_TOP,
    LAYER_GRASS_SIDE,
    LAYER_OAK_LOG_TOP,
    LAYER_OAK_LOG_SIDE,
    LAYER_OAK_PLANKS,
    LAYER_COUNT
};

struct BlockTextureEntry
{
    TextureLayer layer;
    const char *path;
};

constexpr BlockTextureEntry BLOCK_TEXTURES[] = {
    {LAYER_BEDROCK, "assets/textures/block/bedrock.png"},
    {LAYER_STONE, "assets/textures/block/stone.png"},
    {LAYER_DIRT, "assets/textures/block/dirt.png"},
    {LAYER_GRASS_TOP, "assets/textures/block/grass_top.png"},
    {LAYER_GRASS_SIDE, "assets/textures/block/grass_side.png"},
    {LAYER_OAK_LOG_TOP, "assets/textures/block/oak_log_top.png"},
    {LAYER_OAK_LOG_SIDE, "assets/textures/block/oak_log_side.png"},
    {LAYER_OAK_PLANKS, "assets/textures/block/oak_planks.png"}};

struct BlockTypeInfo
{
    bool is_solid;
};

extern BlockTypeInfo g_block_type_table[BLOCK_TYPE_COUNT];

void init_block_type_table();
inline bool is_block_solid(BlockID id)
{
    return g_block_type_table[id].is_solid;
}

std::vector<const char *> get_block_texture_paths();

uint32_t get_texture_layer(BlockID id, glm::ivec3 face_normal);

// TODO: Add BlockID id to get break time of other blocks if I decide to add more to this later
inline float get_break_time(BlockID id)
{
    static const std::unordered_map<BlockID, float> break_times = {
        {BLOCK_STONE, 1.5f},
        {BLOCK_DIRT, 0.5f},
        {BLOCK_GRASS, 0.6f},
        {BLOCK_OAK_LOG, 2.0f},
        {BLOCK_OAK_PLANKS, 2.0f}};

    auto it = break_times.find(id);

    return it != break_times.end() ? it->second : std::numeric_limits<float>().min();
}
