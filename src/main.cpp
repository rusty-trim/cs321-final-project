// TODO: Add file header for class
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <cstdio>

void print_err(const char *name, const char *error)
{
    fprintf(stderr, "Failed to initialize %s%s %s\n", 
            name, 
            (error == NULL) ? "" : ":", 
            (error == NULL) ? "" : error);
}

int main(int argc, char **argv)
{
    // Define variables
    SDL_Window *window = NULL;
    SDL_GLContext gl_context;
    SDL_Event event;
    bool is_running = true;

    // Handle initialization of video, audio, etc.
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        print_err("SDL", SDL_GetError());
        return 1;
    }

    // Configure OpenGL Context Attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Double buffering prevents screen tearing by drawing to an off-screen buffer 
    // first, then swapping it to the screen all at once during display.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Request a 24-bit Z-Buffer. It apparently tracks pixel depth so closer cube 
    // faces correctly obscure faces behind them.
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window.
    window = SDL_CreateWindow("Voxel Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // If window creation failed, print error and cleanup.
    if (window == NULL)
    {
        print_err("SDL", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create OpenGL context so that I can call gl functions.
    gl_context = SDL_GL_CreateContext(window);

    // If glContext creation failed, 
    if (!gl_context)
    {
        print_err("SDL", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        print_err("GLAD", NULL);
        return 1;
    }

    // Enable VSync
    SDL_GL_SetSwapInterval(1);

    // Enable depth test state on GPU. This prevents OpenGL from drawing triangles 
    // in whatever order they were submitted, which can canse back faces to overlap front faces.
    glEnable(GL_DEPTH_TEST);

    // Game loop logic
    while (is_running)
    {
        // Poll events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: 
                {
                    // End game loop when SDL emits an event that the game will be closing.
                    is_running = false;
                }
            }
        }

        glClearColor(0.2f, 0.6f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window);

    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
