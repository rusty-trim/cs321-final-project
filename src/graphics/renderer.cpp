#include "graphics/renderer.hpp"
#include "glad.h"
#include <cstdio>

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

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
