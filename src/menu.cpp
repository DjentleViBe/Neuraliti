#include "MainMenu.h"
#include "imgui.h"
#include "stdio.h"
#include <iostream>
#define _S(_LITERAL)    (const char*)u8##_LITERAL
std::string superkey = "";
bool saveas;
bool openfile;
bool editpref;

void ShowMenu(bool* p_open)
{
    #if defined _WIN32
    superkey = "CTRL";
    #elif defined __APPLE__
    superkey = "⌘";
    #endif
    if(p_open){
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", (superkey + " N").c_str())){std::cout << "new" << std::endl;}
                if (ImGui::MenuItem("Open", (superkey + " O").c_str())){}
                if (ImGui::MenuItem("Open Recent", "")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Close", (superkey + " W").c_str())){}
                if (ImGui::MenuItem("Save", (superkey + " S").c_str())){}
                if (ImGui::MenuItem("Save As...", ("⇧ " + superkey + " S").c_str())){}
                ImGui::Separator();
                if (ImGui::MenuItem("Message...", (superkey + " M").c_str())){}
                ImGui::Separator();
                if (ImGui::MenuItem("Print...", (superkey + " P").c_str())){}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", (superkey + " Z").c_str())) {}
                if (ImGui::MenuItem("Redo", ("⇧ " + superkey + " Z").c_str(), false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", (superkey + " X").c_str())) {}
                if (ImGui::MenuItem("Copy", (superkey + " C").c_str())) {}
                if (ImGui::MenuItem("Paste", (superkey + " V").c_str())) {}
                if (ImGui::MenuItem("Duplicate", (superkey + " D").c_str())) {}
                if (ImGui::MenuItem("Paste Replace", "")) {}
                if (ImGui::MenuItem("Select All", (superkey + " A").c_str())) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Font", "")) {}
                if (ImGui::MenuItem("Zoom In", (superkey + " +").c_str())) {}
                if (ImGui::MenuItem("Zoom Out", (superkey + " -").c_str())) {}
                if (ImGui::MenuItem("TidyUp", ("⇧ " + superkey + " R").c_str())) {}
                if (ImGui::MenuItem("(Dis)Connect Selection", (superkey + " K").c_str())) {}
                if (ImGui::MenuItem("Triggerize", (superkey + " T").c_str())) {}
                if (ImGui::MenuItem("Clear Console", ("⇧ " + superkey + " L").c_str())) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Edit Mode", (superkey + " E").c_str())) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Put"))
            {
                if (ImGui::MenuItem("Object", (superkey + " 1").c_str())) {}
                if (ImGui::MenuItem("Message", (superkey + " 2").c_str())) {}
                if (ImGui::MenuItem("Number", (superkey + " 3").c_str())) {}
                if (ImGui::MenuItem("List", (superkey + " 4").c_str())) {}
                if (ImGui::MenuItem("Symbol", "")) {}
                if (ImGui::MenuItem("Comment", (superkey + " 5").c_str())) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Bang", ("⇧ " + superkey + " B").c_str())) {}
                if (ImGui::MenuItem("Toggle", ("⇧ " + superkey + " T").c_str())) {}
                if (ImGui::MenuItem("Number2", ("⇧ " + superkey + " N").c_str())) {}
                if (ImGui::MenuItem("VSlider", ("⇧ " + superkey + " V").c_str())) {}
                if (ImGui::MenuItem("HSlider", ("⇧ " + superkey + " J").c_str())) {}
                if (ImGui::MenuItem("Vradio", ("⇧ " + superkey + " D").c_str())) {}
                if (ImGui::MenuItem("Hradio", ("⇧ " + superkey + " I").c_str())) {}
                if (ImGui::MenuItem("VU Meter", ("⇧ " + superkey + " U").c_str())) {}
                if (ImGui::MenuItem("Canvas", ("⇧ " + superkey + " C").c_str())) {}
                if (ImGui::MenuItem("Equilizer", ("⇧ " + superkey + " E").c_str())) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Graph", ("⇧ " + superkey + " G").c_str())) {}
                if (ImGui::MenuItem("Array", ("⇧ " + superkey + " A").c_str())) {}
                
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
                if (ImGui::MenuItem("Audio Settings...")) {}
                if (ImGui::MenuItem("MIDI Settings...")) {}
                if (ImGui::BeginMenu("Preferences")) {
                    if(ImGui::MenuItem("Edit Preferences...")){
                        editpref = true;
                    };
                    ImGui::Separator();
                    if(ImGui::MenuItem("Save to...")){};
                    if(ImGui::MenuItem("Load from...")){};
                    ImGui::EndMenu();
                            }

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
