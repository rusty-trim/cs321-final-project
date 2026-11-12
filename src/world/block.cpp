#include "world/block.hpp"
#include <cstdint>
#include <vector>

BlockTypeInfo g_block_type_table[BLOCK_TYPE_COUNT];

void init_block_type_table()
{
    g_block_type_table[BLOCK_AIR] = {false};
    g_block_type_table[BLOCK_BEDROCK] = {true};
    g_block_type_table[BLOCK_STONE] = {true};
    g_block_type_table[BLOCK_DIRT] = {true};
    g_block_type_table[BLOCK_GRASS] = {true};
    g_block_type_table[BLOCK_OAK_LOG] = {true};
    g_block_type_table[BLOCK_OAK_PLANKS] = {true};
}

uint32_t get_texture_layer(BlockID id, glm::ivec3 face_normal)
{
    switch (id)
    {
    case BLOCK_BEDROCK:
        return LAYER_BEDROCK;
    case BLOCK_STONE:
        return LAYER_STONE;
    case BLOCK_DIRT:
        return LAYER_DIRT;
    case BLOCK_GRASS:
        if (face_normal.y == 1)
            return LAYER_GRASS_TOP;
        else if (face_normal.y == -1)
            return LAYER_DIRT;
        else
            return LAYER_GRASS_SIDE;
    case BLOCK_OAK_LOG:
        if (face_normal.y == 1)
            return LAYER_OAK_LOG_TOP;
        else if (face_normal.y == -1)
            return LAYER_OAK_LOG_TOP;
        else
            return LAYER_OAK_LOG_SIDE;
    case BLOCK_OAK_PLANKS:
        return LAYER_OAK_PLANKS;
    default:
        return LAYER_STONE;
    }
}

std::vector<const char *> get_block_texture_paths()
{
    std::vector<const char *> paths(LAYER_COUNT, nullptr);

    for (const BlockTextureEntry &entry : BLOCK_TEXTURES)
        paths[entry.layer] = entry.path;

    return paths;
}
