#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

in vec2 position;
out vec3 shadcolor;
void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    shadcolor = aColor;
    
}
