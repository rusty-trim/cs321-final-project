#include "ui/block_icons.hpp"
#include "game/inventory.hpp"
#include "matrix_clip_space.hpp"
#include "matrix_float4x4.hpp"
#include "matrix_transform.hpp"
#include "renderer.hpp"
#include "vector_float2.hpp"
#include "vector_float3.hpp"
#include "vector_int3.hpp"
#include "world/block.hpp"
#include <cstddef>
#include <glad.h>
#include <vector>
namespace
{
constexpr int ICON_VERTS_PER_BLOCK = 18; // 3 faces * 2 triangles * 3 vertices
constexpr float ICON_CUBE_SIZE = 7.0f;

struct IconVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    float layer;
    float shade;
};

struct IconFace
{
    glm::ivec3 normal;
    glm::vec3 corners[4];
    float shade;
};

const IconFace ICON_FACES[3] = {
    // +Y (top)
    {{0, 1, 0}, {{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}}, 1.0f},
    // +Z (front, shows on the left)
    {{0, 0, 1}, {{-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}}, 0.8f},
    // +X (right)
    {{1, 0, 0}, {{0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}}, 0.6f},
};
} // namespace

BlockIconMesh create_block_icon_mesh()
{
    const glm::vec2 uvs[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    const int tri[6] = {0, 1, 2, 2, 3, 0};

    std::vector<IconVertex> vertices;
    vertices.reserve(BLOCK_TYPE_COUNT * ICON_VERTS_PER_BLOCK);

    for (int id = 0; id < BLOCK_TYPE_COUNT; id++)
    {
        for (const IconFace &face : ICON_FACES)
        {
            float layer = (float)get_texture_layer(id, face.normal);

            for (int i : tri)
            {
                vertices.push_back({face.corners[i], uvs[i], layer, face.shade});
            }
        }
    }

    BlockIconMesh mesh;
    GLCall(glGenVertexArrays(1, &mesh.vao));
    GLCall(glGenBuffers(1, &mesh.vbo));
    GLCall(glBindVertexArray(mesh.vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(IconVertex), vertices.data(), GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(IconVertex), (void *)offsetof(IconVertex, position)));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(IconVertex), (void *)offsetof(IconVertex, uv)));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(IconVertex), (void *)offsetof(IconVertex, layer)));
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(IconVertex), (void *)offsetof(IconVertex, shade)));

    GLCall(glBindVertexArray(0));
    return mesh;
}

void destroy_block_icon_mesh(BlockIconMesh &mesh)
{
    GLCall(glDeleteBuffers(1, &mesh.vbo));
    GLCall(glDeleteVertexArrays(1, &mesh.vao));
    mesh = {};
}

void draw_hotbar_items(const Inventory &inv, const BlockIconMesh &mesh, Shader &shader, const TextureArray &blocks, const UiContext &ui_context)
{
    const float slot = 16.0f * ui_context.scale;
    const float cube = ICON_CUBE_SIZE * ui_context.scale;

    glm::mat4 projection = glm::ortho(0.0f, (float)ui_context.screen_width, (float)ui_context.screen_height, 0.0f, -1000.0f, 1000.0f);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1, 0, 0));
    rotation = glm::rotate(rotation, glm::radians(-45.0f), glm::vec3(0, 1, 0));

    shader.bind();
    shader.set_uniform_mat4f("u_projection", projection);
    shader.set_uniform1i("u_textures", 0);
    blocks.bind(0);
    GLCall(glBindVertexArray(mesh.vao));

    const float center_y = (float)ui_context.screen_height - 1.0f - slot * 0.5f;

    for (int i = 0; i < HOTBAR_SIZE; i++)
    {
        const ItemStack &stack = inv.hotbar[i];

        if (stack.block == BLOCK_AIR || stack.count == 0)
            continue;

        float center_x = (float)ui_context.screen_width * 0.5f - slot * 4.0f + slot * (float)i;

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(center_x, center_y, 0.0f));
        model = glm::scale(model, glm::vec3(cube, -cube, cube));
        model = model * rotation;

        shader.set_uniform_mat4f("u_model", model);
        GLCall(glDrawArrays(GL_TRIANGLES, stack.block * ICON_VERTS_PER_BLOCK, ICON_VERTS_PER_BLOCK));
    }

    GLCall(glBindVertexArray(0));
}
