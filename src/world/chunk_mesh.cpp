#include "world/chunk_mesh.hpp"
#include "graphics/renderer.hpp"
#include "matrix_float4x4.hpp"
#include "matrix_transform.hpp"
#include "vector_float2.hpp"
#include "vector_float3.hpp"
#include "vector_int3.hpp"
// #include "vertex_buffer.hpp"
#include "glad.h"
#include "world/block.hpp"
#include "world/chunk.hpp"
#include <cstdint>
#include <vector>

namespace
{
struct FaceData
{
    glm::ivec3 normal;
    glm::vec3 corners[4];
};

const FaceData FACES[6] = {
    {{1, 0, 0}, {{1, 0, 0}, {1, 0, 1}, {1, 1, 1}, {1, 1, 0}}},
    // -X (left)
    {{-1, 0, 0}, {{0, 0, 1}, {0, 0, 0}, {0, 1, 0}, {0, 1, 1}}},
    // +Y (top)
    {{0, 1, 0}, {{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}}},
    // -Y (bottom)
    {{0, -1, 0}, {{0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {0, 0, 0}}},
    // +Z (front)
    {{0, 0, 1}, {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}}},
    // -Z (back)
    {{0, 0, -1}, {{1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0}}},
};

// int vertex_ao(bool side1, bool side2, bool corner)
// {
//     if (side1 && side2)
//         return 0;
//
//     return 3 - (int)side1 - (int)side2 - (int)corner;
// }
} // namespace

void build_chunk_mesh_cpu(const ChunkData &chunk, const BlockLookup &lookup, std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices)
{
    out_vertices.clear();
    out_indices.clear();

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_MAX_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                BlockID id = get_block(chunk, x, y, z);

                if (!is_block_solid(id))
                    continue;

                for (const FaceData &face : FACES)
                {
                    int nx = x + face.normal.x,
                        ny = y + face.normal.y,
                        nz = z + face.normal.z;

                    BlockID neighbor;

                    if (nx >= 0 && nx < CHUNK_SIZE && ny >= 0 && ny < CHUNK_MAX_HEIGHT && nz >= 0 && nz < CHUNK_SIZE)
                        neighbor = get_block(chunk, nx, ny, nz);
                    else
                    {
                        int world_x = chunk.position.x * CHUNK_SIZE + nx,
                            world_z = chunk.position.z * CHUNK_SIZE + nz;

                        neighbor = lookup(world_x, ny, world_z);
                    }

                    if (is_block_solid(neighbor))
                        continue;

                    uint32_t layer = get_texture_layer(id, face.normal);
                    unsigned int start = (unsigned int)out_vertices.size();
                    glm::vec2 uvs[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

                    for (int i = 0; i < 4; i++)
                    {
                        glm::vec3 pos = glm::vec3(x, y, z) + face.corners[i];
                        out_vertices.push_back(Vertex{pos, uvs[i], glm::vec3(face.normal), layer});
                    }

                    out_indices.insert(out_indices.end(), {start, start + 1, start + 2, start + 2, start + 3, start});
                }
            }
        }
    }
}

ChunkMeshData upload_chunk_mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, ChunkCoord coord)
{
    ChunkMeshData mesh;
    mesh.index_count = indices.size();

    GLCall(glGenVertexArrays(1, &mesh.vao));
    GLCall(glGenBuffers(1, &mesh.vbo));
    GLCall(glGenBuffers(1, &mesh.ebo));
    GLCall(glBindVertexArray(mesh.vao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position)));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texture_coordinates)));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal)));
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void *)offsetof(Vertex, texture_layer)));

    // TODO: AO
    // GLCall(glEnableVertexAttribArray(4));
    // GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, ao)));

    GLCall(glBindVertexArray(0));

    mesh.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * CHUNK_SIZE, 0.0f, coord.z * CHUNK_SIZE));
    return mesh;
}

void draw_chunk_mesh(const ChunkMeshData &mesh)
{
    GLCall(glBindVertexArray(mesh.vao));
    GLCall(glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr));
}

void destroy_chunk_mesh(ChunkMeshData &mesh)
{
    GLCall(glDeleteBuffers(1, &mesh.vbo));
    GLCall(glDeleteBuffers(1, &mesh.ebo));
    GLCall(glDeleteVertexArrays(1, &mesh.vao));
    mesh = {};
}

// struct FaceData
// {
//     glm::ivec3 normal;
//     glm::vec3 corners[4];
// };
//

//
// void build_mesh(const Chunk &chunk, const BlockLookup &lookup, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
// {
//     ChunkCoord coord = chunk.get_position();
//
//     for (int x = 0; x < CHUNK_SIZE; x++)
//     {
//         for (int y = 0; y < CHUNK_MAX_HEIGHT; y++)
//         {
//             for (int z = 0; z < CHUNK_SIZE; z++)
//             {
//                 Block block = chunk.get_block(x, y, z);
//
//                 if (!block.is_solid())
//                     continue;
//
//                 for (const FaceData &face : FACES)
//                 {
//                     int nx = x + face.normal.x, ny = y + face.normal.y, nz = z + face.normal.z;
//                     Block neighbor;
//                     // Block neighbor = chunk.get_block(x + face.normal.x, y + face.normal.y, z + face.normal.z);
//
//                     if (nx >= 0 && nx < CHUNK_SIZE && ny >= 0 && ny < CHUNK_MAX_HEIGHT && nz >= 0 && nz < CHUNK_SIZE)
//                     {
//                         neighbor = chunk.get_block(nx, ny, nz);
//                     }
//                     else
//                     {
//                         int world_x = coord.x * CHUNK_SIZE + nx;
//                         int world_z = coord.z * CHUNK_SIZE + nz;
//                         neighbor = lookup(world_x, ny, world_z);
//                     }
//
//                     if (neighbor.is_solid())
//                         continue;
//
//                     unsigned int layer = get_texture_layer(block.type, face.normal);
//
//                     unsigned int start_index = vertices.size();
//
//                     glm::vec2 uvs[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
//
//                     for (int i = 0; i < 4; i++)
//                     {
//                         glm::vec3 pos = glm::vec3(x, y, z) + face.corners[i];
//                         vertices.push_back(Vertex{pos, uvs[i], face.normal, layer});
//                     }
//
//                     indices.push_back(start_index + 0);
//                     indices.push_back(start_index + 1);
//                     indices.push_back(start_index + 2);
//                     indices.push_back(start_index + 2);
//                     indices.push_back(start_index + 3);
//                     indices.push_back(start_index + 0);
//                 }
//             }
//         }
//     }
// }
//
// ChunkMesh::ChunkMesh(const Chunk &chunk, const BlockLookup &lookup)
// {
//     std::vector<Vertex> vertices;
//     std::vector<unsigned int> indices;
//     build_mesh(chunk, lookup, vertices, indices);
//
//     VertexBufferLayout layout;
//     layout.push<float>(3);        // Vertex Position
//     layout.push<float>(2);        // Vertex Texture Coordinate
//     layout.push<float>(3);        // Vertex Normal
//     layout.push<unsigned int>(1); // Vertex Texture Layer
//
//     m_vb = std::make_unique<VertexBuffer>(vertices);
//     m_va.add_buffer(*m_vb, layout);
//     m_eb = std::make_unique<ElementBuffer>(indices.data(), indices.size());
//
//     ChunkCoord pos = chunk.get_position();
//     m_model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x * CHUNK_SIZE, 0.0f, pos.z * CHUNK_SIZE));
//     // m_model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.get_position() * CHUNK_SIZE, 1.0));
// }
