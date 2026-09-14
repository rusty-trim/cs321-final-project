#pragma once

#include "vector_float2.hpp"
#include "vector_float3.hpp"
struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture_coordinates;
    int texture_layer;
};

class VertexBuffer
{
    private:
        unsigned int m_vbo;

    public:
        VertexBuffer(const std::vector<Vertex> vertices);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;
};
