#include "../dependencies/include/fileoperations.hpp"
#include <string>
#include "stdio.h"
#include "stdlib.h"
#include "../dependencies/include/extras.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

int objnumber = 0;
std::vector<float> Xposition;
std::vector<float> Yposition;
std::vector<std::string> objectnames;

int readpdfile(std::string path){
    std::vector<std::string> lines;
    char delimiter = ' ';
    lines = readfile(path.c_str());
    // loop through the lines one by one
    std::vector<std::string> tokens;
    for (int l = 0; l < lines.size(); l++){
        // check if obj
        if(lines[l].find("obj")!= std::string::npos){
            objnumber++;
            int wordpos = 0;
            std::stringstream ss(lines[l]);
            std::string token;
            std::string tokentemp = "";
            while (std::getline(ss, token, delimiter)) {
                tokens.push_back(token);
                if(wordpos == 2){
                    Xposition.push_back(stof(token));
                }
                else if(wordpos == 3){
                    Yposition.push_back(stof(token));
                }
                else if(wordpos == 4){
                    tokentemp += token;
                }
                else if(wordpos > 4){
                    tokentemp += " " + token;
                }
                wordpos++;
                }
            objectnames.push_back(tokentemp.erase(tokentemp.size() - 1));
            }
        // check if connect
        else if(lines[l].find("connect")!= std::string::npos){
            
            }
        }
    return 0;
    }
