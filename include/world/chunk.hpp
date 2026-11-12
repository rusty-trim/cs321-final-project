#pragma once
#include "world/block.hpp"
#include <functional>
#include <vector>

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_MAX_HEIGHT = 64;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_MAX_HEIGHT * CHUNK_SIZE;

struct ChunkCoord
{
    int x, z;

    bool operator==(const ChunkCoord &other) const
    {
        return x == other.x && z == other.z;
    }
};

struct ChunkCoordHash
{
    size_t operator()(const ChunkCoord &coord) const
    {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.z) << 16);
    }
};

struct ChunkData
{
    std::vector<BlockID> blocks;
    ChunkCoord position{0, 0};
    bool is_generated = false;

    ChunkData()
    {
        blocks.resize(CHUNK_VOLUME, BLOCK_AIR);
    }
};

inline int voxel_index(int x, int y, int z)
{
    return x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE;
}

BlockID get_block(const ChunkData &chunk, int x, int y, int z);
void set_block(ChunkData &chunk, int x, int y, int z, BlockID id);
void generate_chunk_terrain(ChunkData &chunk, ChunkCoord coord);
