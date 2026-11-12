#pragma once
class SDLContext
{
  public:
    SDLContext();
    ~SDLContext();

    SDLContext(const SDLContext &) = delete;
    SDLContext &operator=(const SDLContext &) = delete;
};
