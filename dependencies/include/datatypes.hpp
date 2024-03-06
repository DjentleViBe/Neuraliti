#ifndef datatypes_hpp
#define datatypes_hpp
#include "stdio.h"
#include <string>
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <vector>

class NeuralObj {       // The class
  public:               // Access specifier
    int objtype;        // 0: Object, 1: bang, 2: float
    float x;            // x position
    float y;            // y position
    std::string objname;  // Attribute (string variable)
    unsigned int VAO, VBO, EBO;
    unsigned int inquadVAO, inquadVBO, instanceVBO;
    unsigned int outquadVAO, outquadVBO, outstanceVBO;
    unsigned int texture;
    float* color;
    float verts[32];
    GLuint Matrix;
    int Inletnum;
    int Outletnum;
    int *Inlets;        // store inlet types
    int *Outlets;       // store outlet types
};

#endif
