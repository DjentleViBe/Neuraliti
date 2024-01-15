#ifndef UIELEMENTS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UIELEMENTS_H
#define OBJ_W 512.0f // in px
#define OBJ_H 64.0f // in px
#define BMP_W OBJ_W
#define BMP_H OBJ_H

extern unsigned int VAO, VBO, EBO, VBOfont, VAOfont, EBOfont;
extern int window_width, window_height;
extern float primary_color_2[];
extern unsigned int texture_obj;
int INITgraphics();
void Displayloop(char **argv);

#endif
