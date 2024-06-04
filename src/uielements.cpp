#include <stdio.h>
#include <iostream>
#include <ctime>
#include <map>
#include <string>
#include "../dependencies/include/glad/glad.h"
#define GL_SILENCE_DEPRECATION
#include "../dependencies/include/GLFW/glfw3.h" // Will drag system OpenGL headers
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/stb_image.h"
#include "../dependencies/include/shader_s.h"
#include "../dependencies/include/creategeom.h"
#include "../dependencies/include/imgui.h"
#include "../dependencies/include/imgui_impl_glfw.h"
#include "../dependencies/include/imgui_impl_opengl3.h"
#include "../dependencies/include/menu.h"
#include "../dependencies/include/keybindings.h"
#include "../dependencies/include/extras.h"
#include "../dependencies/include/imfilebrowser.h"
#include "../dependencies/include/windowing.h"
#include "../dependencies/include/picojson.h"
#include "../dependencies/include/glm/glm.hpp"
#include "../dependencies/include/glm/gtc/matrix_transform.hpp"
#include "../dependencies/include/glm/gtc/type_ptr.hpp"
#include "../dependencies/include/loadfont.hpp"
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/createobjs.hpp"
#include "../dependencies/include/fileoperations.hpp"
#include "../dependencies/include/mathoperations.h"
#include "../dependencies/include/initobjs.hpp"
#include <mach-o/dyld.h>

#define SCR_WIDTH 1280.0f
#define SCR_HEIGHT 960.0f
std::string filename = "";
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::map<std::string, std::string> appsettings;
std::vector<std::string> fontlist, configlist, fontsizelist;
const char *homeDir;
picojson::value v;
glm::mat4 *mvp;
glm::mat4 *mvp_lines;
NeuralCanvas NC;
std::string CurrentDir;
int selectindex;
bool selected;
glm::vec2 mouseloc;
std::vector<unsigned char> pixels(window_width * window_height); // Assuming RGB color format
int globalfontsize = 0;
double Xpos, Ypos ,tempmouseX, tempmouseY, transmouseX, transmouseY = 0.0;
float zoomlevel = 1.15f;
bool show_demo_window = true;
GLFWcursor *cursor_normal; 
GLFWcursor *cursor_hand;
GLFWcursor *custom_cursor;
int cursor_type = 0; // 0: default, 1: hand
char buffer[256] = "";
bool showobjprop = false;
int inletnum = 0;
int outletnum = 0;

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
int window_width 			= 1280;
int window_height 			= 960;
float primary_color_1[]     = {0.8196, 0.8352, 0.8313}; // grey
float primary_color_2[]     = {0.1294, 0.1568, 0.1960}; // blue
float primary_color_3[]     = {0.9490, 0.9490, 0.9372}; // white
float primary_color_4[]     = {0.9411, 0.4705, 0.2235}; // orange
float primary_color_5[]     = {0.2313, 0.6862, 0.6862}; // green
float primary_color_6[]     = {0.9529, 0.6666, 0.2549}; // yellow
float primary_color_7[]     = {0.2784, 0.4705, 0.4901}; // dark green
float primary_color_8[]     = {0.1647, 0.2862, 0.4784}; // IMGUI blue
float primary_color_9[]     = {0.0000, 0.0000, 1.0000}; // Navy blue

float primary_color_10[]    = {0.3098, 0.3529, 0.2980}; // float
float primary_color_11[]    = {0.7098, 0.6470, 0.5568};
float primary_color_12[]    = {0.9058, 0.8392, 0.7607};
float primary_color_13[]    = {0.6588, 0.6431, 0.5450};

std::vector<int> globalinlets;
std::vector<int> globaloutlets;

GLFWwindow *window;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool MyInputTextCallback(ImGuiInputTextCallbackData* data) {
    // Your callback logic here (optional)
    return 0;
}

// GLFW char callback function
void CharCallback(GLFWwindow* window, unsigned int c) {
    ImGui_ImplGlfw_CharCallback(window, c);
}

