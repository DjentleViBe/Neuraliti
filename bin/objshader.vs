#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 ProjMat;
uniform vec3 startPos;
uniform vec2 size;
out vec3 ourColor;

void main()
{   
    vec3 localPos = vec3(aPos.x * size.x, aPos.y * size.y, 0.0) + startPos;
    gl_Position = ProjMat * vec4(localPos.x, localPos.y, localPos.z, 1.0);
    ourColor = aColor;

}
