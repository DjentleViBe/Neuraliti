#ifndef UIELEMENTS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UIELEMENTS_H
//#define OBJ_W 512.0f // in px
#define OBJ_H 64.0f // in px
//#define BMP_W 
#define BMP_H OBJ_H
#include <map>
#include "stdio.h"

extern float primary_color_1[3]; // grey
extern float primary_color_2[3]; 
extern float primary_color_3[3]; // white
extern float primary_color_4[3]; // orange
extern float primary_color_5[3]; // green
extern float primary_color_6[3]; // yellow
extern float primary_color_7[3]; // dark green

extern int window_width, window_height;
extern double zoomlevel;
extern std::map<std::string, std::string> appsettings;
extern std::string CurrentDir;
extern int globalfontsize;
extern std::vector<int> globalinlets;
extern std::vector<int> globaloutlets;

int INITgraphics();
void Displayloop();

#endif
