#pragma once

#include "matrix_float4x4.hpp"
// #include "vertex_buffer.hpp"
#include "world/block.hpp"
#include "world/chunk.hpp"
#include <functional>
#include <vector>

using BlockLookup = std::function<BlockID(int world_x, int world_y, int world_z)>;

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture_coordinates;
    glm::vec3 normal;
    unsigned int texture_layer;
};

struct ChunkMeshData
{
    unsigned int vao = 0, vbo = 0, ebo = 0;
    unsigned int index_count = 0;
    glm::mat4 model_matrix{1.0f};
};

void build_chunk_mesh_cpu(const ChunkData &chunk, const BlockLookup &lookup, std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices);
ChunkMeshData upload_chunk_mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, ChunkCoord coord);
void draw_chunk_mesh(const ChunkMeshData &mesh);
void destroy_chunk_mesh(ChunkMeshData &mesh);
