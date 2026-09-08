#include "graphics/renderer.hpp"
#include "graphics/element_buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

void gl_clear_error()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool gl_log_call(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        printf("[OPENGL Error] (%i): %s %s: %i\n", error, function, file, line);
        return false;
    }

    return true;
}

void Renderer::draw(const VertexArray &va, const ElementBuffer &eb, const Shader &shader) const
{
    shader.bind();
    va.bind();
    eb.bind();

    GLCall(glDrawElements(GL_TRIANGLES, eb.get_count(), GL_UNSIGNED_INT, NULL));
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
