#version 330 core
out vec4 ObjColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform vec3 aColor;
uniform float select;
// texture samplers
uniform sampler2D texture1;

void main()
{
    ObjColor = vec4(aColor, 1.0);
}
