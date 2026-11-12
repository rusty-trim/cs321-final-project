#pragma once

#include "SDL_video.h"
#include <SDL2/SDL.h>

class Window
{
  private:
    SDL_Window *m_handle;
    SDL_GLContext m_gl_context;
    int m_width;
    int m_height;

  public:
    Window(const char *title, int x, int y, int w, int h, Uint32 flags);
    ~Window();

    int get_width() const
    {
        return m_width;
    };

    int get_height() const
    {
        return m_height;
    };

    SDL_Window *get_handle() const
    {
        return m_handle;
    }

    void set_size(int width, int height);
    void swap_buffers();
};