int loadconfig(const std::string& path){
    // read default preferences //
    std::string defaultprefs;
    defaultprefs = readfileconcat(path.c_str());
    // Parse the JSON string
    
    std::string err = picojson::parse(v, defaultprefs);
    
    // Check for parsing errors
    if (!err.empty()) {
        std::cerr << "Error parsing JSON: " << err << std::endl;
        return 1;
    }
    // Access JSON values
    addlogs("Initialisation started\n");
    appsettings["defaultfolder"] = std::string(homeDir) + v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("defaultfolder").to_str();
    //addlogs("Default folder : " + appsettings["defaultfolder"] + "\n");
    appsettings["defaultfont"] =  v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("font").to_str();
    addlogs("Default font : " + appsettings["defaultfont"] + "\n");
    appsettings["fontsize"] =  v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("fontsize").to_str();
    globalfontsize = stoi(appsettings["fontsize"]);
    fontlist = listfiles(appsettings["defaultfolder"] + "/fonts", ".ttf");
    configlist = listfiles(appsettings["defaultfolder"], ".json");
    fontsizelist = {"12", "13", "14", "15", "16", "17", "18"};
    return 0;
}

void loadfont(ImGuiIO& io){
    ImFontConfig config;
    static const ImWchar myGlyphRanges[] = {
        0x21E7, 0x21E7, // shift
        0x2318, 0x2318, // cmd
        0
    };
    addlogs("Font loading\n");
    io.Fonts->AddFontFromFileTTF((CurrentDir + "/assets/fonts/" + appsettings["defaultfont"]).c_str() , std::stof(appsettings["fontsize"]));
    static ImFontConfig cfg;
    cfg.OversampleH = cfg.OversampleV = 2;
    cfg.MergeMode = true;
    #if defined __APPLE__
    io.Fonts->AddFontFromFileTTF((CurrentDir + "/assets/fonts/" +appsettings["defaultfont"]).c_str() , 17, &cfg,
                                 myGlyphRanges);
    #endif
    io.Fonts->Build();
    addlogs("Font loaded\n");
}

void loadobjects(){
    for (int i = 0; i < objnumber; ++i) {
        NC.MyObj_rect.push_back(createobj1(i, Xposition[i], Yposition[i], objectnames[i], 0));
        NC.MyObj_rect[i].Inlets = new float*[NC.MyObj_rect[i].Inletnum];
        NC.MyObj_rect[i].Outlets = new float*[NC.MyObj_rect[i].Outletnum];
        for (int j = 0; j < NC.MyObj_rect[i].Inletnum; j++){
            NC.MyObj_rect[i].Inlets[j] = new float[1];
        }
        for (int j = 0; j < NC.MyObj_rect[i].Outletnum; j++){
            NC.MyObj_rect[i].Outlets[j] = new float[1];
        }   
    }
    
    for (int i = 0; i < objnumber; ++i) {
        NC.MyObj_font.push_back(createobj1(i, Xposition[i], Yposition[i], objectnames[i], 1));
    }
    
    NC.MyObj_lines = setupconnections(NC.MyObj_rect, CurrentDir + "/" + filename);
    
    /*
    // inlet outlet mapping
    NC.MyObj_rect[1].Inlets[0] = new int[4];
    NC.MyObj_rect[0].Outlets[0] = new int[4];
    NC.MyObj_rect[1].Inlets[0] = NC.MyObj_rect[0].Outlets[0];
    NC.MyObj_rect[0].Outlets[0][0] = 10;
    NC.MyObj_rect[0].Outlets[0][1] = 20;*/
}

int INITgraphics(){
    homeDir = std::getenv("HOME");
    // std::cout << std::string(homeDir);
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0){
        // printf("\nexecutable path is %s\n", path);
    }
    else{
        // printf("\nbuffer too small; need size %u\n", size);
        return 1;
    }
    // Initial startup
    addlogs("Opening preferences\n");
    CurrentDir = std::string(path).erase(std::string(path).size() - 14);
    // std::cout << CurrentDir;
    loadconfig(CurrentDir + "/prefs.json");
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    #endif

    window = glfwCreateWindow(window_width, window_height, "NEURALITI",  NULL, NULL);
    //glfwSetWindowAspectRatio(window,1189,1000);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Enable vsync
    // Setup Platform/Renderer backends
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    #ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
    #endif
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    loadfont(io);
    
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    addlogs("Initialisation ended\n");
    addlogs("Opening file\n");
    // open history file if it exists otherwise make empty .pd file
    std::vector<std::string> filenametemp = readfile((CurrentDir + "/.history").c_str());
    filename = filenametemp[filenametemp.size() - 1];

    if(filename.length() != 0){
        addlogs("Opening file : " + filename);
        initobjs(CurrentDir + "/" + filename);
    }
    else{
        addlogs("no history found");
    }

    return 0;
}

