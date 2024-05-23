#include <iostream>
#include <vector>
#include <map>
#include "../dependencies/include/keybindings.h"
#include "../dependencies/include/GLFW/glfw3.h"
#include "../dependencies/include/extras.h"
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/fileoperations.hpp"
#include "../dependencies/include/datatypes.hpp"

#define     GLFW_KEY_SPACE          32
#define     GLFW_KEY_APOSTROPHE     39 /* ' */
#define     GLFW_KEY_COMMA          44 /* , */
#define     GLFW_KEY_MINUS          45 /* - */
#define     GLFW_KEY_PERIOD         46 /* . */
#define     GLFW_KEY_SLASH          47 /* / */
#define     GLFW_KEY_0              48
#define     GLFW_KEY_1              49
#define     GLFW_KEY_2              50
#define     GLFW_KEY_3              51
#define     GLFW_KEY_4              52
#define     GLFW_KEY_5              53
#define     GLFW_KEY_6              54
#define     GLFW_KEY_7              55
#define     GLFW_KEY_8              56
#define     GLFW_KEY_9              57
#define     GLFW_KEY_SEMICOLON      59 /* ; */
#define     GLFW_KEY_EQUAL          61 /* = */
#define     GLFW_KEY_A              65
#define     GLFW_KEY_B              66
#define     GLFW_KEY_C              67
#define     GLFW_KEY_D              68
#define     GLFW_KEY_E              69
#define     GLFW_KEY_F              70
#define     GLFW_KEY_G              71
#define     GLFW_KEY_H              72
#define     GLFW_KEY_I              73
#define     GLFW_KEY_J              74
#define     GLFW_KEY_K              75
#define     GLFW_KEY_L              76
#define     GLFW_KEY_M              77
#define     GLFW_KEY_N              78
#define     GLFW_KEY_O              79
#define     GLFW_KEY_P              80
#define     GLFW_KEY_Q              81
#define     GLFW_KEY_R              82
#define     GLFW_KEY_S              83
#define     GLFW_KEY_T              84
#define     GLFW_KEY_U              85
#define     GLFW_KEY_V              86
#define     GLFW_KEY_W              87
#define     GLFW_KEY_X              88
#define     GLFW_KEY_Y              89
#define     GLFW_KEY_Z              90
#define     GLFW_KEY_LEFT_BRACKET   91 /* [ */
#define     GLFW_KEY_BACKSLASH      92 /* \ */
#define     GLFW_KEY_RIGHT_BRACKET  93 /* ] */
#define     GLFW_KEY_GRAVE_ACCENT   96 /* ` */
#define     GLFW_KEY_WORLD_1        161 /* non-US #1 */
#define     GLFW_KEY_WORLD_2        162 /* non-US #2 */
#define     GLFW_KEY_ESCAPE         256
#define     GLFW_KEY_ENTER          257
#define     GLFW_KEY_TAB            258
#define     GLFW_KEY_BACKSPACE      259
#define     GLFW_KEY_INSERT         260
#define     GLFW_KEY_DELETE         261
#define     GLFW_KEY_RIGHT          262
#define     GLFW_KEY_LEFT           263
#define     GLFW_KEY_DOWN           264
#define     GLFW_KEY_UP             265
#define     GLFW_KEY_PAGE_UP        266
#define     GLFW_KEY_PAGE_DOWN      267
#define     GLFW_KEY_HOME           268
#define     GLFW_KEY_END            269
#define     GLFW_KEY_CAPS_LOCK      280
#define     GLFW_KEY_SCROLL_LOCK    281
#define     GLFW_KEY_NUM_LOCK       282
#define     GLFW_KEY_PRINT_SCREEN   283
#define     GLFW_KEY_PAUSE          284
#define     GLFW_KEY_F1             290
#define     GLFW_KEY_F2             291
#define     GLFW_KEY_F3             292
#define     GLFW_KEY_F4             293
#define     GLFW_KEY_F5             294
#define     GLFW_KEY_F6             295
#define     GLFW_KEY_F7             296
#define     GLFW_KEY_F8             297
#define     GLFW_KEY_F9             298
#define     GLFW_KEY_F10            299
#define     GLFW_KEY_F11            300
#define     GLFW_KEY_F12            301
#define     GLFW_KEY_F13            302
#define     GLFW_KEY_F14            303
#define     GLFW_KEY_F15            304
#define     GLFW_KEY_F16            305
#define     GLFW_KEY_F17            306
#define     GLFW_KEY_F18            307
#define     GLFW_KEY_F19            308
#define     GLFW_KEY_F20            309
#define     GLFW_KEY_F21            310
#define     GLFW_KEY_F22            311
#define     GLFW_KEY_F23            312
#define     GLFW_KEY_F24            313
#define     GLFW_KEY_F25            314
#define     GLFW_KEY_KP_0           320
#define     GLFW_KEY_KP_1           321
#define     GLFW_KEY_KP_2           322
#define     GLFW_KEY_KP_3           323
#define     GLFW_KEY_KP_4           324
#define     GLFW_KEY_KP_5           325
#define     GLFW_KEY_KP_6           326
#define     GLFW_KEY_KP_7           327
#define     GLFW_KEY_KP_8           328
#define     GLFW_KEY_KP_9           329
#define     GLFW_KEY_KP_DECIMAL     330
#define     GLFW_KEY_KP_DIVIDE      331
#define     GLFW_KEY_KP_MULTIPLY    332
#define     GLFW_KEY_KP_SUBTRACT    333
#define     GLFW_KEY_KP_ADD         334
#define     GLFW_KEY_KP_ENTER       335
#define     GLFW_KEY_KP_EQUAL       336
#define     GLFW_KEY_LEFT_SHIFT     340
#define     GLFW_KEY_LEFT_CONTROL   341
#define     GLFW_KEY_LEFT_ALT       342
#define     GLFW_KEY_LEFT_SUPER     343
#define     GLFW_KEY_RIGHT_SHIFT    344
#define     GLFW_KEY_RIGHT_CONTROL  345
#define     GLFW_KEY_RIGHT_ALT      346
#define     GLFW_KEY_RIGHT_SUPER    347
#define     GLFW_KEY_MENU           348
#define     GLFW_KEY_LAST   GLFW_KEY_MENU

