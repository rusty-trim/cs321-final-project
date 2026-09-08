#include "SDL2/SDL.h"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "graphics/camera.hpp"
#include "graphics/element_buffer.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"
#include "graphics/vertex_buffer.hpp"
#include <cstdio>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main()
{
    bool is_running = true;
    SDL_Window *window = NULL;
    SDL_GLContext gl_context;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
        "Voxel Engine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    gl_context = SDL_GL_CreateContext(window);

    if (gl_context == NULL)
    {
        printf("Failed to create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("Failed to load GL functions\n");
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    float vertices[] = {
        // Front Face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        // Back Face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f};

    unsigned int indices[] = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Right
        1, 5, 6, 6, 2, 1,
        // Back
        5, 4, 7, 7, 6, 5,
        // Left
        4, 0, 3, 3, 7, 4,
        // Top
        3, 2, 6, 6, 7, 3,
        // Bottom
        4, 5, 1, 1, 0, 4};

    VertexArray va;
    VertexBuffer vb(vertices, 8 * 3 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);

    va.add_buffer(vb, layout);

    ElementBuffer eb(indices, 36);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    float fov = glm::radians(70.0f);
    float aspect_ratio = (float)width / (float)height;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;

    glm::mat4 proj = glm::perspective(fov, aspect_ratio, near_plane, far_plane);

    glm::vec3 camera_pos = glm::vec3(0.0f, 2.0f, 5.0f);
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(camera_pos, camera_target, up_vector);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 mvp = proj * view * model;

    Shader shader("assets/shaders/basic.shader");
    shader.bind();

    va.unbind();
    shader.unbind();
    vb.unbind();
    eb.unbind();

    Renderer renderer;
    Camera camera(camera_pos, camera_target, up_vector);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    Uint64 last_time = SDL_GetPerformanceCounter();

    while (is_running)
    {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - last_time) / SDL_GetPerformanceFrequency();
        last_time = now;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    is_running = false;

                break;
            }
        }

        glClearColor(0.51f, 0.78f, 0.9f, 1.0f);
        renderer.clear();

        int xrel = 0, yrel = 0;
        SDL_GetRelativeMouseState(&xrel, &yrel);

        if (xrel != 0 || yrel != 0)
        {
            camera.handle_mouse(xrel, yrel);
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        SDL_Keymod mod_state = SDL_GetModState();

        if (state[SDL_SCANCODE_W])
            camera.handle_keyboard(CameraDirection::FORWARD, dt, mod_state & KMOD_SHIFT);
        if (state[SDL_SCANCODE_S])
            camera.handle_keyboard(CameraDirection::BACKWARD, dt, mod_state & KMOD_SHIFT);
        if (state[SDL_SCANCODE_A])
            camera.handle_keyboard(CameraDirection::LEFT, dt, mod_state & KMOD_SHIFT);
        if (state[SDL_SCANCODE_D])
            camera.handle_keyboard(CameraDirection::RIGHT, dt, mod_state & KMOD_SHIFT);
        if (state[SDL_SCANCODE_LCTRL])
            camera.handle_keyboard(CameraDirection::DOWN, dt);
        if (state[SDL_SCANCODE_SPACE])
            camera.handle_keyboard(CameraDirection::UP, dt);

        mvp = proj * camera.get_view_matrix() * model;

        shader.bind();
        shader.set_uniform_mat4f("u_mvp", mvp);
        renderer.draw(va, eb, shader);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
