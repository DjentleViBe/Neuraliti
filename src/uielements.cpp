#include <stdio.h>
#include <iostream>
#include <ctime>
#include <map>
#include <string>
#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h" // Will drag system OpenGL headers
#include "uielements.h"
#include "stb_image.h"
#include "shader_s.h"
#include "creategeom.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "menu.h"
#include "keybindings.h"
#include "extras.h"
#include "imfilebrowser.h"
#include "windowing.h"
#include "picojson.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "loadfont.hpp"
#include "datatypes.hpp"

#define SCR_WIDTH 1280.0f
#define SCR_HEIGHT 960.0f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::map<std::string, std::string> appsettings;
std::vector<std::string> fontlist, configlist, fontsizelist;
const char* homeDir = std::getenv("HOME");
picojson::value v;
GLuint MatrixID;
glm::mat4 mvp;
NeuralObj MyObj1, MyObj2;

bool show_demo_window;
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
ImVec4 clear_color          = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
int window_width 			= 1280;
int window_height 			= 960;
float primary_color_1[]     = {0.93725, 0.89019, 0.79215};
float primary_color_2[]     = {0.12941, 0.12941, 0.15294};
float primary_color_3[]     = {1.0, 1.0, 1.0};

GLFWwindow* window;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int loadconfig(std::string path){
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
    std::string name = v.get("Media").get("Preferences").to_str();
    addlogs("Initialisation started\n");
    appsettings["defaultfolder"] = std::string(homeDir) + v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("defaultfolder").to_str();
    //double age = v.get("id").get<double>();
    addlogs("Default folder : " + appsettings["defaultfolder"] + "\n");
    appsettings["defaultfont"] =  v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("font").to_str();
    addlogs("Default font : " + appsettings["defaultfont"] + "\n");
    appsettings["fontsize"] =  v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("fontsize").to_str();
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
    io.Fonts->AddFontFromFileTTF(("../assets/fonts/" + appsettings["defaultfont"]).c_str() , std::stof(appsettings["fontsize"]));
    static ImFontConfig cfg;
    cfg.OversampleH = cfg.OversampleV = 2;
    cfg.MergeMode = true;
    #if defined __APPLE__
    io.Fonts->AddFontFromFileTTF(("../assets/fonts/" +appsettings["defaultfont"]).c_str() , 17, &cfg,
                                 myGlyphRanges);
    #endif
    io.Fonts->Build();
    addlogs("Font loaded\n");
}

int INITgraphics(){
    std::string currentDir = GetCurrentWorkingDirectory();
    if (!currentDir.empty()) {
        std::cout << "Current working directory: " << currentDir << std::endl;
    }
    else {
        std::cerr << "Error: Unable to retrieve current working directory." << std::endl;
        }
    // Initial startup
    loadconfig("../prefs.json");
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
    readkeybindings();
    
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    addlogs("Initialisation ended\n");
    return 0;
}

GLuint calculate_view(Shader mainShader, float wid, float hei, glm::vec3 point){
    
    //glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float) wid / (float)hei, 0.1f, 10.0f);
    //glm::mat4 projection = glm::ortho(-1.0, 1.0, -1.0, 1.0);
    //projection = glm::scale(projection, glm::vec3(0.5, 0.5, 1.0));
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(0,0,3), // Camera is at (0,0,0), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    
    // Model matrix: an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Step 1: Translate the point to the origin
    glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -point);

    // Step 2: Perform scaling operation
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(SCR_WIDTH / wid, SCR_HEIGHT / hei, 1));

    // Step 3: Translate the point back to its original position
    glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), point);
    //glm::mat4 scaling = glm::scale(glm::mat4(1), glm::vec3(SCR_WIDTH / wid,1,1));
    // Our ModelViewProjection: multiplication of our 3 matrices
    mvp = translateBack * scaleMatrix * translateToOrigin;
    GLuint MatrixID = glGetUniformLocation(mainShader.ID, "ProjMat");
    return MatrixID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //std::cout << window_width << "\n";
    //glViewport(0, 0, height, height);
    Shader objShader("../assets/objshader.vs", "../assets/objshader.fs");
    //scale window
    addlogs("scaled");
    //std::cout << (float)window_width/(float)window_height << "\n";
    //calculate_view(ourShader, (float)window_width, (float)window_height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Displayloop(char **argv){
    //int display_w, display_h;
    bool show_demo_window = true;
    ImGuiIO io = ImGui::GetIO();
    ImGui::FileBrowser fileDialog;
    Shader objShader("../assets/objshader.vs", "../assets/objshader.fs");
    Shader fontShader("../assets/fontshader.vs", "../assets/fontshader.fs");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //calculate_view(objShader, window_width, window_height, glm::vec3(-0.45, 0.0f, 0.0f));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    MyObj1.x = -0.45f;
    MyObj1.y = 0.1f;
    MyObj1.objtype = 1;
    MyObj1.color = primary_color_2;
    MyObj1 = createobj(MyObj1);
    
    MyObj2.x = -0.45f;
    MyObj2.y = 0.1f;
    MyObj2.objtype = 0;
    MyObj2.color = primary_color_3;
    MyObj2 = createobj(MyObj2);

    
    //std::cout << "First  :" << MyObj1.VAO << "\n";
    // Main loop
    
    while (!glfwWindowShouldClose(window))
    {
        //calculate_view(objShader, window_width, window_height, glm::vec3(-0.45, 0.0f,0.0f));
        
        glfwGetWindowSize(window, &window_width, &window_height);
        glfwSetKeyCallback(window, key_callback);
        glClearColor(primary_color_1[0], primary_color_1[1], primary_color_1[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        /*
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), verticesbox);
       */
        
        objShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MyObj1.texture);
        glBindVertexArray(MyObj1.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        
        fontShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MyObj2.texture);
        glBindVertexArray(MyObj2.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //
        ImGui::SetNextWindowSize(ImVec2(window_width / 4.0, window_height * 5.0 / 6.0));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

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
        
        ShowMenu(&show_demo_window);
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
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &MyObj1.VAO);
    glDeleteBuffers(1, &MyObj1.VBO);
    glDeleteBuffers(1, &MyObj1.EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

}
