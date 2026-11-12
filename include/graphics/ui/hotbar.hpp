#include "game/inventory.hpp"
#include "ui/ui.hpp"
#include "ui/ui_element.hpp"
#include <cstddef>
#include <vector>

struct Hotbar
{
    const size_t background;
    const size_t selector;
};

void sync_hotbar_ui(const Hotbar &hotbar, std::vector<UiElement> &elements, const Inventory &inv, const UiContext &ui_context);
