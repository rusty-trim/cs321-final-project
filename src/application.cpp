#include "application.hpp"
#include "SDL_scancode.h"
#include "SDL_video.h"
#include "entity/player.hpp"
#include "game/inventory.hpp"
#include "glad.h"
#include "matrix_transform.hpp"
#include "vector_float2.hpp"
#include "world/block.hpp"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const int LOAD_RADIUS = 8;

const int CROSSHAIR_SIZE = 8;
const int HOTBAR_WIDTH = 144, HOTBAR_HEIGHT = 16;

MoveInput read_move_input()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_Keymod mod_state = SDL_GetModState();
    MoveInput in;

    in.sprint = mod_state & KMOD_SHIFT;

    if (state[SDL_SCANCODE_W])
        in.forward += 1.0f;
    if (state[SDL_SCANCODE_S])
        in.forward -= 1.0f;
    if (state[SDL_SCANCODE_A])
        in.right -= 1.0f;
    if (state[SDL_SCANCODE_D])
        in.right += 1.0f;
    if (state[SDL_SCANCODE_SPACE])
        in.up += 1.0f;
    if (state[SDL_SCANCODE_LCTRL])
        in.up -= 1.0f;

    return in;
}

Application::Application()
    : m_sdl(),
      m_window(Window("Voxel Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED |
                          SDL_WINDOW_OPENGL)),
      m_world_shader("assets/shaders/world.shader"), m_block_textures(get_block_texture_paths()),
      m_player(glm::vec3(0.0f, 50.0f, 5.0f)),
      m_camera(glm::radians(70.0f), (float)m_window.get_width() / (float)m_window.get_height(),
               0.1f, 1000.0f),

      m_ui_context({.screen_width = m_window.get_width(),
                    .screen_height = m_window.get_height(),
                    .scale = 1}),
      m_ui_shader("assets/shaders/ui.shader"),
      m_crosshair_texture(load_texture_2d("assets/textures/ui/crosshair.png")),
      m_hotbar_texture(load_texture_2d("assets/textures/ui/hotbar.png")),
      m_hotbar_slot_active(load_texture_2d("assets/textures/ui/hotbar_slot_active.png")),
      m_ui_quad(create_unit_quad_mesh())
{
    init_block_type_table();

    std::cout << m_ui_context.scale << std::endl;
    m_ui_elements.push_back({&m_crosshair_texture,
                             glm::vec2(0.0f),
                             glm::vec2(CROSSHAIR_SIZE * m_ui_context.scale),
                             {1.0f, 1.0f, 1.0f, 1.0f},
                             CENTER});
    m_ui_elements.push_back(
        {&m_hotbar_texture,
         glm::vec2(0.0f, 1.0f),
         glm::vec2(HOTBAR_WIDTH * m_ui_context.scale, HOTBAR_HEIGHT * m_ui_context.scale),
         {1.0f, 1.0f, 1.0f, 1.0f},
         BOTTOM_CENTER});
    m_ui_elements.push_back({&m_hotbar_slot_active,
                             glm::vec2((-16.0f * m_ui_context.scale) * 4, 1.0f),
                             glm::vec2(16.0f * m_ui_context.scale, 16.0f * m_ui_context.scale),
                             {1.0f, 1.0f, 1.0f, 1.0f},
                             BOTTOM_CENTER});

    Inventory &inv = m_player.get_inventory();
    inv.hotbar[0] = {BLOCK_GRASS, 1};
    inv.hotbar[1] = {BLOCK_DIRT, 1};
    inv.hotbar[2] = {BLOCK_STONE, 1};
    inv.hotbar[3] = {BLOCK_OAK_LOG, 1};
    inv.hotbar[4] = {BLOCK_OAK_PLANKS, 1};
    inv.hotbar[5] = {BLOCK_AIR, 1};
    inv.hotbar[6] = {BLOCK_AIR, 1};
    inv.hotbar[7] = {BLOCK_AIR, 1};
};

void Application::update_day_night_cycle(float time_elapsed, glm::vec3 &base_sun_direction,
                                         glm::vec3 &rotation_sun_axis)
{
    float time_speed = 0.01f;
    float sun_angle = 0.0f + (time_elapsed * time_speed);

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), sun_angle, rotation_sun_axis);
    glm::vec3 light_direction = glm::vec3(rotation * glm::vec4(-base_sun_direction, 0.0f));

    float sun_height = glm::dot(light_direction, glm::vec3(0.0f, 1.0f, 0.0f));
    float day_factor = glm::clamp(sun_height, 0.0f, 1.0f); // 0 at night, 1 at full day

    m_world_shader.set_uniform3f("u_sun", light_direction.x, light_direction.y, light_direction.z);

    glm::vec3 day_sky(0.51f, 0.78f, 0.9f);
    glm::vec3 night_sky(0.02f, 0.02f, 0.08f);
    glm::vec3 sky_color = glm::mix(night_sky, day_sky, day_factor);

    m_world_shader.set_uniform3f("u_camera_pos", m_camera.get_position().x,
                                 m_camera.get_position().y, m_camera.get_position().z);
    m_world_shader.set_uniform3f("u_sky_color", sky_color.r, sky_color.g, sky_color.b);
    m_world_shader.set_uniform1f("u_fog_near", 80.0f);
    m_world_shader.set_uniform1f("u_fog_far", 130.0f);

    GLCall(glClearColor(sky_color.r, sky_color.g, sky_color.b, 1.0f));
    m_renderer.clear();
}

