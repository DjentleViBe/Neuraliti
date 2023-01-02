#include <cstring> // for strerror
#include <cstdlib> // for _getcwd (Windows) or getcwd (Unix-like)
#include <unistd.h>
#include <fstream>
#include "Windowing.h"
#include "imgui.h"
#include "imfilebrowser.h"
#include "MainMenu.h"
#include "Extras.h"
#include "picojson.h"

void editprefwindow(ImGui::FileBrowser fileDialog){
    // Buffer to hold the current working directory
    //ImGui::SetNextWindowPos(ImVec2(window_width / 2.0, (window_height / 4.0)));
    ImGui::Begin("Preferences", &editpref);
    
    /*if(ImGui::Button("Choose font")){
        fileDialog.Open();
        fileDialog.Display();
    }*/
    if (ImGui::TreeNode("Defaults"))
    {
        ImGui::Text("%s", "prefs.json [compiled]");
        ImGui::Text("%s", appsettings["defaultfolder"].c_str());
        ImGui::Text("%s", (appsettings["defaultfont"] + ", " + appsettings["fontsize"] ).c_str());
        ImGui::TreePop();
    }
    ImGui::Separator();
    static int currentItem = 0;
    if (ImGui::TreeNode("Fonts"))
    {
        ImGui::Text("Font name");
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
        ImGui::TreePop();
    }
    if(ImGui::Button("Save")){
        // write to json
        addlogs(v.get("Media").get("Preferences").get("EditPreferences").get("defaults").get("defaultfolder").to_str());
        
        picojson::object& person = v.get<picojson::object>();
        person["defaultfolder"] = picojson::value("neuraliti");

        // Serialize the modified JSON object to a string
        std::string serialized_json = picojson::value(v).serialize();

        // Write the updated JSON data back to the file
        std::ofstream outfile("../prefs.json");
        if (!outfile.is_open()) {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
            }
        outfile << serialized_json;
        outfile.close();

        addlogs("\nPreferences saved. Restart the app to see changes\n");
        //addlogs(appsettings["defaultfont"]);
    }
    //ImGui::Button("Save");
}
