#pragma once
#include "graphics/element_buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

#define GLCall(x)     \
    gl_clear_error(); \
    x;                \
    gl_log_call(#x, __FILE__, __LINE__);

void gl_clear_error();
bool gl_log_call(const char *function, const char *file, int line);

class Renderer
{
  public:
    void clear() const;
    void draw(const VertexArray &va, const ElementBuffer &eb, const Shader &shader) const;
};
