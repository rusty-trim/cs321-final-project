#include "graphics/shader.hpp"
#include "graphics/renderer.hpp"
#include <fstream>
#include <glad/glad.h>
#include <sstream>
#include <string>

Shader::Shader(const char *filepath)
{
    m_filepath = filepath;
    ShaderProgramSource source = parse_shader();
    m_programID = create_shader(source.vertex_source.c_str(), source.fragment_source.c_str());
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_programID));
}

ShaderProgramSource Shader::parse_shader()
{
    std::ifstream stream(m_filepath);

    enum ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return ShaderProgramSource{ss[0].str(), ss[1].str()};
}

unsigned int Shader::compile_shader(unsigned int type, const char *source)
{
    GLCall(unsigned int id = glCreateShader(type));
    GLCall(glShaderSource(id, 1, &source, NULL));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char *message = (char *)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        printf("Failed to compile %s shader: %s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

unsigned int Shader::create_shader(const char *vertex_shader, const char *fragment_shader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_programID));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::set_uniform1i(const char *name, int value)
{
    GLCall(glUniform1i(get_uniform_location(name), value));
}

void Shader::set_uniform1f(const char *name, float value)
{
    GLCall(glUniform1f(get_uniform_location(name), value));
}

void Shader::set_uniform4f(const char *name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

void Shader::set_uniform_mat4f(const char *name, glm::mat4 &mat)
{
    GLCall(glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::get_uniform_location(const char *name)
{
    if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
    {
        return m_uniform_location_cache[name];
    }

    GLCall(int location = glGetUniformLocation(m_programID, name));

    if (location == -1)
    {
        printf("Warning: uniform %s does not exist.\n", name);
    }
    else
    {
        m_uniform_location_cache[name] = location;
    }

    return location;
}
