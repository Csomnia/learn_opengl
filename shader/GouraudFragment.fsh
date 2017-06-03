#version 330 core
in vec3 vertexCalulatedLightColor;

out vec4 color;

uniform vec3 objectColor;

void main()
{
    color = vec4(vertexCalulatedLightColor * objectColor, 1.0f);
}
