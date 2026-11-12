#include "graphics/ui/ui_element.hpp"
#include "matrix_float4x4.hpp"
#include "matrix_transform.hpp"
#include "ui/ui.hpp"
#include "vector_float2.hpp"
#include "vector_float3.hpp"

glm::vec2 anchor_fraction(Anchor anchor)
{
    switch (anchor)
    {
    case TOP_LEFT:
        return {0.0f, 0.0f};
    case TOP_CENTER:
        return {0.5f, 0.0f};
    case TOP_RIGHT:
        return {1.0f, 0.0f};
    case BOTTOM_LEFT:
        return {0.0f, 1.0f};
    case BOTTOM_CENTER:
        return {0.5f, 1.0f};
    case BOTTOM_RIGHT:
        return {1.0f, 1.0f};
    case CENTER:
    default:
        return {0.5f, 0.5f};
    }
}

void draw_ui_element(const UiElement &element, const UiQuadMesh &quad, Shader &shader, int screen_width, int screen_height)
{
    if (element.texture == nullptr || element.texture->id == 0)
        return;

    glm::vec2 screen((float)screen_width, (float)screen_height);
    glm::vec2 frac = anchor_fraction(element.anchor);
    glm::vec2 inward(frac.x > 0.5f ? -1.0f : 1.0f, frac.y > 0.5f ? -1.0f : 1.0f);
    glm::vec2 center = frac * screen + inward * element.position + (glm::vec2(0.5f) - frac) * element.size;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));

    model = glm::scale(model, glm::vec3(element.size, 1.0f));

    shader.set_uniform_mat4f("u_model", model);
    shader.set_uniform4f("u_color", element.color.r, element.color.g, element.color.b, element.color.a);

    draw_ui_quad(quad, *element.texture);
}
