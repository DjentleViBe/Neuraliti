#include "MainMenu.h"
#include "imgui.h"
#include "stdio.h"
#include <iostream>

std::string superkey = "";

void ShowMenu(bool* p_open)
{
    #if defined _WIN32
    superkey = "CTRL";
    #elif defined __APPLE__
    superkey = "CMD";
    #endif
    if(p_open){
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                //ShowExampleMenuFile();
                if (ImGui::MenuItem("New", (superkey + "+N").c_str())){std::cout << "new" << std::endl;}
                if (ImGui::MenuItem("Open", (superkey + "+O").c_str())){}
                if (ImGui::MenuItem("Open Recent", "")){}
                if (ImGui::MenuItem("Close", (superkey + "+W").c_str())){}
                if (ImGui::MenuItem("Save", (superkey + "+S").c_str())){}
                if (ImGui::MenuItem("Save As...", ("SHIFT+" + superkey + "+S").c_str())){}
                if (ImGui::MenuItem("Message...", (superkey + "+M").c_str())){}
                if (ImGui::MenuItem("Print...", (superkey + "+P").c_str())){}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", (superkey + "+Z").c_str())) {}
                if (ImGui::MenuItem("Redo", ("SHIFT+"+superkey+"+Z").c_str(), false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", (superkey + "+X").c_str())) {}
                if (ImGui::MenuItem("Copy", (superkey + "+C").c_str())) {}
                if (ImGui::MenuItem("Paste", (superkey + "+V").c_str())) {}
                if (ImGui::MenuItem("Duplicate", (superkey + "+D").c_str())) {}
                if (ImGui::MenuItem("Paste Replace", "")) {}
                if (ImGui::MenuItem("Select All", (superkey + "+A").c_str())) {}
                if (ImGui::MenuItem("Font", "")) {}
                if (ImGui::MenuItem("Zoom In", (superkey + "++").c_str())) {}
                if (ImGui::MenuItem("Zoom Out", (superkey + "+-").c_str())) {}
                if (ImGui::MenuItem("TidyUp", ("SHIFT+" + superkey + "+R").c_str())) {}
                if (ImGui::MenuItem("(Dis)Connect Selection", (superkey + "+K").c_str())) {}
                if (ImGui::MenuItem("Triggerize", (superkey + "+T").c_str())) {}
                if (ImGui::MenuItem("Clear Console", ("SHIFT+" + superkey + "+L").c_str())) {}
                if (ImGui::MenuItem("Edit Mode", (superkey + "+E").c_str())) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Put"))
            {
                if (ImGui::MenuItem("Object", (superkey + "+1").c_str())) {}
                if (ImGui::MenuItem("Message", (superkey + "+2").c_str())) {}
                if (ImGui::MenuItem("Number", (superkey + "+3").c_str())) {}
                if (ImGui::MenuItem("List", (superkey + "+4").c_str())) {}
                if (ImGui::MenuItem("Symbol", "")) {}
                if (ImGui::MenuItem("Comment", (superkey + "+5").c_str())) {}
                if (ImGui::MenuItem("Bang", ("SHIFT+" + superkey + "+B").c_str())) {}
                if (ImGui::MenuItem("Toggle", ("SHIFT+" + superkey + "+T").c_str())) {}
                if (ImGui::MenuItem("Number2", ("SHIFT+" + superkey + "+N").c_str())) {}
                if (ImGui::MenuItem("VSlider", ("SHIFT+" + superkey + "+V").c_str())) {}
                if (ImGui::MenuItem("HSlider", ("SHIFT+" + superkey + "+J").c_str())) {}
                if (ImGui::MenuItem("Vradio", ("SHIFT+" + superkey + "+D").c_str())) {}
                if (ImGui::MenuItem("Hradio", ("SHIFT+" + superkey + "+I").c_str())) {}
                if (ImGui::MenuItem("VU Meter", ("SHIFT+" + superkey + "+U").c_str())) {}
                if (ImGui::MenuItem("Equilizer", ("SHIFT+" + superkey + "+E").c_str())) {}
                if (ImGui::MenuItem("Graph", ("SHIFT+" + superkey + "+G").c_str())) {}
                if (ImGui::MenuItem("Array", ("SHIFT+" + superkey + "+A").c_str())) {}
                
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
