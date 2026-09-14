#include "graphics/texture_buffer.hpp"
#include "renderer.hpp"
#include "vertex_array.hpp"
#include <cstdio>
#include <stb/stb_image.h>
#include <vector>

TextureBuffer::TextureBuffer(std::vector<const char *> &filepaths)
{
    m_tbo = 0;
    m_layer_count = filepaths.size();
    if (m_layer_count == 0) return;

    stbi_set_flip_vertically_on_load(1);

    // 1. Load images into host memory first to determine size
    int width = 0, height = 0, channels = 0;
    std::vector<unsigned char*> loaded_data(m_layer_count, nullptr);

    for (size_t i = 0; i < m_layer_count; i++) {
        loaded_data[i] = stbi_load(filepaths[i], &width, &height, &channels, 4);
        if (!loaded_data[i]) {
            printf("Failed to load image: %s\n", filepaths[i]);
        }
    }

    m_width = width;
    m_height = height;

    // 2. Allocate 3D storage with non-zero dimensions
    GLCall(glGenTextures(1, &m_tbo));
    GLCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_tbo));
    GLCall(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, m_width, m_height, m_layer_count, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

    // 3. Upload sub-images
    for (int layer = 0; layer < m_layer_count; layer++)
    {
        if (loaded_data[layer])
        {
            GLCall(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, m_width, m_height, 1, GL_RGBA, GL_UNSIGNED_BYTE, loaded_data[layer]));
            stbi_image_free(loaded_data[layer]);
        }
    }

    // 4. Set Texture Parameters using GL_TEXTURE_2D_ARRAY target
    GLCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

TextureBuffer::~TextureBuffer()
{
    GLCall(glDeleteTextures(1, &m_tbo));
}

void TextureBuffer::bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_tbo));
}

void TextureBuffer::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
}
