#include "../dependencies/include/creategeom.h"
#define GL_SILENCE_DEPRECATION
#include "../dependencies/include/GLFW/glfw3.h"
#include "../dependencies/include/shader_s.h"
#include "../dependencies/include/stb_image.h"
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/loadfont.hpp"
#include <string>

void InitShader(const char* shadevs, const char* shadefs);

float* drawobject(float x, float y, float* color, float vertices[]){
    vertices[0] = x + OBJ_W/(float)window_width;
    vertices[1] = y;
    vertices[2] = 0.0f;
    vertices[3] = color[0];
    vertices[4] = color[1];
    vertices[5] = color[2];
    vertices[6] = 1.0;
    vertices[7] = 0.0;
    vertices[8] = x + OBJ_W/(float)window_width;
    vertices[9] = y - OBJ_H/(float)window_height;
    vertices[10] = 0.0f;
    vertices[11] = color[0];
    vertices[12] = color[1];
    vertices[13] = color[2];
    vertices[14] = 1.0f;
    vertices[15] = 1.0f;
    vertices[16] = x;
    vertices[17] = y - OBJ_H/(float)window_height;
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

NeuralObj createobj(NeuralObj &MyObj){
    // create the background box
    
    //float *vertices;
    drawobject(MyObj.x, MyObj.y, MyObj.color, MyObj.verts);
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
    // objects /////////////////
    glGenVertexArrays(1, &MyObj.VAO);
    glGenBuffers(1, &MyObj.VBO);
    glGenBuffers(1, &MyObj.EBO);
    glBindVertexArray(MyObj.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, MyObj.VBO);
    glBufferData(GL_ARRAY_BUFFER, 128, MyObj.verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyObj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &MyObj.texture);
    glBindTexture(GL_TEXTURE_2D, MyObj.texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // if object type is 0
    // render font
    if(MyObj.objtype == 0){
        
        stbi_set_flip_vertically_on_load(true);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        unsigned char *map = loadfont((CurrentDir + "/assets/fonts/" + appsettings["defaultfont"]).c_str(), MyObj.objname);
        //GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, BMP_W, BMP_H, 0, GL_RED, GL_UNSIGNED_BYTE, map);
    }
    return MyObj;
}
