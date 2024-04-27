#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 ProjMat;
in vec3 position;
out vec3 shadcolor;
void main()
{
    gl_Position = ProjMat * vec4(position, 1.0);
    shadcolor = aColor;
    
}
