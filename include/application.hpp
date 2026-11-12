#pragma once

#include "entity/player.hpp"
#include "game/block_interaction.hpp"
#include "graphics/camera.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture_array.hpp"
#include "graphics/ui/hotbar.hpp"
#include "graphics/ui/ui.hpp"
#include "sdl_context.hpp"
#include "ui/ui_element.hpp"
#include "window.hpp"
#include "world/world.hpp"
#include <vector>

class Application
{
  private:
    SDLContext m_sdl;
    Window m_window;
    Renderer m_renderer;
    Shader m_world_shader;
    TextureArray m_block_textures;
    Player m_player;
    Camera m_camera;
    WorldData m_world;
    bool m_is_running;
    UiContext m_ui_context;

    Shader m_ui_shader;
    Hotbar m_hotbar{.background = 1, .selector = 2};
    std::vector<UiElement> m_ui_elements;
    Texture2D m_crosshair_texture;
    Texture2D m_hotbar_texture;
    Texture2D m_hotbar_slot_active;
    UiQuadMesh m_ui_quad;

    BlockPlaceState m_place_state;
    BlockBreakState m_break_state;

  public:
    Application();
    ~Application();

    void run();
    void update_day_night_cycle(float time_elapsed, glm::vec3 &base_sun_direction, glm::vec3 &rotation_sun_axis);
    void update_ui();
};
