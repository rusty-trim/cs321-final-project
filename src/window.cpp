#include "window.hpp"
#include "SDL_error.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "glad/glad.h"
#include "renderer.hpp"
#include <stdexcept>
#include <string>

Window::Window(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    m_handle = SDL_CreateWindow(title, x, y, w, h, flags);

    if (m_handle == NULL)
    {
        throw std::runtime_error(std::string("Failed to create Window: ") + SDL_GetError());
    }

    SDL_GetWindowSize(m_handle, &m_width, &m_height);

    m_gl_context = SDL_GL_CreateContext(m_handle);

    if (m_gl_context == NULL)
    {
        SDL_DestroyWindow(m_handle);
        throw std::runtime_error(std::string("Failed to create OpenGL context: ") + SDL_GetError());
    }

    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_GL_DeleteContext(m_gl_context);
        SDL_DestroyWindow(m_handle);
        throw std::runtime_error(std::string("Failed to load OpenGL functions."));
    }

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Window::set_size(int width, int height)
{
    m_width = width;
    m_height = height;
    SDL_SetWindowSize(m_handle, width, height);
}

void Window::swap_buffers()
{
    SDL_GL_SwapWindow(m_handle);
}

Window::~Window()
{
    SDL_DestroyWindow(m_handle);
    SDL_GL_DeleteContext(m_gl_context);
}
