#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_mvp;
void main()
{
   gl_Position = u_mvp * vec4(position, 1.0);
};

#shader fragment
#version 330 core

out vec4 color;
uniform vec3 u_color;
void main()
{
    color = vec4(u_color, 1.0);
};