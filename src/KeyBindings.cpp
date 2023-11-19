#include <iostream>
#include <vector>
#include "KeyBindings.h"
#include "GLFW/glfw3.h"
#include "ReadFile.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << key << std::endl;
}

void assignbinding(std::vector<std::string> bind){
    for(unsigned int i = 0; i < bind.size(); i++)
      {
        std::cout << bind[i] << " ";
      }
}

void readkeybindings(){
    std::vector<std::string> bind;
    // read config file for keybinding
    #if defined _WIN32
    printf("windows");
    #elif defined __APPLE__
    char const *p = "keybinding_macos.txt";
    bind = readfile(p);
    assignbinding(bind);
    printf("macos");
    #elif defined __linux__
    printf("linux");
    #endif
}
