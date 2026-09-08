#pragma once
#include "graphics/vertex_buffer.hpp"
#include <glad/glad.h>
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int get_size_of_type(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(float);
        case GL_UNSIGNED_INT:
            return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);
        }

        return 0;
    }
};

class VertexBufferLayout
{
  private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride = 0;

  public:
    template <typename T>
    void push(unsigned int count);

    inline const std::vector<VertexBufferElement> get_elements() const
    {
        return m_elements;
    };

    inline unsigned int get_stride() const
    {
        return m_stride;
    };
};

class VertexArray
{
  private:
    unsigned int m_vao;

  public:
    VertexArray();
    ~VertexArray();

    void add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void bind() const;
    void unbind() const;
};
