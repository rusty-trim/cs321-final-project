struct Texture2D
{
    unsigned int id = 0;
    int width = 0;
    int height = 0;
};

Texture2D load_texture_2d(const char *filepath);
void destroy_texture_2d(Texture2D &texture);
