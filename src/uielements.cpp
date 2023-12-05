#include <stdio.h>
#include <iostream>
#include <ctime>
#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h" // Will drag system OpenGL headers
#include "uielements.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader_s.h"
#include "CreateWindow.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MainMenu.h"
#include "KeyBindings.h"
#include "Extras.h"
#include "imfilebrowser.h"
#include "Windowing.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
bool show_demo_window;
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
int 					window_width 			= 	1393;
int 					window_height 			= 	1193;
float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

float primary_color_1[] = {0.93725, 0.89019, 0.79215};
float primary_color_2[] = {0.12941, 0.12941, 0.15294};
float box1vert[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2],  // top right
         0.5f, -0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2],  // bottom right
        -0.5f, -0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2], // bottom left
        -0.5f,  0.5f, 0.0f,  primary_color_2[0], primary_color_2[1], primary_color_2[2] // top left
    };
unsigned int box1ind[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };


unsigned int VBO, VAO, EBO;
unsigned int VBO_box, VAO_box, EBO_box;
GLFWwindow* window;
unsigned int texture1, texture_box;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int INITgraphics(){
    addlogs("Initialisation started\n");
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
    glfwSetWindowAspectRatio(window,1189,1000);
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

    //InitRectangle(sizeof(vertices), vertices, sizeof(indices), indices, texture1, VBO, VAO, EBO, "simpleshader.vs", "simpleshader.fs");
    InitRectangle(sizeof(box1vert), box1vert, sizeof(box1ind), box1ind, texture_box, VBO_box, VAO_box, EBO_box, "windowshader.vs", "windowshader.fs");
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
    ImFontConfig config;
    
    static const ImWchar myGlyphRanges[] = {
        0x21E7, 0x21E7, // shift
        0x2318, 0x2318, // cmd
        0
    };
    addlogs("Font loading\n");
    io.Fonts->AddFontFromFileTTF("FreeMono.ttf", 17);
    static ImFontConfig cfg;
    cfg.OversampleH = cfg.OversampleV = 2;
    cfg.MergeMode = true;
    #if defined __APPLE__
    io.Fonts->AddFontFromFileTTF("FreeMono.ttf", 17, &cfg,
                                 myGlyphRanges);
    #endif
    io.Fonts->Build();
    addlogs("Font loaded\n");
    readkeybindings();
    
    addlogs("Initialisation ended\n");
    return 0;
}

void Displayloop(char **argv){
    bool show_demo_window = true;
    ImGuiIO io = ImGui::GetIO();
    ImGui::FileBrowser fileDialog;
    Shader ourShader("simpleshader.vs", "simpleshader.fs");
    Shader ourwinShader("windowshader.vs", "windowshader.fs");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Main loop
    while (!glfwWindowShouldClose(window))
    {   
        glfwGetWindowSize(window, &window_width, &window_height);
        glfwSetKeyCallback(window, key_callback);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //
        ImGui::SetNextWindowSize(ImVec2(window_width / 4.0, window_height * 5.0 / 6.0));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("Window A");
        ImGui::Text("This is main window");
        // open file dialog when user clicks this button
        
                
        if(fileDialog.HasSelected()){
            std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
            fileDialog.ClearSelected();
        }
        ImGui::End();
        
        
        //
        ImGui::SetNextWindowSize(ImVec2(window_width - (window_width / 4.0), window_height - window_height * 5.0 / 6.0));
        ImGui::SetNextWindowPos(ImVec2(window_width / 4.0, (window_height * 5.0 / 6.0)));

        ImGui::Begin("Debug");
        time_t now = time(0);
        // convert now to string form
        const char *date_time = ctime(&now);
        ImGui::Text("%s", date_time);
        if(editpref){
            editprefwindow(editpref, fileDialog);
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
        glClearColor(primary_color_1[0], primary_color_1[1], primary_color_1[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glfwGetWindowSize(window, &window_width, &window_height);
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        
        // bind textures on corresponding texture units
        //first primitive
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        //ourShader.use();
        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // second primitive
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_box);
        ourwinShader.use();
        glBindVertexArray(VAO_box);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
