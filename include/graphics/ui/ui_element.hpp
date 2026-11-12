#pragma once

#include "shader.hpp"
#include "ui/ui.hpp"
#include "vector_float2.hpp"
#include "vector_float4.hpp"

enum Anchor
{
    TOP_LEFT = 0,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT,
    CENTER,
    TOP_CENTER,
    BOTTOM_CENTER,
};

struct UiElement
{
    Texture2D *texture;
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;
    Anchor anchor;
};

void draw_ui_element(const UiElement &element, const UiQuadMesh &quad, Shader &shader, int screen_width, int screen_height);