void NeuralCanvas::eraseLinesIf(std::function<bool(NeuralLines&)> condition) {
    MyObj_lines.erase(std::remove_if(MyObj_lines.begin(), MyObj_lines.end(), condition), MyObj_lines.end());
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if((glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS) &&
       glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS){
        switch(key){
            // File
            case GLFW_KEY_N:
                std::cout << "N" << std::endl;
                break;
            case GLFW_KEY_O:
                std::cout << "O" << std::endl;
                break;
            case GLFW_KEY_W:
                std::cout << "W" << std::endl;
                break;
            case GLFW_KEY_S:
                std::cout << "S" << std::endl;
                break;
            case GLFW_KEY_M:
                std::cout << "M" << std::endl;
                break;
            case GLFW_KEY_P:
                std::cout << "P" << std::endl;
                break;
            
            // Edit
            case GLFW_KEY_Z:
                std::cout << "z" << std::endl;
                break;
            case GLFW_KEY_X:
                std::cout << "X" << std::endl;
                break;
            case GLFW_KEY_C:
                std::cout << "C" << std::endl;
                break;
            case GLFW_KEY_V:
                std::cout << "V" << std::endl;
                break;
            case GLFW_KEY_D:
                std::cout << "D" << std::endl;
                break;
            case GLFW_KEY_A:
                std::cout << "A" << std::endl;
                break;
            case 93:
                //std::cout << "+" << std::endl;
                zoomlevel += 0.25;
                break;
            case 47:
                //std::cout << "-" << std::endl;
                zoomlevel -= 0.25;
                break;
            case GLFW_KEY_K:
                std::cout << "K" << std::endl;
                break;
            case GLFW_KEY_T:
                std::cout << "T" << std::endl;
                break;
            case GLFW_KEY_E:
                std::cout << "E" << std::endl;
                break;
                
            // Put
            case GLFW_KEY_1:
                std::cout << "1" << std::endl;
                break;
            case GLFW_KEY_2:
                std::cout << "2" << std::endl;
                break;
            case GLFW_KEY_3:
                std::cout << "3" << std::endl;
                break;
            case GLFW_KEY_4:
                std::cout << "4" << std::endl;
                break;
            case GLFW_KEY_5:
                std::cout << "5" << std::endl;
                break;
        }
    }
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS &&
       glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_S:
                std::cout << "Save As..." << std::endl;
                break;
            case GLFW_KEY_Z:
                std::cout << "Redo" << std::endl;
                break;
            case GLFW_KEY_R:
                std::cout << "tidy Up" << std::endl;
                break;
            case GLFW_KEY_L:
                std::cout << "Clear Console" << std::endl;
                break;
            // Put
            case GLFW_KEY_B:
                std::cout << "Bang" << std::endl;
                break;
            case GLFW_KEY_T:
                std::cout << "Toggle" << std::endl;
                break;
            case GLFW_KEY_N:
                std::cout << "Number2" << std::endl;
                break;
            case GLFW_KEY_V:
                std::cout << "VSlider" << std::endl;
                break;
            case GLFW_KEY_J:
                std::cout << "HSlider" << std::endl;
                break;
            case GLFW_KEY_D:
                std::cout << "Vradio" << std::endl;
                break;
            case GLFW_KEY_I:
                std::cout << "Hradio" << std::endl;
                break;
            case GLFW_KEY_U:
                std::cout << "VU Meter" << std::endl;
                break;
            case GLFW_KEY_C:
                std::cout << "Canvas" << std::endl;
                break;
            case GLFW_KEY_E:
                std::cout << "Equilizer" << std::endl;
                break;
            case GLFW_KEY_G:
                std::cout << "Graph" << std::endl;
                break;
            case GLFW_KEY_A:
                std::cout << "Array" << std::endl;
                break;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS ||
    glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS){
        // delete connections
        auto linesCondition = [&](NeuralLines& line) {
            if (line.startobj == selectindex || line.endobj == selectindex) {
                connectnumber--;
            return true;
            }
            if (line.startobj >= selectindex){
                line.startobj--;
            }
            if (line.endobj >= selectindex){
                line.endobj--;
            }
        return false;
        };
        NC.eraseLinesIf(linesCondition);
        NC.MyObj_rect.erase(NC.MyObj_rect.begin()+selectindex);
        NC.MyObj_font.erase(NC.MyObj_font.begin()+selectindex);
        objnumber--;
    }
}

void assignbinding(std::vector<std::string> bind){
    std::map<std::string, int> mp;
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
    //char const *p = "keybinding_macos.txt";
    //bind = readfile(p);
    //assignbinding(bind);
    #elif defined __linux__
    printf("linux");
    #endif
}
