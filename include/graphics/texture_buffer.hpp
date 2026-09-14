#pragma once

#include <vector>

class TextureBuffer
{
    private:
        unsigned int m_tbo;
        unsigned char *m_local_buffer;
        int m_width, m_height, m_bpp, m_layer_count;
    
    public:
        TextureBuffer(std::vector<const char *> &filepaths);
        ~TextureBuffer();

        void bind(unsigned int slot = 0) const;
        void unbind() const;

        inline int get_width() const { return m_width; };
        inline int get_height() const { return m_height; };
};