GLuint calculate_view(float wid, float hei, glm::vec3 point, double transX, double transY){
    
    //glm::mat4 projection = glm::perspective(glm::radians(35.0f), 1.0f, 0.1f, 100.0f);
    float aspect = (float)window_width/(float)window_height;
    glm::mat4 projection = glm::ortho(-aspect/zoomlevel, aspect/zoomlevel, -1.0f/zoomlevel, 1.0f/zoomlevel, 0.1f, 100.0f);
    // Camera matrix
    Xpos -= transX * 0.01;
    Ypos += transY * 0.01;
    glm::mat4 View = glm::lookAt(
        glm::vec3(Xpos, Ypos, zoomlevel), // Camera is at (0,0,0), in World Space
        glm::vec3(Xpos, Ypos, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    // Step 1: Translate the point to the origin
    glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -point);

    // Step 2: Perform scaling operation
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(SCR_WIDTH / wid, SCR_HEIGHT / hei, 1));

    // Step 3: Translate the point back to its original position
    glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), point);
    // Our ModelViewProjection: multiplication of our 3 matrices
    for(int m = 0; m < objnumber; m++){
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(NC.MyObj_rect[m].offsetx, NC.MyObj_rect[m].offsety, 0.0));
        mvp[m] = projection * View * translateBack * scaleMatrix * translateToOrigin * translate;
        NC.MyObj_rect[m].result = mvp[m] * glm::vec4(NC.MyObj_rect[m].x, NC.MyObj_rect[m].y, 0.0, 1.0);
    }

    for(int m = 0; m < objnumber; m++){
        //glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(NC.MyObj_rect[m].offsetx, NC.MyObj_rect[m].offsety, 0.0));
        mvp_lines[m] = projection * View * translateBack * scaleMatrix * translateToOrigin;
    }
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //std::cout << window_width << "\n";
    //glViewport(0, 0, height, height);
    //Shader objShader("../assets/objshader.vs", "../assets/objshader.fs");
    //Shader fontShader("../assets/objshader.vs", "../assets/objshader.fs");
    //scale window
    
    //addlogs("scaled");
}

