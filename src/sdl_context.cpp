#include "sdl_context.hpp"
#include "SDL2/SDL.h"
#include "SDL_video.h"
#include <stdexcept>
#include <string>

SDLContext::SDLContext()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

SDLContext::~SDLContext()
{
    SDL_Quit();
}