void Application::update_ui()
{
    GLCall(glDisable(GL_DEPTH_TEST));

    int screen_w = m_window.get_width(), screen_h = m_window.get_height();

    glm::mat4 ui_projection = glm::ortho(0.0f, (float)screen_w, (float)screen_h, 0.0f, -1.0f, 1.0f);

    m_ui_shader.bind();
    m_ui_shader.set_uniform_mat4f("u_projection", ui_projection);
    m_ui_shader.set_uniform1i("u_texture", 0);

    for (const UiElement element : m_ui_elements)
    {
        draw_ui_element(element, m_ui_quad, m_ui_shader, screen_w, screen_h);
    }

    GLCall(glEnable(GL_DEPTH_TEST));
}

void Application::run()
{
    SDL_Event event;
    float time_elapsed = 0;
    m_is_running = true;

    SDL_SetRelativeMouseMode(SDL_TRUE);
    Uint64 last_time = SDL_GetPerformanceCounter();

    glm::vec3 base_sun_direction(0.0f, -1.0f, 0.0f);
    glm::vec3 rotation_sun_axis(1.0f, 0.0f, 0.0f);

    while (m_is_running)
    {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - last_time) / SDL_GetPerformanceFrequency();
        last_time = now;

        time_elapsed += dt;

        update_world_streaming(m_world, m_camera.get_position(), LOAD_RADIUS);

        Inventory &inv = m_player.get_inventory();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                m_is_running = false;
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    m_is_running = false;
                    break;
                case SDL_SCANCODE_M:
                    m_player.set_gamemode(GameMode((m_player.get_gamemode() + 1) % GAMEMODE_COUNT));
                    break;
                case SDL_SCANCODE_1:
                case SDL_SCANCODE_2:
                case SDL_SCANCODE_3:
                case SDL_SCANCODE_4:
                case SDL_SCANCODE_5:
                case SDL_SCANCODE_6:
                case SDL_SCANCODE_7:
                case SDL_SCANCODE_8:
                case SDL_SCANCODE_9:
                    select_hotbar_slot(inv, event.key.keysym.scancode - SDL_SCANCODE_1);
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEWHEEL:
            {
                if (event.wheel.y < 0)
                    select_hotbar_slot(inv, inv.selected + 1);
                else if (event.wheel.y > 0)
                    select_hotbar_slot(inv, inv.selected - 1);

                break;
            }
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    int w, h;
                    SDL_GL_GetDrawableSize(m_window.get_handle(), &w, &h);

                    if (h == 0)
                        break;

                    m_window.set_size(w, h);
                    glViewport(0, 0, w, h);
                    m_camera.set_aspect_ratio((float)event.window.data1 / event.window.data2);
                    m_ui_context.scale = std::max(1, h / 240);
                    m_ui_context.screen_width = w;
                    m_ui_context.screen_height = h;

                    m_ui_elements[0].size = glm::vec2(CROSSHAIR_SIZE * m_ui_context.scale);
                    m_ui_elements[1].size = glm::vec2(HOTBAR_WIDTH * m_ui_context.scale,
                                                      HOTBAR_HEIGHT * m_ui_context.scale);
                    m_ui_elements[2].position = glm::vec2((-16.0f * m_ui_context.scale) * 4, 1.0f);
                    m_ui_elements[2].size = glm::vec2(HOTBAR_HEIGHT * m_ui_context.scale);
                }
                break;
            }
        }

        sync_hotbar_ui(m_hotbar, m_ui_elements, inv, m_ui_context);

        int xrel = 0, yrel = 0;
        Uint32 buttons = SDL_GetRelativeMouseState(&xrel, &yrel);

        update_block_breaking(m_break_state, m_world, m_player,
                              buttons & SDL_BUTTON(SDL_BUTTON_LEFT), dt);
        update_block_placing(m_place_state, m_world, m_player,
                             buttons & SDL_BUTTON(SDL_BUTTON_RIGHT),
                             get_selected_block(m_player.get_inventory()), dt);

        m_player.look(xrel, yrel);
        m_player.update(read_move_input(), dt);
        m_camera.set_pose(m_player.get_eye_position(), m_player.get_yaw(), m_player.get_pitch());

        glm::mat4 view_projection = m_camera.get_proj_matrix() * m_camera.get_view_matrix();

        m_world_shader.bind();
        m_block_textures.bind();
        m_world_shader.set_uniform1i("u_textures", 0);

        update_day_night_cycle(time_elapsed, base_sun_direction, rotation_sun_axis);

        update_world_streaming(m_world, m_camera.get_position(), LOAD_RADIUS);
        rebuild_dirty_meshes(m_world);

        for (auto &[coord, mesh] : m_world.chunk_meshes)
        {
            glm::mat4 mvp = view_projection * mesh.model_matrix;
            m_world_shader.set_uniform_mat4f("u_mvp", mvp);
            m_world_shader.set_uniform_mat4f("u_model", mesh.model_matrix);
            draw_chunk_mesh(mesh);
        }

        update_ui();

        m_window.swap_buffers();
    }
}

Application::~Application()
{
    destroy_world(m_world);
    destroy_texture_2d(m_crosshair_texture);
    destroy_texture_2d(m_hotbar_texture);
    destroy_ui_quad_mesh(m_ui_quad);
};