double deltaX;
double deltaY;

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    
    float boundingx = (xpos * 2 / window_width) - 1;
    float boundingy = -(ypos * 2 / window_height) + 1;
    cursor_type = 0;
    if(objnumber!=0){
        for(int o = 0; o < objnumber; o++){
            if(NC.MyObj_rect[o].result.x < boundingx && NC.MyObj_rect[o].result.x + NC.MyObj_rect[o].sentencewidth > boundingx){
                if(NC.MyObj_rect[o].result.y > boundingy && NC.MyObj_rect[o].result.y - NC.MyObj_rect[o].sentenceheight < boundingy){
                    cursor_type = 1;
                    }
                }
            }
    }
    
    if (state == GLFW_PRESS){
        if(selected){
            transmouseX = mouseloc.x;
            transmouseY = mouseloc.y;
            deltaX = boundingx - transmouseX;
            deltaY = boundingy - transmouseY;
            NC.MyObj_rect[selectindex].offsetx = NC.MyObj_rect[selectindex].transX + deltaX;
            NC.MyObj_rect[selectindex].offsety = NC.MyObj_rect[selectindex].transY + deltaY;
        }
        else{
            deltaX = xpos - tempmouseX;
            deltaY = ypos - tempmouseY;
            calculate_view(window_width, window_height, glm::vec3(-0.45f, 0.0f, 0.0f), deltaX * 0.1, deltaY * 0.1);
        }
    }
    
    if(state == GLFW_RELEASE){
        if(objnumber != 0){
            NC.MyObj_rect[selectindex].transX = NC.MyObj_rect[selectindex].offsetx;
            NC.MyObj_rect[selectindex].transY = NC.MyObj_rect[selectindex].offsety;
        }
    }
    
    tempmouseX = xpos;
    tempmouseY = ypos;
    transmouseX = boundingx;
    transmouseY = boundingy;
    
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float boundingx = (xpos * 2 / window_width) - 1;
        float boundingy = -(ypos * 2 / window_height) + 1;
        for(int o = 0; o < objnumber; o++){
            if(NC.MyObj_rect[o].result.x < boundingx && NC.MyObj_rect[o].result.x + NC.MyObj_rect[o].sentencewidth > boundingx){
                if(NC.MyObj_rect[o].result.y > boundingy && NC.MyObj_rect[o].result.y - NC.MyObj_rect[o].sentenceheight < boundingy){
                    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
                        // addlogs("Pressed :" + NC.MyObj_rect[o].objdisplayname + "\n");
                        NC.MyObj_rect[o].select = 1;
                        selectindex = o;
                        selected = true;
                        mouseloc.x = boundingx;
                        mouseloc.y = boundingy;
                        clearproperties();
                        showobjprop = true;
                        std::strncpy(buffer, NC.MyObj_rect[o].objdisplayname.c_str(), sizeof(buffer));
                        addproperties("Inlets       : " + intToString(NC.MyObj_rect[o].Inletnum) + "\n");
                        addproperties("Outlets      : " + intToString(NC.MyObj_rect[o].Outletnum) + "\n");
                        switch(NC.MyObj_rect[o].objtype){
                            case 0 :
                                addproperties("Objtect type : Object\n");
                                break;
                            case 1 :
                                addproperties("Objtect type : Object\n");
                                break;
                            case 2 :
                                addproperties("Objtect type : Float\n");
                                break;
                            default :
                                addproperties("Objtect type : Unknown\n");
                                break;
                        }
                        break;
                    }
                    else{
                        selected = false;
                    }
                }
            }
        }
        float xscale, yscale;
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        glfwGetMonitorContentScale(primary, &xscale, &yscale);
        unsigned char pick_col[0];
        glReadPixels(xpos*xscale, (window_height - ypos)*yscale, 1, 1, GL_RED, GL_UNSIGNED_BYTE, pick_col);
        if(abs(static_cast<int>(pick_col[0]) / 255.0 - primary_color_1[0]) < 0.001){
            // std::cout << "reset\n";
            for(int o = 0; o < objnumber; o++){
                NC.MyObj_rect[o].select = 0;
                showobjprop = false;
                clearproperties();
            }
        }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        //NC.MyObj_rect[selectindex].x = NC.MyObj_rect[selectindex].offsetx;
        //NC.MyObj_rect[selectindex].y = NC.MyObj_rect[selectindex].offsety;
        //NC.MyObj_rect[selectindex].offsetx = 0.0;
        //NC.MyObj_rect[selectindex].offsety = 0.0;
    }
}

