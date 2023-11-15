#include <iostream>
#include "KeyBindings.h"
#include "GLFW/glfw3.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << key << std::endl;
}
