#pragma once
#include "texture_2d.hpp"

struct UiContext
{
    int screen_width,
        screen_height,
        scale;
};

struct UiQuadMesh
{
    unsigned int vao = 0,
                 vbo = 0,
                 ebo = 0;
};

UiQuadMesh create_unit_quad_mesh();
void destroy_ui_quad_mesh(UiQuadMesh &mesh);
void draw_ui_quad(const UiQuadMesh &mesh, const Texture2D &texture);
