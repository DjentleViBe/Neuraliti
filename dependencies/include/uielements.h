#ifndef UIELEMENTS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UIELEMENTS_H
#define OBJ_W 512.0f // in px
#define OBJ_H 64.0f // in px
#define BMP_W OBJ_W
#define BMP_H OBJ_H
#include <map>
#include "stdio.h"

extern int window_width, window_height;
extern std::map<std::string, std::string> appsettings;
int INITgraphics();
void Displayloop(char **argv);

#endif
