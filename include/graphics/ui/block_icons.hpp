#pragma once

#include "game/inventory.hpp"
#include "shader.hpp"
#include "texture_array.hpp"
#include "ui/ui.hpp"
struct BlockIconMesh
{
    unsigned int vao = 0,
                 vbo = 0;
};

BlockIconMesh create_block_icon_mesh();
void destroy_block_icon_mesh(BlockIconMesh &mesh);

void draw_hotbar_items(const Inventory &inv, const BlockIconMesh &mesh, Shader &shader, const TextureArray &blocks, const UiContext &ui_context);
