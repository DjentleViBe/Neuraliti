#include <stdio.h>
#include <iostream>
#include <ctime>
#include <map>
#include <string>
#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h" // Will drag system OpenGL headers
#include "uielements.h"
#define STB_IMAGE_IMPLEMENTATION
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
#include "textgen.h"
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
GLuint programID;
//Shader ourShader("simpleshader.vs", "simpleshader.fs");

bool show_demo_window;
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
int 					window_width 			= 	1280;
int 					window_height 			= 	960;
float primary_color_1[] = {0.93725, 0.89019, 0.79215};
float primary_color_2[] = {0.12941, 0.12941, 0.15294};
float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],    // top right
        0.5f, -0.5f, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],    // bottom right
        -0.5f, -0.5f, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],    // bottom left
        -0.5f,  0.5f, 0.0f,   primary_color_2[0], primary_color_2[1], primary_color_2[2],     // top left
    };
unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

float box1vert[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2],  // top right
         0.5f, -0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2],  // bottom right
        -0.5f, -0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2], // bottom left
        -0.5f,  0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2] // top left
    };
unsigned int box1ind[] = {
        0, 1, 3, // first triangle
        1, 1, 1  // second triangle
    };

unsigned int VBO, VAO, EBO;
unsigned int VBO_box, VAO_box, EBO_box;
GLFWwindow* window;
unsigned int texture1, texture_box;

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
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif

    // Create window with graphics context
    //const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    //window = glfwCreateWindow(mode->width, mode->height, "GENT",  NULL, NULL);
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
    
    InitRectangle(sizeof(vertices), -0.45, 0.0f, sizeof(indices), indices, texture1, VBO, VAO, EBO, "simpleshader.vs", "simpleshader.fs");
    //InitRectangle(sizeof(box1vert), box1vert, sizeof(box1ind), box1ind, texture_box, VBO_box, VAO_box, EBO_box, "windowshader.vs", "windowshader.fs");
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

GLuint calculate_view(Shader ourShader, float wid, float hei, glm::vec3 point){
    
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
    GLuint MatrixID = glGetUniformLocation(ourShader.ID, "ProjMat");
    return MatrixID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //std::cout << window_width << "\n";
    //glViewport(0, 0, height, height);
    Shader ourShader("simpleshader.vs", "simpleshader.fs");
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
    //Shader ourwinShader("windowshader.vs", "windowshader.fs");
    Shader ourShader("simpleshader.vs", "simpleshader.fs");
    Shader shader("text.vs", "text.fs");
    //glm::mat4 projection = glm::ortho(0.0f, 1400.0f, 0.0f, 1260.0f, 0.1f, 100.0f);
    
    //mvp = glm::mat4(1.0f);
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_width), 0.0f, static_cast<float>(window_height));
    
    //shader.use();
    fontface("../assets/fonts/FreeMonoBold.ttf");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // calculate object position
    int logo_x = 0.80 * window_width;
    int logo_y = 0.89 * window_height;
    calculate_view(ourShader, window_width, window_height, glm::vec3(-0.45, 0.0f,0.0f));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Main loop
    
    while (!glfwWindowShouldClose(window))
    {
        calculate_view(ourShader, window_width, window_height, glm::vec3(-0.45, 0.0f,0.0f));
        //glViewport(0,0,1280,960);
        //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwGetWindowSize(window, &window_width, &window_height);
        glfwSetKeyCallback(window, key_callback);
        glClearColor(primary_color_1[0], primary_color_1[1], primary_color_1[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        //RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        //std::cout << 1200.0f / (2 * window_width);
        //RenderText(shader, "NEURALITI", logo_x, logo_y, 1.0f, glm::vec3(0.1294f, 0.1294f, 0.1490f));
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
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
        
        //glfwGetWindowSize(window, &window_width, &window_height);
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        
        // bind textures on corresponding texture units
        //first primitive
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // second primitive
        
        /*
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_box);
        ourwinShader.use();
        glBindVertexArray(VAO_box);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        */
        ImGui::Render();
        
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        
        //glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

}
