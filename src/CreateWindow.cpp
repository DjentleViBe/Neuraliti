

#include "CreateWindow.hpp"
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h"
#include "shader_s.h"
#include "stb_image.h"
void InitShader(const char* shadevs, const char* shadefs);

void DrawRectangle(){
    //glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
     
    //glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //Reset the drawing perspective
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS); //Begin quadrilateral coordinates
     
    //Trapezoid
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
 
    glEnd(); //End quadrilateral coordinates
    glFlush();
}

int InitRectangle(size_t vert, float vertices[], size_t ind, unsigned int indices[], unsigned int &texture, unsigned int &VBO, unsigned int &VAO, unsigned int &EBO, const char* shadevs, const char* shadefs){
    
    // objects /////////////////
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    // texture 1
    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    InitShader(shadevs, shadefs);
    return 0;
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
