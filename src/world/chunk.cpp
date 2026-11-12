#include "world/chunk.hpp"
#include "noise.hpp"
#include "vector_float2.hpp"
#include "world/block.hpp"

BlockID get_block(const ChunkData &chunk, int x, int y, int z)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_MAX_HEIGHT || z < 0 || z > CHUNK_SIZE)
        return BLOCK_AIR;

    return chunk.blocks[voxel_index(x, y, z)];
}

void set_block(ChunkData &chunk, int x, int y, int z, BlockID id)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_MAX_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        return;

    chunk.blocks[voxel_index(x, y, z)] = id;
}

int get_surface_height(int world_x, int world_z)
{
    const float scale = 0.05f;    // Lower = smoother, wider hills
    const float amplitude = 3.0f; // How tall hills can get
    const int base_height = 32;   // sea-level-ish baseline

    float noise_value = glm::perlin(glm::vec2(world_x * scale, world_z * scale));
    // float noise_value = glm::simplex(glm::vec2(world_x * scale, world_z * scale));

    return base_height + (noise_value * amplitude);
}

void generate_chunk_terrain(ChunkData &chunk, ChunkCoord coord)
{
    chunk.position = coord;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            int world_x = coord.x * CHUNK_SIZE + x;
            int world_z = coord.z * CHUNK_SIZE + z;
            int surface_height = get_surface_height(world_x, world_z);

            for (int y = 0; y < CHUNK_MAX_HEIGHT; y++)
            {
                BlockID id;

                if (y > surface_height)
                    id = BLOCK_AIR;
                else if (y == surface_height)
                    id = BLOCK_GRASS;
                else if (y > surface_height - 4)
                    id = BLOCK_DIRT;
                else if (y == 0)
                    id = BLOCK_BEDROCK;
                else
                    id = BLOCK_STONE;

                set_block(chunk, x, y, z, id);
            }
        }
    }

    chunk.is_generated = true;
}
//
// Chunk::Chunk(ChunkCoord coord)
// {
//     m_position = coord;
// }
//
// Block Chunk::get_block(int x, int y, int z) const
// {
//     if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_MAX_HEIGHT || z < 0 || z >= CHUNK_SIZE)
//         return Block{BlockType::AIR};
//
//     return m_blocks[x][y][z];
// }
//
// void Chunk::set_block(int x, int y, int z, BlockType type)
// {
//     if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_MAX_HEIGHT || z < 0 || z >= CHUNK_SIZE)
//         return;
//
//     m_blocks[x][y][z].type = type;
// }
//
// Chunk Chunk::make_test_chunk()
// {
//     Chunk chunk;
//
//     // Flat ground: a 4-block-thick stone floor across the whole chunk
//     for (int x = 0; x < CHUNK_SIZE; x++)
//     {
//         for (int z = 0; z < CHUNK_SIZE; z++)
//         {
//             for (int y = 0; y < 4; y++)
//             {
//                 chunk.set_block(x, y, z, BlockType::STONE);
//             }
//         }
//     }
//
//     //  small rasied platform (test side + top faces being exposed
//     for (int x = 4; x < 10; x++)
//     {
//         for (int z = 4; z < 10; z++)
//         {
//             chunk.set_block(x, 4, z, BlockType::STONE);
//         }
//     }
//
//     // A single floating column sticking up out of the platform (tests all 6 faces exposed on some blocks and overhangs)
//     for (int y = 5; y < 9; y++)
//     {
//         chunk.set_block(6, y, 6, BlockType::STONE);
//     }
//
//     // A couple of isolated floating blocks, disconnected from anything (tests fully exposed blocks with all 6 faces visible
//     chunk.set_block(2, 6, 12, BlockType::STONE);
//     chunk.set_block(12, 8, 2, BlockType::STONE);
//
//     return chunk;
// }
//
// Chunk Chunk::generate(ChunkCoord coord)
// {
//     Chunk chunk(coord);
//
//     for (int x = 0; x < CHUNK_SIZE; x++)
//     {
//         for (int z = 0; z < CHUNK_SIZE; z++)
//         {
//             int world_x = coord.x * CHUNK_SIZE + x;
//             int world_z = coord.z * CHUNK_SIZE + z;
//             int surface_height = get_surface_height(world_x, world_z);
//
//             for (int y = 0; y < CHUNK_MAX_HEIGHT; y++)
//             {
//                 BlockType type;
//
//                 if (y > surface_height)
//                     type = BlockType::AIR;
//                 else if (y == surface_height)
//                     type = BlockType::GRASS; // TODO: Grass
//                 else if (y > surface_height - 4)
//                     type = BlockType::DIRT; // TODO: Dirt
//                 else if (y == 0)
//                     type = BlockType::BEDROCK; // TODO: Bedrock or something similar
//                 else
//                     type = BlockType::STONE;
//
//                 chunk.set_block(x, y, z, type);
//             }
//         }
//     }
//
//     return chunk;
// }
