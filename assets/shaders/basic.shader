#shader vertex
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in uint a_layer;

out vec2 f_tex_coord;
flat out uint f_layer;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    f_tex_coord = a_tex_coord;
    f_layer = a_layer;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 f_tex_coord;
flat in uint f_layer;

uniform sampler2DArray u_textures;

void main()
{
    // FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    FragColor = texture(u_textures, vec3(f_tex_coord, f_layer));
}
