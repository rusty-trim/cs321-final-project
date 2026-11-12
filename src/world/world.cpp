#include "world/world.hpp"
#include "math/math.hpp"
#include "vector_float3.hpp"
#include "world/block.hpp"
#include "world/chunk.hpp"
#include "world/chunk_mesh.hpp"
#include <cstdlib>
#include <utility>
#include <vector>

static const ChunkCoord NEIGHBOR_OFFSETS[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

static void rebuild_chunk_mesh(WorldData &world, ChunkCoord coord)
{
    auto chunk_it = world.chunks.find(coord);

    if (chunk_it == world.chunks.end())
        return;

    BlockLookup lookup = [&world](int x, int y, int z)
    {
        return get_world_block(world, x, y, z);
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    build_chunk_mesh_cpu(chunk_it->second, lookup, vertices, indices);

    auto mesh_it = world.chunk_meshes.find(coord);

    if (mesh_it != world.chunk_meshes.end())
        destroy_chunk_mesh(mesh_it->second);

    world.chunk_meshes[coord] = upload_chunk_mesh(vertices, indices, coord);
}

static void mark_dirty_with_neighbors(WorldData &world, ChunkCoord coord)
{
    world.dirty_meshes.insert(coord);

    for (ChunkCoord off : NEIGHBOR_OFFSETS)
    {
        world.dirty_meshes.insert({coord.x + off.x, coord.z + off.z});
    }
}

void rebuild_dirty_meshes(WorldData &world)
{
    for (const ChunkCoord &coord : world.dirty_meshes)
    {
        if (world.chunks.find(coord) != world.chunks.end())
            rebuild_chunk_mesh(world, coord);
    }

    world.dirty_meshes.clear();
}

static void mark_dirty(WorldData &world, ChunkCoord coord)
{
    if (world.chunks.count(coord))
        world.dirty_meshes.insert(coord);
}

BlockID get_world_block(const WorldData &world, int world_x, int world_y, int world_z)
{
    ChunkCoord coord{floor_div(world_x, CHUNK_SIZE), floor_div(world_z, CHUNK_SIZE)};
    auto it = world.chunks.find(coord);

    if (it == world.chunks.end())
        return BLOCK_AIR;

    return get_block(it->second, world_x - coord.x * CHUNK_SIZE, world_y, world_z - coord.z * CHUNK_SIZE);
}

bool set_world_block(WorldData &world, int world_x, int world_y, int world_z, BlockID id)
{
    if (world_y < 0 || world_y >= CHUNK_MAX_HEIGHT)
        return false;

    ChunkCoord coord{floor_div(world_x, CHUNK_SIZE), floor_div(world_z, CHUNK_SIZE)};
    auto it = world.chunks.find(coord);

    if (it == world.chunks.end() || !it->second.is_generated)
        return false;

    int local_x = world_x - coord.x * CHUNK_SIZE;
    int local_z = world_z - coord.z * CHUNK_SIZE;

    set_block(it->second, local_x, world_y, local_z, id);

    mark_dirty(world, coord);

    if (local_x == 0)
        mark_dirty(world, {coord.x - 1, coord.z});
    if (local_x == CHUNK_SIZE - 1)
        mark_dirty(world, {coord.x + 1, coord.z});
    if (local_z == 0)
        mark_dirty(world, {coord.x, coord.z - 1});
    if (local_z == CHUNK_SIZE - 1)
        mark_dirty(world, {coord.x, coord.z + 1});

    return true;
}

void update_world_streaming(WorldData &world, glm::vec3 camera_pos, int load_radius)
{
    ChunkCoord center{floor_div((int)std::floor(camera_pos.x), CHUNK_SIZE),
                      floor_div((int)std::floor(camera_pos.z), CHUNK_SIZE)};

    // Load
    for (int dx = -load_radius; dx <= load_radius; dx++)
    {
        for (int dz = -load_radius; dz <= load_radius; dz++)
        {
            ChunkCoord coord{center.x + dx, center.z + dz};

            if (world.chunks.find(coord) != world.chunks.end())
                continue;

            ChunkData chunk;
            generate_chunk_terrain(chunk, coord);
            world.chunks[coord] = std::move(chunk);
            mark_dirty_with_neighbors(world, coord);
            // rebuild_chunk_mesh(world, coord);

            // for (ChunkCoord off : offsets)
            // {
            //     ChunkCoord neighbor{coord.x + off.x, coord.z + off.z};
            //
            //     if (world.chunks.find(neighbor) != world.chunks.end())
            //         rebuild_chunk_mesh(world, neighbor);
            // }
        }
    }

    // Unload
    for (auto it = world.chunks.begin(); it != world.chunks.end();)
    {
        ChunkCoord coord = it->first;

        if (std::abs(coord.x - center.x) > load_radius || std::abs(coord.z - center.z) > load_radius)
        {
            auto mesh_it = world.chunk_meshes.find(coord);

            if (mesh_it != world.chunk_meshes.end())
            {
                destroy_chunk_mesh(mesh_it->second);
                world.chunk_meshes.erase(mesh_it);
            }

            it = world.chunks.erase(it);
            mark_dirty_with_neighbors(world, coord);

            // for (ChunkCoord off : offsets)
            // {
            //     ChunkCoord neighbor{coord.x + off.x, coord.z + off.z};
            //
            //     if (world.chunks.find(neighbor) != world.chunks.end())
            //         rebuild_chunk_mesh(world, neighbor);
            // }
        }
        else
            ++it;
    }
}

void destroy_world(WorldData &world)
{
    for (auto &[coord, mesh] : world.chunk_meshes)
        destroy_chunk_mesh(mesh);

    world.chunk_meshes.clear();
    world.chunks.clear();
}
