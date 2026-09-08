#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string vertex_source;
    std::string fragment_source;
};

class Shader
{
  private:
    unsigned int m_programID;
    const char *m_filepath;
    std::unordered_map<const char *, int> m_uniform_location_cache;

  public:
    Shader(const char *filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void set_uniform1i(const char *name, int value);
    void set_uniform1f(const char *name, float value);
    void set_uniform4f(const char *name, float v0, float v1, float v2, float v3);
    void set_uniform_mat4f(const char *name, glm::mat4 &mat);

    private:
        ShaderProgramSource parse_shader();
        unsigned int compile_shader(unsigned int type, const char *source);
        unsigned int create_shader(const char *vertex_shader, const char *fragment_shader);
        int get_uniform_location(const char *name);
};
