#pragma once

#include "world/block.hpp"
#include <array>

constexpr int HOTBAR_SIZE = 9;

struct ItemStack
{
    BlockID block = BLOCK_AIR;
    unsigned int count = 0;
};

struct Inventory
{
    std::array<ItemStack, HOTBAR_SIZE> hotbar{};
    int selected = 0;
};

inline void select_hotbar_slot(Inventory &inv, int slot)
{
    inv.selected = (slot % HOTBAR_SIZE + HOTBAR_SIZE) % HOTBAR_SIZE;
}

inline BlockID get_selected_block(const Inventory &inv)
{
    return inv.hotbar[inv.selected].block;
}
