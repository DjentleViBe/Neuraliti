#include <cstring> // for strerror
#include <cstdlib> // for _getcwd (Windows) or getcwd (Unix-like)
#include <unistd.h>
#include <fstream>
#include "../dependencies/include/windowing.h"
#include "../dependencies/include/imgui.h"
#include "../dependencies/include/imfilebrowser.h"
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/menu.h"
#include "../dependencies/include/extras.h"
#include "../dependencies/include/picojson.h"
#include "../dependencies/include/initobjs.hpp"
#include "../dependencies/include/pa.hpp"

void editprefwindow(ImGui::FileBrowser fileDialog){
    // Buffer to hold the current working directory
    ImGui::SetNextWindowPos(ImVec2(window_width / 4.0, (window_height / 4.0)));
    ImGui::SetNextWindowSize(ImVec2(window_width / 3.0, (window_height / 3.0)));
    ImGui::Begin("Preferences", &editpref);
    
    if (ImGui::TreeNode("Defaults"))
    {
        ImGui::Text("%s", "prefs.json [compiled]");
        ImGui::Text("%s", appsettings["defaultfolder"].c_str());
        ImGui::Text("%s", (appsettings["defaultfont"] + ", " + appsettings["fontsize"] ).c_str());
        ImGui::TreePop();
    }
    ImGui::Separator();
    static int currentItem = 0;
    static int currentSize = std::stof(appsettings["fontsize"]) - 12;
    if (ImGui::TreeNode("Fonts"))
    {
        ImGui::PushItemWidth(window_width / 7.0);
        if (ImGui::BeginCombo("##combo", fontlist[currentItem].c_str())) {
            for (int i = 0; i < fontlist.size(); i++) {
                const bool isSelected = (currentItem == i);
                if (ImGui::Selectable(fontlist[i].c_str(), isSelected))
                    currentItem = i;
                    appsettings["defaultfont"] = fontlist[currentItem];
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (ImGui::BeginCombo("##combo2", fontsizelist[currentSize].c_str())) {
            for (int j = 0; j < fontsizelist.size(); j++) {
                const bool isSelected = (currentSize == j);
                if (ImGui::Selectable(fontsizelist[j].c_str(), isSelected))
                    currentSize = j;
                    appsettings["fontsize"] = fontsizelist[currentSize];
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::TreePop();
        
    }
    if(ImGui::Button("Save")){
        // write to json
        // Write the updated JSON data back to the file
        overwriteLine("../prefs.json", 8, "\"fontsize\": \"" + appsettings["fontsize"] + "\"");
        addlogs("\nPreferences saved. Restart the app to see changes\n");
    }
}

void closefile(){
    clearobjs();
}

void fileopenwindow(ImGui::FileBrowser openDialog){
    //ImGui::Begin("File", &openfile);
    openDialog.SetTypeFilters({ ".pd" });
    // open file dialog when user clicks this button
    openDialog.SetPwd(CurrentDir);
    openDialog.Open();

    openDialog.Display();
    
    if(openDialog.HasSelected())
    {
        std::cout << "Selected filename" << openDialog.GetSelected().string() << std::endl;
        filename = openDialog.GetSelected().string();
        filename = filename.substr(filename.rfind("/") + 1);
        // write the file name to .history
        std::cout << CurrentDir + "/" + filename << std::endl;
        writefile(CurrentDir + "/.history", filename);
        clearobjs();
        initobjs(CurrentDir + "/" + filename);
        loadobjects();
        openfile = false;
        // sleep(1);
        openDialog.ClearSelected();

    }
    //ImGui::End();
}

void audioprefwindow(ImGui::FileBrowser fileDialog){
    ImGui::SetNextWindowPos(ImVec2(window_width / 4.0, (window_height / 4.0)));
    ImGui::SetNextWindowSize(ImVec2(window_width / 3.0, (window_height / 3.0)));
    ImGui::Begin("Audio Preferences", &audiopref);
    static int currentinputItem = 0;
    static int currentoutputItem = 0;
    if (ImGui::CollapsingHeader("Audio devices",  ImGuiTreeNodeFlags_DefaultOpen)){
        if (ImGui::TreeNodeEx("Input Device", ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::SetNextItemWidth(window_width * 0.7 / 4.0);
            if (ImGui::BeginCombo("Input Device", audioinputdevicelist[currentinputItem].c_str())) {
                for (int i = 0; i < audioinputdevicelist.size(); i++) {
                    const bool isSelected = (currentinputItem == i);
                    if (ImGui::Selectable(audioinputdevicelist[i].c_str(), isSelected))
                        currentinputItem = i;

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            NC.MyObj_inputaudio.device_name = pdi[audioinputdeviceid[currentinputItem]]->name;
            NC.MyObj_inputaudio.sample_rate = pdi[audioinputdeviceid[currentinputItem]]->defaultSampleRate;
            ImGui::Text("%s", "Device name : ");
            ImGui::SameLine();
            ImGui::Text("%s", NC.MyObj_inputaudio.device_name.c_str());
            ImGui::Text("%s", "Sample rate : ");
            ImGui::SameLine();
            ImGui::Text("%s", doubleToChar(NC.MyObj_inputaudio.sample_rate));
            ImGui::TreePop();
        }
        ImGui::Separator();
        if (ImGui::TreeNodeEx("Output Device", ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::SetNextItemWidth(window_width * 0.7 / 4.0);
            if (ImGui::BeginCombo("Output Device", audiooutputdevicelist[currentoutputItem].c_str())) {
                for (int i = 0; i < audiooutputdevicelist.size(); i++) {
                    const bool isSelected = (currentoutputItem == i);
                    if (ImGui::Selectable(audiooutputdevicelist[i].c_str(), isSelected))
                    
                        currentoutputItem = i;

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            NC.MyObj_outputaudio.device_name = pdi[audiooutputdeviceid[currentoutputItem]]->name;
            NC.MyObj_outputaudio.sample_rate = pdi[audiooutputdeviceid[currentoutputItem]]->defaultSampleRate;
            ImGui::Text("%s", "Device name : ");
            ImGui::SameLine();
            ImGui::Text("%s", NC.MyObj_outputaudio.device_name.c_str());
            ImGui::Text("%s", "Sample rate : ");
            ImGui::SameLine();
            ImGui::Text("%s", doubleToChar(NC.MyObj_outputaudio.sample_rate));
            ImGui::TreePop();
        } 
    }
}
