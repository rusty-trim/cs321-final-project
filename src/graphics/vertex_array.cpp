#include "graphics/vertex_array.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_buffer.hpp"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_vao));
    GLCall(glBindVertexArray(m_vao));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_vao));
}

void VertexArray::add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.bind();
    const std::vector<VertexBufferElement> &elements = layout.get_elements();
    unsigned int offset = 0;

    for (int i = 0; i < elements.size(); i++)
    {
        const VertexBufferElement &element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void *)(size_t)offset));

        offset += element.count * VertexBufferElement::get_size_of_type(element.type);
    }
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(m_vao));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}

template <>
void VertexBufferLayout::push<float>(unsigned int count)
{
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += VertexBufferElement::get_size_of_type(GL_FLOAT) * count;
}

template <>
void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT) * count;
}

template <>
void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_FALSE});
    m_stride += VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE) * count;
}
