#shader vertex
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in uint a_layer;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform vec3 u_sun;

uniform vec3 u_camera_pos;
uniform vec3 u_sky_color;
uniform float u_fog_near;
uniform float u_fog_far;
uniform float u_ambient;

out vec2 f_tex_coord;
out vec3 f_normal;
out vec3 f_sun;
out vec3 f_world_pos;
flat out uint f_layer;

out vec3 f_camera_pos;
out vec3 f_sky_color;
out float f_fog_near;
out float f_fog_far;
out float f_ambient;

void main()
{
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    f_tex_coord = a_tex_coord;
    f_normal = a_normal;
    f_sun = u_sun;
    f_world_pos = vec3(u_model * vec4(a_pos, 1.0));
    f_layer = a_layer;

    f_camera_pos = u_camera_pos;
    f_sky_color = u_sky_color;
    f_fog_near = u_fog_near;
    f_fog_far = u_fog_far;
    f_ambient = u_ambient;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 f_tex_coord;
in vec3 f_normal;
in vec3 f_sun;
in vec3 f_world_pos;
flat in uint f_layer;

uniform sampler2DArray u_textures;
in vec3 f_camera_pos;
in vec3 f_sky_color;
in float f_fog_near;
in float f_fog_far;
in float f_ambient;

void main()
{
    vec4 tex_color = texture(u_textures, vec3(f_tex_coord, f_layer));

    vec3 n = normalize(f_normal);
    vec3 sun = normalize(f_sun);

    float face_shade;
    if (n.y > 0.5)              face_shade = 1.0;   // top
    else if (n.y < -0.5)        face_shade = 0.5;   // bottom
    else if (abs(n.z) > 0.5)    face_shade = 0.8;   // north/south
    else                        face_shade = 0.65;  // east/west

    float day = smoothstep(-0.1, 0.25, sun.y);
    float daylight = mix(0.18, 1.0, day);

    float sun_boost = max(dot(n, sun), 0.0) * 0.2 * day;

    vec3 lit_color = tex_color.rgb * (face_shade * daylight + sun_boost);

    float dist = length(f_world_pos - f_camera_pos);
    float fog = clamp((dist - f_fog_near) / (f_fog_far - f_fog_near), 0.0, 1.0);

    FragColor = vec4(mix(lit_color, f_sky_color, fog), tex_color.a);
}
