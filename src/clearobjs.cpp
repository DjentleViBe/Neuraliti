#include <string>
#include <iostream>
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/fileoperations.hpp"

void clearobjs(){
    connectnumber = 0;
    objnumber = 0;
    
    NC.MyObj_floats.erase( NC.MyObj_floats.begin(),  NC.MyObj_floats.end());
    NC.MyObj_font.erase(NC.MyObj_font.begin(), NC.MyObj_font.end());
    NC.MyObj_rect.erase(NC.MyObj_rect.begin(), NC.MyObj_rect.end());
    NC.MyObj_lines.erase(NC.MyObj_lines.begin(), NC.MyObj_lines.end());
    
}

void clearlines(int maxinlet, int objnumber){
    for (int l = NC.MyObj_lines.size() - 1; l >= 0; --l) {
        if (NC.MyObj_lines[l].endobj == objnumber) {
            std::cout << "delete" << std::endl;
            NC.MyObj_lines.erase(NC.MyObj_lines.begin() + l);
    }
}
}