void Displayloop(){
    ImGuiIO io = ImGui::GetIO();
    ImGui::FileBrowser fileDialog;
    ImGui::FileBrowser openDialog;
    
    Shader fontShader((CurrentDir + "/bin/fontshader.vs").c_str(), (CurrentDir + "/bin/fontshader.fs").c_str());
    Shader objShader((CurrentDir + "/bin/objshader.vs").c_str(), (CurrentDir + "/bin/objshader.fs").c_str());
    Shader nodeShader((CurrentDir + "/bin/inletshader.vs").c_str(), (CurrentDir + "/bin/inletshader.fs").c_str());
    Shader lineShader((CurrentDir + "/bin/lineshader.vs").c_str(), (CurrentDir + "/bin/lineshader.fs").c_str());
    cursor_normal = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursor_hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // loop through objects here
    mvp = new glm::mat4[objnumber];
    mvp_lines = new glm::mat4[objnumber];
    loadobjects();
    calculate_view(window_width, window_height, glm::vec3(-0.45, 0.0f, 0.0f), Xpos, Ypos);
    unsigned int objectColorLoc = glGetUniformLocation(objShader.ID, "aColor");
    unsigned int fontColorLoc = glGetUniformLocation(fontShader.ID, "aColor");

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glm::vec3 startPos;
    glm::vec2 size;
    glm::mediump_vec3 endPos;
    float angle;

    while (!glfwWindowShouldClose(window))
    {   
        glfwGetWindowSize(window, &window_width, &window_height);
        glfwSetKeyCallback(window, key_callback);
        glClearColor(primary_color_1[0], primary_color_1[1], primary_color_1[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        calculate_view(window_width, window_height, glm::vec3(-0.45, 0.0f, 0.0f), 0.0, 0.0);
        
        for (int i = 0; i < connectnumber; i++){
            lineShader.use();
            NC.MyObj_lines[i].Matrix = glGetUniformLocation(lineShader.ID, "ProjMat");
            startPos = glm::vec3(NC.MyObj_lines[i].startx + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsetx,
                                 NC.MyObj_lines[i].starty + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsety, 0.0f);
            endPos = glm::vec3(NC.MyObj_lines[i].startx + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsetx + 
                                calculate_distance(NC.MyObj_lines[i].startx + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsetx,
                                NC.MyObj_lines[i].starty + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsety,
                                NC.MyObj_lines[i].endx + NC.MyObj_rect[NC.MyObj_lines[i].endobj].offsetx, 
                                NC.MyObj_lines[i].endy + NC.MyObj_rect[NC.MyObj_lines[i].endobj].offsety), 
                                NC.MyObj_lines[i].starty + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsety + 0.003, 0.0f);
            angle = M_PI + calculate_angle(NC.MyObj_lines[i].endx + NC.MyObj_rect[NC.MyObj_lines[i].endobj].offsetx, 
                                NC.MyObj_lines[i].endy + NC.MyObj_rect[NC.MyObj_lines[i].endobj].offsety,
                                NC.MyObj_lines[i].startx + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsetx,
                                NC.MyObj_lines[i].starty + NC.MyObj_rect[NC.MyObj_lines[i].startobj].offsety);
            glUniform3fv(glGetUniformLocation(lineShader.ID, "startPos"), 1, &startPos[0]);
            glUniform3fv(glGetUniformLocation(lineShader.ID, "endPos"), 1, &endPos[0]);
            glUniform1fv(glGetUniformLocation(lineShader.ID, "angle"), 1, &angle);
            glUniformMatrix4fv(NC.MyObj_lines[i].Matrix, 1, GL_FALSE, &mvp_lines[i][0][0]);
            glBindVertexArray(NC.MyObj_lines[i].VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        
        for (int i = 0; i < objnumber; ++i) {
            objShader.use();
            NC.MyObj_rect[i].Matrix = glGetUniformLocation(objShader.ID, "ProjMat");
            startPos = glm::vec3(NC.MyObj_rect[i].x, NC.MyObj_rect[i].y, 0.0f);
            size = glm::vec2(NC.MyObj_rect[i].sentencewidth, -NC.MyObj_rect[i].sentenceheight);
            glUniform3fv(glGetUniformLocation(objShader.ID, "startPos"), 1, &startPos[0]);
            glUniform2fv(glGetUniformLocation(objShader.ID, "size"), 1, &size[0]);

            if(NC.MyObj_rect[i].select == 1){
                glUniform3fv(objectColorLoc, 1, primary_color_8);
            }
            else{
                glUniform3fv(objectColorLoc, 1, NC.MyObj_rect[i].color);
            }
            glUniformMatrix4fv(NC.MyObj_rect[i].Matrix, 1, GL_FALSE, &mvp[i][0][0]);
            glBindVertexArray(NC.MyObj_rect[i].VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            
            nodeShader.use();
            NC.MyObj_rect[i].Matrix = glGetUniformLocation(nodeShader.ID, "ProjMat");
            glUniformMatrix4fv(NC.MyObj_rect[i].Matrix, 1, GL_FALSE, &mvp[i][0][0]);
            glBindVertexArray(NC.MyObj_rect[i].inquadVAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NC.MyObj_rect[i].Inletnum);
            glUniformMatrix4fv(NC.MyObj_rect[i].Matrix, 1, GL_FALSE, &mvp[i][0][0]);
            glBindVertexArray(NC.MyObj_rect[i].outquadVAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NC.MyObj_rect[i].Outletnum);

            fontShader.use();
            NC.MyObj_font[i].objectwidth = NC.MyObj_font[i].objdisplayname.length() * (float)globalfontsize * 2.0;
            NC.MyObj_rect[i].objectwidth = NC.MyObj_font[i].objectwidth;
            NC.MyObj_rect[i].sentencewidth = (float)NC.MyObj_rect[i].objectwidth/(float)window_width;
            NC.MyObj_font[i].sentencewidth = (float)NC.MyObj_font[i].objectwidth/(float)window_width;
            NC.MyObj_font[i].Matrix = glGetUniformLocation(fontShader.ID, "ProjMat");
            startPos = glm::vec3(NC.MyObj_font[i].x, NC.MyObj_font[i].y, 0.0f);
            size = glm::vec2(NC.MyObj_font[i].sentencewidth, -NC.MyObj_font[i].sentenceheight);
            glUniform3fv(glGetUniformLocation(fontShader.ID, "startPos"), 1, &startPos[0]);
            glUniform2fv(glGetUniformLocation(fontShader.ID, "size"), 1, &size[0]);
            glUniform3fv(fontColorLoc, 1, NC.MyObj_font[i].color);
            glUniformMatrix4fv(NC.MyObj_font[i].Matrix, 1, GL_FALSE, &mvp[i][0][0]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, NC.MyObj_font[i].texture);
            glBindVertexArray(NC.MyObj_font[i].VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ShowMenu(&show_demo_window);
        ImGui::SetNextWindowSize(ImVec2(window_width / 4.0, window_height * 5.0 / 6.0 - 20));
        ImGui::SetNextWindowPos(ImVec2(0, 20));

        ImGui::Begin("Object Properties");
        if(showobjprop){
            ImGui::Text("Object name  :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(window_width * 0.4 / 4.0);
            ImGui::InputText("##Input", buffer, IM_ARRAYSIZE(buffer));
            ImGui::TextUnformatted(properties.c_str());
            if (ImGui::Button("Modify!")) {
                if(isFloat(buffer) == 0 && NC.MyObj_rect[selectindex].objtype == 2){
                    addlogs("\nObject is of float type, enter a number!\n");
                }
                else{
                    NC.MyObj_font[selectindex].objdisplayname = buffer;
                    NC.MyObj_rect[selectindex].objdisplayname = buffer;
                    NC.MyObj_font[selectindex].objectwidth = NC.MyObj_font[selectindex].objdisplayname.length() * (float)globalfontsize * 2.0;
                    NC.MyObj_rect[selectindex].objectwidth = NC.MyObj_font[selectindex].objectwidth;
                    NC.MyObj_rect[selectindex].sentencewidth = (float)NC.MyObj_rect[selectindex].objectwidth/(float)window_width;
                    NC.MyObj_font[selectindex].sentencewidth = (float)NC.MyObj_font[selectindex].objectwidth/(float)window_width;
                    modifyobject(selectindex);
                }
            }
        }

        for (int i = 0; i < objnumber; i++){
            sharedlibrary(i);
            // process the name of the methods
        }
        // open file dialog when user clicks this button
        if(fileDialog.HasSelected()){
            // std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
            fileDialog.ClearSelected();
        }
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(window_width - (window_width / 4.0), window_height - window_height * 5.0 / 6.0));
        ImGui::SetNextWindowPos(ImVec2(window_width / 4.0, (window_height * 5.0 / 6.0)));

        ImGui::Begin("Debug");
        time_t now = time(0);
        // convert now to string form
        const char *date_time = ctime(&now);
        ImGui::Text("%s", date_time);
        if(editpref){
            editprefwindow(fileDialog);
            ImGui::End();
            //ImGui::ShowDemoWindow(&editpref);
        }
        if(openfile){
            fileopenwindow(openDialog);
        }
        // std::cout << "looping" << std::endl;
        //ImGui::Text("This is the debug window");
        ImGui::TextUnformatted(logs.c_str());
        ImGui::SetScrollY(ImGui::GetScrollMaxY());
        
        ImGui::End();
        
        //
        ImGui::SetNextWindowSize(ImVec2((window_width / 4.0), window_height - window_height * 5.0 / 6.0));
        ImGui::SetNextWindowPos(ImVec2(0, (window_height * 5.0 / 6.0)));

        ImGui::Begin("Properties");
        ImGui::Combo("Verbose", &verbose,
                    "0\0"
                    "1\0");
        ImGui::End();
        
        
        // input
        // -----
        processInput(window);
        
        // second primitive
        
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if(cursor_type == 1){
            glfwSetCursor(window, cursor_hand);
        }
        else{
            glfwSetCursor(window, cursor_normal);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    clearobjs();
    // delete[] NC.MyObj_font;
    // delete[] NC.MyObj_rect;
    // delete[] NC.NC.MyObj_lines;
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    /*glDeleteVertexArrays(1, &MyObj1.VAO);
    glDeleteBuffers(1, &MyObj1.VBO);
    glDeleteBuffers(1, &MyObj1.EBO);

    glDeleteVertexArrays(1, &MyObj2.VAO);
    glDeleteBuffers(1, &MyObj2.VBO);
    glDeleteBuffers(1, &MyObj2.EBO);*/
    
    glfwDestroyWindow(window);
    glfwTerminate();

}
