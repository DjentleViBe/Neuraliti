#ifndef windowing_h
#define windowing_h

#include <stdio.h>
#include "imgui.h"
#include "imfilebrowser.h"

void editprefwindow(ImGui::FileBrowser fileDialog);
void fileopenwindow(ImGui::FileBrowser openDialog);
void closefile();
#endif /* Windowing_h */
