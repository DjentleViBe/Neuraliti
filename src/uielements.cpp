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
//#include "../dependencies/include/imfilebrowser.h"
#include "../dependencies/include/windowing.h"
#include "../dependencies/include/picojson.h"
#include "../dependencies/include/glm/glm.hpp"
#include "../dependencies/include/glm/gtc/matrix_transform.hpp"
#include "../dependencies/include/glm/gtc/type_ptr.hpp"
#include "../dependencies/include/loadfont.hpp"
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/createobjs.hpp"
#include "../dependencies/include/fileoperations.hpp"
#include "../dependencies/include/initobjs.hpp"
#include <mach-o/dyld.h>

#define SCR_WIDTH 1280.0f
#define SCR_HEIGHT 960.0f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::map<std::string, std::string> appsettings;
std::vector<std::string> fontlist, configlist, fontsizelist;
const char* homeDir;
picojson::value v;
glm::mat4 mvp;
std::string CurrentDir;
int globalfontsize = 0;
double Xpos, Ypos ,tempmouseX, tempmouseY = 0.0;
double zoomlevel = 3.0;
bool show_demo_window = true;
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
//ImVec4 clear_color          = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
int window_width 			= 1280;
int window_height 			= 960;
float primary_color_1[]     = {0.8196, 0.8352, 0.8313}; // grey
float primary_color_2[]     = {0.12941, 0.1568, 0.1960}; // blue
float primary_color_3[]     = {0.9490, 0.9490, 0.9372}; // white
float primary_color_4[]     = {0.9411, 0.4705, 0.2235}; // orange
float primary_color_5[]     = {0.2313, 0.6862, 0.6862}; // green
float primary_color_6[]     = {0.9529, 0.6666, 0.2549}; // yellow
float primary_color_7[]     = {0.2784, 0.4705, 0.4901}; // dark green
std::vector<int> globalinlets;
std::vector<int> globaloutlets;

GLFWwindow* window;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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
    //std::string name = v.get("Media").get("Preferences").to_str();
    addlogs("Initialisation started\n");
    appsettings["defaultfolder"] = std::string(homeDir) + v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("defaultfolder").to_str();
    //double age = v.get("id").get<double>();
    addlogs("Default folder : " + appsettings["defaultfolder"] + "\n");
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

int INITgraphics(){
    
    homeDir = std::getenv("HOME");
    std::cout << std::string(homeDir);
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0){
        printf("executable path is %s\n", path);
    }
    else{
        printf("buffer too small; need size %u\n", size);
        return 1;
    }
    // Initial startup
    addlogs("Opening preferences\n");
    CurrentDir = std::string(path).erase(std::string(path).size() - 9);
    std::cout << CurrentDir;
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

    window = glfwCreateWindow(window_width, window_height, "GENT",  NULL, NULL);
    //glfwSetWindowAspectRatio(window,1189,1000);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Enable vsync
    // Setup Platform/Renderer backends
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
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
    //readkeybindings();
    
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    addlogs("Initialisation ended\n");
    addlogs("Opening file\n");
    initobjs(CurrentDir + "/Untitled-1.pd");
    printvector(globalinlets, "Inlets");
    printvector(globaloutlets, "Outlets");
    return 0;
}

GLuint calculate_view(float wid, float hei, glm::vec3 point, double transX, double transY){
    
    glm::mat4 projection = glm::perspective(glm::radians(35.0f), 1.0f, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.5f, 1.5f);
    // Camera matrix
    Xpos -= transX * 0.01;
    Ypos += transY * 0.01;
    glm::mat4 View = glm::lookAt(
        glm::vec3(0, 0, zoomlevel), // Camera is at (0,0,0), in World Space
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
    mvp =  projection * View * translateBack * scaleMatrix * translateToOrigin;
    //GLuint MatrixID = glGetUniformLocation(mainShader.ID, "ProjMat");
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    //Shader objShader((CurrentDir + "/bin/objshader.vs").c_str(), (CurrentDir + "/bin/objshader.fs").c_str());
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    double deltaX = xpos - tempmouseX;
    double deltaY = ypos - tempmouseY;
    if (state == GLFW_PRESS){
        calculate_view(window_width, window_height, glm::vec3(-0.45, 0.1f, 0.0f), deltaX * 0.1, deltaY * 0.1);
        }
    tempmouseX = xpos;
    tempmouseY = ypos;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        std::cout << "pressed" << "\n";
    }
}

