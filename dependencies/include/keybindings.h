#ifndef keybindings_h
#define keybindings_h

#include <stdio.h>
#include "GLFW/glfw3.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void readkeybindings();
#endif /* keybindings_h */
