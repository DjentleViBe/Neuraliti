#include "../dependencies/include/creategeom.h"
#define GL_SILENCE_DEPRECATION
#include "../dependencies/include/GLFW/glfw3.h"
#include "../dependencies/include/shader_s.h"
#include "../dependencies/include/stb_image.h"
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/loadfont.hpp"
#include <string>
#include "../dependencies/include/glm/glm.hpp"
#include "../dependencies/include/glm/gtc/matrix_transform.hpp"
#include "../dependencies/include/glm/gtc/type_ptr.hpp"

// unsigned char *map;

void InitShader(const char* shadevs, const char* shadefs);

float* drawnodes(float wid, float* color){
    float* quadVertices = new float[30]{
    // positions     // colors
    0.0f,  wid,  color[0], color[1], color[2],
     wid, 0.0f,  color[0], color[1], color[2],
    0.0f, 0.0f,  color[0], color[1], color[2],

    0.0f,  wid,  color[0], color[1], color[2],
     wid, 0.0f,  color[0], color[1], color[2],   
     wid,  wid,  color[0], color[1], color[2]		    		
    }; 
    return quadVertices;
}

float* drawobject(float x, float y, float* color, float vertices[], float sent_width, float sent_height){
    
    vertices[0] = x + sent_width/(float)window_width;
    vertices[1] = y;
    vertices[2] = 0.0f;
    vertices[3] = color[0];
    vertices[4] = color[1];
    vertices[5] = color[2];
    vertices[6] = 1.0;
    vertices[7] = 0.0;
    vertices[8] = x + sent_width/(float)window_width;
    vertices[9] = y - sent_height/(float)window_height;
    vertices[10] = 0.0f;
    vertices[11] = color[0];
    vertices[12] = color[1];
    vertices[13] = color[2];
    vertices[14] = 1.0f;
    vertices[15] = 1.0f;
    vertices[16] = x;
    vertices[17] = y - sent_height/(float)window_height;
    vertices[18] = 0.0f;
    vertices[19] = color[0];
    vertices[20] = color[1];;
    vertices[21] = color[2];;
    vertices[22] = 0.0f;
    vertices[23] = 1.0f;
    vertices[24] = x;
    vertices[25] = y;
    vertices[26] = 0.0f;
    vertices[27] = color[0];
    vertices[28] = color[1];
    vertices[29] = color[2];
    vertices[30] = 0.0f;
    vertices[31] = 0.0f;

    return vertices;
}

float* drawline(float wid, float x1, float y1, float x2, float y2, float* color){
    float slope = 1.5708 - atan((y2 - y1) / (x2 - x1));
    float y = sin(slope) * wid;
    float x = cos(slope) * wid;
    float* quadLines = new float[24]{
    // positions     // colors
    x1, y1, color[0], color[1], color[2], // top right
    x2, y2, color[0], color[1], color[2],// bottom right
    x2 + x, y2 - y, color[0], color[1], color[2],// bottom left
    x1 + x, y1 - y, color[0], color[1], color[2]	// top left	
    }; 
    return quadLines;
}

void InitShader(const char* shadevs, const char* shadefs){
    Shader ourShader(shadevs, shadefs);
    // load image, create texture and generate mipmaps
    // int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    /*unsigned char *data = //stbi_load(std::filesystem::path("./UI_Elements/CabinV5.jpg").c_str(), &width, //&height, &nrChannels, 0);
        //std::string filename("CabinV5.jpg");
        stbi_load(std::filesystem::path("CabinV5.jpg").c_str(), &width, &height, &nrChannels, 0);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);*/
}

