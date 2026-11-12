#include "graphics/ui/hotbar.hpp"
#include "ui/ui.hpp"
#include "ui/ui_element.hpp"
#include <vector>

void sync_hotbar_ui(const Hotbar &hotbar, std::vector<UiElement> &elements, const Inventory &inv, const UiContext &ui_context)
{
    float SLOT_WIDTH = 16.0f * ui_context.scale;
    elements[hotbar.selector].position.x = -SLOT_WIDTH * 4 + SLOT_WIDTH * inv.selected;
}
