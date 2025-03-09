#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
void main()
{
   TexCoords = aTexCoords;
   gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
   FragPos = vec3(u_model * vec4(position, 1.0));
   Normal = mat3(transpose(inverse(u_model))) * aNormal;
};

#shader fragment
#version 330 core

out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
uniform vec3 u_color;
uniform vec3 u_lightColor;
uniform vec3 u_lightPosition;
uniform sampler2D u_texDiffuse;
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  diff * u_lightColor;

    vec3 result = (ambient + diffuse) * u_color;

    color = vec4(result, 1.0);
};