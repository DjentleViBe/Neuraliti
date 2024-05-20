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
    NeuralLines *MyObj_line = new NeuralLines[connectnumber];
    char delimiter = ' ';
    std::vector<std::string> lines;
    lines = readfile(path.c_str());
    std::string token;
    int linenumber = 0;
    float foffsetx, toffsetx;
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
            toffsetx = (MyObj_rect[toobj].sentencewidth / (fmax(MyObj_rect[toobj].Inletnum - 1, 1))) * toinlet;
            foffsetx = (MyObj_rect[fromobj].sentencewidth / (fmax(MyObj_rect[toobj].Outletnum - 1, 1))) * fromoutlet;

            MyObj_line[linenumber] = createline2(0, 0, 0, 0);
            MyObj_line[linenumber].startx = MyObj_rect[fromobj].x + foffsetx;
            MyObj_line[linenumber].starty = MyObj_rect[fromobj].y - globalfontsize * 3 / (float)window_height;
            MyObj_line[linenumber].endx = MyObj_rect[toobj].x + toffsetx;
            MyObj_line[linenumber].endy = MyObj_rect[toobj].y;
            MyObj_line[linenumber].startobj = fromobj;
            MyObj_line[linenumber].endobj = toobj;

            linenumber++;
        }
    }
    return MyObj_line;
}

