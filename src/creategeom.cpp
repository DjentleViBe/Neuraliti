#include "creategeom.h"
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h"
#include "shader_s.h"
#include "stb_image.h"
#include "uielements.h"
#include "datatypes.hpp"
#include "loadfont.hpp"

void InitShader(const char* shadevs, const char* shadefs);

float* drawobject(float x, float y){
    // convert object pixels to screen sizes
    static float vertices[] = {
            // positions          // colors           // texture coords
            x + OBJ_W/(float)window_width,  y, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],  1.0f, 0.0f,  // top right
            x + OBJ_W/(float)window_width, y - OBJ_H/(float)window_height, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],  1.0f, 1.0f,  // bottom right
            x, y - OBJ_H/(float)window_height, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],   0.0f, 1.0f,   // bottom left
            x, y, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],    0.0f, 0.0f // top left
        };
    ;
    return vertices;
}

void InitShader(const char* shadevs, const char* shadefs){
    Shader ourShader(shadevs, shadefs);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = //stbi_load(std::filesystem::path("./UI_Elements/CabinV5.jpg").c_str(), &width, //&height, &nrChannels, 0);
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
    stbi_image_free(data);
}

int createobj(NeuralObj MyObj){
    // create the background box
    float *vertices;
    vertices = drawobject(MyObj.x, MyObj.y);
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
    // objects /////////////////
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 128, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    glGenTextures(1, &texture_obj);
    glBindTexture(GL_TEXTURE_2D, texture_obj);
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
        glGenVertexArrays(1, &VAOfont);
        glGenBuffers(1, &VBOfont);
        glGenBuffers(1, &EBOfont);

        glBindVertexArray(VAOfont);

        glBindBuffer(GL_ARRAY_BUFFER, VBOfont);
        glBufferData(GL_ARRAY_BUFFER, 128, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOfont);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
        glGenTextures(1, &texture_obj);
        glBindTexture(GL_TEXTURE_2D, texture_obj);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        stbi_set_flip_vertically_on_load(true);
        //int channels;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        unsigned char *map = loadfont("../assets/fonts/FreeMonoBold.ttf", "Hello World!");
        std::cout << &map;
        //GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, BMP_W, BMP_H, 0, GL_RED, GL_UNSIGNED_BYTE, map);
    }
    
    return 0;
}
