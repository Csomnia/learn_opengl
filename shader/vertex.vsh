#version 330 core
layout (location = 0) in vec3 position; // 位置变量的属性位置值为 0
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor; // 向片段着色器输出一个颜色
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoord = texCoord;
}

