#shader vertex
#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in float a_layer;
layout(location = 3) in float a_shade;

uniform mat4 u_projection;
uniform mat4 u_model;

out vec2 f_tex_coord;
flat out float f_layer;
flat out float f_shade;

void main()
{
    gl_Position = u_projection * u_model * vec4(a_pos, 1.0);
    f_tex_coord = a_tex_coord;
    f_layer = a_layer;
    f_shade = a_shade;
}

#shader fragment
#version 330 core

in vec2 f_tex_coord;
flat in float f_layer;
flat in float f_shade;

uniform sampler2DArray u_textures;

out vec4 FragColor;

void main()
{
    vec4 tex = textureLod(u_textures, vec3(f_tex_coord, f_layer), 0.0);
    FragColor = vec4(tex.rgb * f_shade, tex.a);
}
