#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
    if(texture(texture1, TexCoord).r == 0.0)
    {
        // If it's black, make it transparent
        FragColor = vec4(1, 1, 1, 1);
        //discard;
    }
    else{
        // linearly interpolate between both textures (80% container, 20% awesomeface)
        //FragColor = vec4(texture(texture1, TexCoord).r * ourColor[0], texture(texture1, TexCoord).r* ourColor[0], texture(texture1, TexCoord).r * ourColor[0], 1.0);
        FragColor = vec4(ourColor, texture(texture1, TexCoord).r);
    
    }
}
