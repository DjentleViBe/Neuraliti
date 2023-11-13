#include "MainMenu.h"
#include "imgui.h"
#include "stdio.h"
#include <iostream>
void ShowMenu(bool* p_open)
{
    if(p_open){
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                //ShowExampleMenuFile();
                if (ImGui::MenuItem("New", "CTRL+N")){std::cout << "new" << std::endl;}
                if (ImGui::MenuItem("Open", "CTRL+O")){}
                if (ImGui::MenuItem("Open Recent", "")){}
                if (ImGui::MenuItem("Close", "CTRL+W")){}
                if (ImGui::MenuItem("Save", "CTRL+S")){}
                if (ImGui::MenuItem("Save As...", "CTRL+S")){}
                if (ImGui::MenuItem("Message...", "CTRL+M")){}
                if (ImGui::MenuItem("Print...", "CTRL+P")){}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "SHIFT+CTRL+Z", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                if (ImGui::MenuItem("Duplicate", "CTRL+D")) {}
                if (ImGui::MenuItem("Paste Replace", "")) {}
                if (ImGui::MenuItem("Select All", "CTRL+A")) {}
                if (ImGui::MenuItem("Font", "")) {}
                if (ImGui::MenuItem("Zoom In", "CTRL++")) {}
                if (ImGui::MenuItem("Zoom Out", "CTRL+-")) {}
                if (ImGui::MenuItem("TidyUp", "SHIFT+CTRL+R")) {}
                if (ImGui::MenuItem("(Dis)Connect Selection", "CTRL+K")) {}
                if (ImGui::MenuItem("Triggerize", "CTRL+T")) {}
                if (ImGui::MenuItem("Clear Console", "SHIFT+CTRL+L")) {}
                if (ImGui::MenuItem("Edit Mode", "CTRL+E")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Put"))
            {
                if (ImGui::MenuItem("Object", "CTRL+1")) {}
                if (ImGui::MenuItem("Message", "CTRL+2")) {}
                if (ImGui::MenuItem("Number", "CTRL+3")) {}
                if (ImGui::MenuItem("List", "CTRL+4")) {}
                if (ImGui::MenuItem("Symbol", "")) {}
                if (ImGui::MenuItem("Comment", "CTRL+5")) {}
                if (ImGui::MenuItem("Bang", "SHIFT+CTRL+B")) {}
                if (ImGui::MenuItem("Toggle", "SHIFT+CTRL+T")) {}
                if (ImGui::MenuItem("Number2", "SHIFT+CTRL+N")) {}
                if (ImGui::MenuItem("VSlider", "SHIFT+CTRL+V")) {}
                if (ImGui::MenuItem("HSlider", "SHIFT+CTRL+J")) {}
                if (ImGui::MenuItem("Vradio", "SHIFT+CTRL+D")) {}
                if (ImGui::MenuItem("Hradio", "SHIFT+CTRL+I")) {}
                if (ImGui::MenuItem("VU Meter", "SHIFT+CTRL+U")) {}
                if (ImGui::MenuItem("VU Meter", "SHIFT+CTRL+C")) {}
                if (ImGui::MenuItem("Graph", "SHIFT+CTRL+G")) {}
                if (ImGui::MenuItem("Array", "SHIFT+CTRL+A")) {}
                
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Find"))
            {
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Media"))
            {
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            
            ImGui::EndMainMenuBar();
        }
    }
}
