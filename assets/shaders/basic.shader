#shader vertex
#version 330 core

layout (location = 0) in vec3 a_pos;
uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(a_pos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
