#shader vertex
#version 330 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_tex_coord;

uniform mat4 u_projection;
uniform mat4 u_model;

out vec2 f_tex_coord;

void main()
{
    gl_Position = u_projection * u_model * vec4(a_pos, 0.0, 1.0);
    f_tex_coord = a_tex_coord;
}

#shader fragment
#version 330 core

in vec2 f_tex_coord;

uniform sampler2D u_texture;
uniform vec4 u_color;

out vec4 FragColor;

void main()
{
   FragColor = texture(u_texture, f_tex_coord) * u_color; 
}
