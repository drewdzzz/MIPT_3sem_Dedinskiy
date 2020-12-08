#version 330 core

layout (location = 0) in vec2 position;
out vec3 vertexColor;

uniform vec3 inColor;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    vertexColor = inColor;
}