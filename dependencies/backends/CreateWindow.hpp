
#ifndef CreateWindow_hpp
#define CreateWindow_hpp

#include <stdio.h>
void DrawRectangle();
int InitRectangle(size_t vert, float vertices[], size_t ind, unsigned int indices[], unsigned int &texture, unsigned int &VBO, unsigned int &VAO, unsigned int &EBO, const char* shadevs, const char* shadefs);
#endif /* CreateWindow_hpp */
