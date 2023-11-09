#version 330 core
in vec3 shadcolor;
out vec4 outColor;

void main()
{
    outColor = vec4(shadcolor, 1.0);
}
