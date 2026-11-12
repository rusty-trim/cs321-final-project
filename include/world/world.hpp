#pragma once
#include "vector_float3.hpp"
#include "world/block.hpp"
#include "world/chunk.hpp"
#include "world/chunk_mesh.hpp"
#include <unordered_map>
#include <unordered_set>

struct WorldData
{
    std::unordered_map<ChunkCoord, ChunkData, ChunkCoordHash> chunks;
    std::unordered_map<ChunkCoord, ChunkMeshData, ChunkCoordHash> chunk_meshes;
    std::unordered_set<ChunkCoord, ChunkCoordHash> dirty_meshes;
};

void rebuild_dirty_meshes(WorldData &world);
void update_world_streaming(WorldData &world, glm::vec3 camera_pos, int load_radius);
BlockID get_world_block(const WorldData &world, int world_x, int world_y, int world_z);
bool set_world_block(WorldData &world, int world_x, int world_y, int world_z, BlockID id);
void destroy_world(WorldData &world);
