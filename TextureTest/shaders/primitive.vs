#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 inColor;
out vec3 vertexColor;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    vertexColor = inColor;
}