NeuralObj createinlets(NeuralObj &MyObj){
    glm::vec2 translations[MyObj.Inletnum];
    int index = 0;
    float offset = 0.0f;
    
    for (int x = 0; x <= MyObj.Inletnum; x++)
        {
            glm::vec2 translation;
            translation.x = MyObj.x + offset;
            translation.y = MyObj.y;
            translations[index++] = translation;
            offset += -0.02f/(MyObj.Inletnum - 1) + (fmax(MyObj.objectwidth, globalfontsize * 3.0) / (window_width * (MyObj.Inletnum - 1)));
        }
    // store instance data in an array buffer
    // --------------------------------------
    glGenBuffers(1, &MyObj.instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, MyObj.instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 10, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float *quadVertices = drawnodes(0.02f, primary_color_4);
    glGenVertexArrays(1, &MyObj.inquadVAO);
    glGenBuffers(1, &MyObj.inquadVBO);
    glBindVertexArray(MyObj.inquadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, MyObj.inquadVBO);
    glBufferData(GL_ARRAY_BUFFER, 120, quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, MyObj.instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    return MyObj;
}

NeuralObj createoutlets(NeuralObj &MyObj){
    glm::vec2 translations[MyObj.Outletnum];
    int index = 0;
    float offset = 0.0f;

    for (int x = 0; x <= MyObj.Outletnum; x++)
        {
            glm::vec2 translation;
            translation.x = MyObj.x + offset;
            //translation.y = (float)y / 10.0f + offset;
            translation.y = MyObj.y - 0.02f - globalfontsize * 2.3/window_height;
            translations[index++] = translation;
            offset += fmax(MyObj.objectwidth, globalfontsize * 3.0) / window_width - 0.02f;
        }
        
    // store instance data in an array buffer
    // --------------------------------------
    glGenBuffers(1, &MyObj.outstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, MyObj.outstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 10, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float *quadVertices;
    if(MyObj.objtype != 0){
        quadVertices = drawnodes(0.02f, primary_color_9);
    }
    else{
        quadVertices = drawnodes(0.02f, primary_color_5);
    }
    
    glGenVertexArrays(1, &MyObj.outquadVAO);
    glGenBuffers(1, &MyObj.outquadVBO);
    glBindVertexArray(MyObj.outquadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, MyObj.outquadVBO);
    glBufferData(GL_ARRAY_BUFFER, 120, quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, MyObj.outstanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    return MyObj;
}

void updateTexture(unsigned int textureID, unsigned char* map, float sw){
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fmax(sw, globalfontsize * 3.0), globalfontsize * 3.0, 0, GL_RED, GL_UNSIGNED_BYTE, map);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

NeuralLines createlines(NeuralLines &MyObj){
    float *lineVertices = drawline(0.003, MyObj.startx, MyObj.starty, MyObj.endx, MyObj.endy, primary_color_2);

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

    glGenVertexArrays(1, &MyObj.VAO);
    glGenBuffers(1, &MyObj.VBO);
    glGenBuffers(1, &MyObj.EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(MyObj.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, MyObj.VBO);
    glBufferData(GL_ARRAY_BUFFER, 80, lineVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyObj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    return MyObj;
}

NeuralLines createhorizontallines(NeuralLines &MyObj){
    float vertices[] = {
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        0.0f, 1.0f, // Top-left
        1.0f, 1.0f  // Top-right
    };

    glGenVertexArrays(1, &MyObj.VAO);
    glGenBuffers(1, &MyObj.VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s).
    glBindVertexArray(MyObj.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, MyObj.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return MyObj;
}

NeuralObj createhorizontallines(NeuralObj &MyObj){
    MyObj.objectwidth = MyObj.objdisplayname.length() * (float)globalfontsize * 2.0;
    MyObj.sentencewidth = (float)MyObj.objectwidth/(float)window_width;
    MyObj.sentenceheight = (float)globalfontsize * 2.3/(float)window_height;
    float vertices[] = {
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        0.0f, 1.0f, // Top-left
        1.0f, 1.0f  // Top-right
    };

    glGenVertexArrays(1, &MyObj.VAO);
    glGenBuffers(1, &MyObj.VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s).
    glBindVertexArray(MyObj.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, MyObj.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    createinlets(MyObj);
    createoutlets(MyObj);
    return MyObj;
}

NeuralObj createhorizontalfonts(NeuralObj &MyObj){
    MyObj.objectwidth = MyObj.objdisplayname.length() * (float)globalfontsize * 2.0;
    MyObj.sentencewidth = (float)MyObj.objectwidth/(float)window_width;
    MyObj.sentenceheight = (float)globalfontsize * 2.3/(float)window_height;

    float vertices[] = {
       // positions    // texture coords
        0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f,
        0.0f,  1.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &MyObj.VAO);
    glGenBuffers(1, &MyObj.VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s).
    glBindVertexArray(MyObj.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, MyObj.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyObj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glGenTextures(1, &MyObj.texture);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    FontBitmap fontBit = loadfont((CurrentDir + "/assets/fonts/" + appsettings["defaultfont"]).c_str(), MyObj.objdisplayname);
    updateTexture(MyObj.texture, fontBit.bitmap, MyObj.objectwidth);
    free(fontBit.bitmap);

    return MyObj;
}