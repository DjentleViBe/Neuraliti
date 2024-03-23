#include "../dependencies/include/initobjs.hpp"
#include "../dependencies/include/fileoperations.hpp"
#include "stdio.h"
#include "iostream"
#include <vector>
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/extras.h"
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/createobjs.hpp"
#include <sstream>

int initobjs(std::string path){
    auto result = readpdfile(path);
    globalinlets = std::get<0>(result);
    globaloutlets = std::get<1>(result);
    return 0;
}

NeuralLines* setupconnections(NeuralObj* MyObj_rect, std::string path){
    // std::cout << connectnumber << "\n";
    NeuralLines *MyObj_line = new NeuralLines[connectnumber];
    char delimiter = ' ';
    std::vector<std::string> lines;
    lines = readfile(path.c_str());
    std::string token;
    int linenumber = 0;
    for (int l = 0; l < lines.size(); l++){
        std::stringstream ss(lines[l]);
        int fromobj, fromoutlet, toobj, toinlet;
        if(lines[l].find("connect")!= std::string::npos){
            int wordpos = 0;
            while (std::getline(ss, token, delimiter)){
                if(wordpos == 2){
                    // object from number
                    fromobj = stoi(token);
                }
                else if(wordpos == 3){
                    // outlet number
                    fromoutlet = stoi(token);
                }
                else if(wordpos == 4){
                    // object to number
                    toobj = stoi(token);
                }
                else if(wordpos == 5){
                    toinlet = stoi(token);
                }
                wordpos++;
            }
            // determine x y from objnumber

            MyObj_line[linenumber] = createline1(MyObj_rect[fromobj].x, 
                                                MyObj_rect[fromobj].y - globalfontsize * 3 / (float)window_height,
                                                MyObj_rect[toobj].x, 
                                                MyObj_rect[toobj].y);
            linenumber++;
        }
    }
    return MyObj_line;
}

