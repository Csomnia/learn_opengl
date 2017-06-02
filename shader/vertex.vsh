#version 330 core
layout (location = 0) in vec3 position; // 位置变量的属性位置值为 0
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(position, 1.0f));
}

