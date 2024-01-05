//
//  KeyBindings.hpp
//  GENT
//
//  Created by Vishakh Begari on 14.04.24.
//

#ifndef KeyBindings_h
#define KeyBindings_h
#include "GLFW/glfw3.h"

#include <stdio.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void readkeybindings();
#endif /* KeyBindings_hpp */
