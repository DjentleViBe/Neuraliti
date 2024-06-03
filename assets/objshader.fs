#version 330 core
out vec4 ObjColor;

in vec3 ourColor;
uniform vec3 aColor;

void main()
{
    ObjColor = vec4(aColor, 1.0);
}
