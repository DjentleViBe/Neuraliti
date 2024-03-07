#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 ProjMat;
out vec3 fColor;

void main()
{
    gl_Position = ProjMat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fColor = aColor;
}  
