#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 ProjMat;
in vec3 position;
out vec3 fontColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    fontColor = aColor;
    TexCoord = aTexCoord;
}

