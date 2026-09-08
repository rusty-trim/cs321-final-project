#include "graphics/element_buffer.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_array.hpp"

ElementBuffer::ElementBuffer(const unsigned int *data, unsigned int count)
{
    m_count = count;
    GLCall(glGenBuffers(1, &m_ebo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

ElementBuffer::~ElementBuffer()
{
    GLCall(glDeleteBuffers(1, &m_ebo));
}

void ElementBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
}

void ElementBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