void Displayloop(){
    //int display_w, display_h;
    ImGuiIO io = ImGui::GetIO();
    ImGui::FileBrowser fileDialog;
    
    Shader fontShader((CurrentDir + "/bin/fontshader.vs").c_str(), (CurrentDir + "/bin/fontshader.fs").c_str());
    Shader objShader((CurrentDir + "/bin/objshader.vs").c_str(), (CurrentDir + "/bin/objshader.fs").c_str());
    Shader nodeShader((CurrentDir + "/bin/inletshader.vs").c_str(), (CurrentDir + "/bin/inletshader.fs").c_str());
    Shader lineShader((CurrentDir + "/bin/lineshader.vs").c_str(), (CurrentDir + "/bin/lineshader.fs").c_str());
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    calculate_view(window_width, window_height, glm::vec3(-0.45, 0.1f, 0.0f), Xpos, Ypos);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetMouseButtonCallback(window, mouse_button_callback);

    //nodeShader.use();
    // loop through objects here
    NeuralObj *MyObj_rect = new NeuralObj[objnumber];
    for (int i = 0; i < objnumber; ++i) {
        MyObj_rect[i] = createobj1(i, Xposition[i], Yposition[i], objectnames[i], 0);
        MyObj_rect[i].Inlets = new int*[MyObj_rect[i].Inletnum];
        MyObj_rect[i].Outlets = new int*[MyObj_rect[i].Outletnum];
    }
    NeuralObj *MyObj_font = new NeuralObj[objnumber];
    for (int i = 0; i < objnumber; ++i) {
        MyObj_font[i] = createobj1(i, Xposition[i], Yposition[i], objectnames[i], 1);
    }

    // NeuralLines* MyObj_lines = setupconnections(MyObj_rect, CurrentDir + "/Untitled-1.pd");
    // inlet outlet mapping
    MyObj_rect[1].Inlets[0] = new int[4];
    MyObj_rect[0].Outlets[0] = new int[4];
    MyObj_rect[1].Inlets[0] = MyObj_rect[0].Outlets[0];
    MyObj_rect[0].Outlets[0][0] = 10;
    MyObj_rect[0].Outlets[0][1] = 20;
    //std::cout << MyObj_rect[1].Inlets[0][1] * 20 << "\n";

    while (!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &window_width, &window_height);
        glfwSetKeyCallback(window, key_callback);
        glClearColor(primary_color_1[0], primary_color_1[1], primary_color_1[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        calculate_view(window_width, window_height, glm::vec3(-0.45, 0.1f, 0.0f), 0.0, 0.0);
        /*
        lineShader.use();
        for (int i = 0; i < connectnumber; i++){
            MyObj_lines[i].Matrix = glGetUniformLocation(lineShader.ID, "ProjMat");
            glUniformMatrix4fv(MyObj_lines[i].Matrix, 1, GL_FALSE, &mvp[0][0]);
            glBindVertexArray(MyObj_lines[i].VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        */
        
        for (int i = 0; i < objnumber; ++i) {
            objShader.use();
            MyObj_rect[i].Matrix = glGetUniformLocation(objShader.ID, "ProjMat");
            glUniformMatrix4fv(MyObj_rect[i].Matrix, 1, GL_FALSE, &mvp[0][0]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, MyObj_rect[i].texture);
            glBindVertexArray(MyObj_rect[i].VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            /*
            nodeShader.use();
            MyObj_rect[i].Matrix = glGetUniformLocation(nodeShader.ID, "ProjMat");
            glUniformMatrix4fv(MyObj_rect[i].Matrix, 1, GL_FALSE, &mvp[0][0]);
            glBindVertexArray(MyObj_rect[i].inquadVAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MyObj_rect[i].Inletnum); 

            glUniformMatrix4fv(MyObj_rect[i].Matrix, 1, GL_FALSE, &mvp[0][0]);
            glBindVertexArray(MyObj_rect[i].outquadVAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MyObj_rect[i].Outletnum); 
            */
        }
        /*
        fontShader.use();
        for (int i = 0; i < objnumber; ++i) {
            MyObj_font[i].Matrix = glGetUniformLocation(fontShader.ID, "ProjMat");
            glUniformMatrix4fv(MyObj_font[i].Matrix, 1, GL_FALSE, &mvp[0][0]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, MyObj_font[i].texture);
            glBindVertexArray(MyObj_font[i].VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        */

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ShowMenu(&show_demo_window);
        ImGui::SetNextWindowSize(ImVec2(window_width / 4.0, window_height * 5.0 / 6.0));
        ImGui::SetNextWindowPos(ImVec2(0, 20));

        ImGui::Begin("Window A");
        ImGui::Text("NEURALITI");
        // open file dialog when user clicks this button
        
        if(fileDialog.HasSelected()){
            std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
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
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete[] MyObj_font;
    delete[] MyObj_rect;
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
