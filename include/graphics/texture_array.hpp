#pragma once

#include <vector>

class TextureArray
{
  private:
    unsigned int m_tbo;
    unsigned char *m_local_buffer;
    size_t m_width, m_height, m_bpp, m_layer_count;

  public:
    TextureArray(const std::vector<const char *> &filepaths);
    ~TextureArray();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int get_width() const
    {
        return m_width;
    };
    inline int get_height() const
    {
        return m_height;
    };
};
