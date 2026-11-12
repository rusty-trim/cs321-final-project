#include "graphics/ui/ui.hpp"
#include "glad.h"
#include "graphics/renderer.hpp"
#include <stb/stb_image.h>

UiQuadMesh create_unit_quad_mesh()
{
    UiQuadMesh mesh;

    const static float vertices[16] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f};

    const static unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

    GLCall(glGenVertexArrays(1, &mesh.vao));
    GLCall(glGenBuffers(1, &mesh.vbo));
    GLCall(glGenBuffers(1, &mesh.ebo));

    GLCall(glBindVertexArray(mesh.vao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float))));

    GLCall(glBindVertexArray(0));

    return mesh;
}

void destroy_ui_quad_mesh(UiQuadMesh &mesh)
{
    GLCall(glDeleteBuffers(1, &mesh.vbo));
    GLCall(glDeleteBuffers(1, &mesh.ebo));
    GLCall(glDeleteVertexArrays(1, &mesh.vao));
    mesh = {};
}

void draw_ui_quad(const UiQuadMesh &mesh, const Texture2D &texture)
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.id));
    GLCall(glBindVertexArray(mesh.vao));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
