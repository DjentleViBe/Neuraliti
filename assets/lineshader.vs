#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 ProjMat;
uniform vec3 startPos;
uniform vec3 endPos;
uniform float angle;
out vec3 fColor;

void main()
{   
    vec3 size = endPos - startPos;
    vec3 localPos = vec3(aPos.x * size.x, aPos.y * size.y, 0.0) + startPos;
    // vec3 pos = mix(startPos, endPos, aPos);
    // Translate the vertex to the center
    localPos -= startPos;
    
    // Apply rotation
    float s = sin(angle);
    float c = cos(angle);
    vec3 rotatedPos = vec3(
        localPos.x * c - localPos.y * s,
        localPos.x * s + localPos.y * c,
        0.0
    );

    // Translate back from the center
    localPos = rotatedPos + startPos;
    //pos = rotatedPos;

    gl_Position = ProjMat * vec4(localPos.x, localPos.y, localPos.z, 1.0);
    fColor = aColor;
}  
