#include "texture_2d.hpp"
#include "glad/glad.h"
#include "renderer.hpp"
#include "stb_image.h"

Texture2D load_texture_2d(const char *filepath)
{
    Texture2D texture;

    stbi_set_flip_vertically_on_load(1);

    int channels = 0;
    unsigned char *data = stbi_load(filepath, &texture.width, &texture.height, &channels, 4);

    if (data == NULL)
    {
        printf("Failed to load image: %s\n", filepath);
        return texture;
    }

    GLCall(glGenTextures(1, &texture.id));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.id));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    stbi_image_free(data);
    return texture;
}

void destroy_texture_2d(Texture2D &texture)
{
    GLCall(glDeleteTextures(1, &texture.id));
    texture = {};
}
