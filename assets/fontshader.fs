#version 330 core
out vec4 FragColor;

in vec3 fontColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
    if(texture(texture1, TexCoord).r == 0.0)
    {
        //FragColor = vec4(ourColor, 0.0);
        // If it's black, make it transparent
        //FragColor = vec4(1, 1, 1, 1);
        FragColor = vec4(fontColor, 0.0);
        //discard;
    }
    else{
        FragColor = vec4(fontColor, texture(texture1, TexCoord).r);
    
    }
}

