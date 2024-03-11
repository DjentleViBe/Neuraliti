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
int connectnumber = 0;
std::vector<float> Xposition;
std::vector<float> Yposition;
std::vector<std::string> objectnames;

std::tuple<std::vector<int>, std::vector<int>> readpdfile(std::string path){
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
        }

    // create connection matrix;
    std::vector<std::vector<int>> connectionmatrix(objnumber, std::vector<int>(objnumber));
    std::vector<int> objinlets(objnumber);
    std::vector<int> objoutlets(objnumber);
    for (int l = 0; l < lines.size(); l++){
        // check if obj
    // check if connect
        if(lines[l].find("connect")!= std::string::npos){
            connectnumber++;
            int connectpos = 0;
            std::stringstream cs(lines[l]);
            std::string tokenconnect;
            std::string tokenconnecttemp = "";
            int obj1 = 0;
            int obj2 = 0;
            while (std::getline(cs, tokenconnect, delimiter)) {
                if(connectpos == 2){
                    // object
                    // std::cout << "\nobject number :" << tokenconnect << "\n";
                    obj1 = stoi(tokenconnect);
                }
                else if(connectpos == 3){
                    // outlet number
                    // std::cout << "outlet number :" << tokenconnect << "\n";
                    objoutlets[obj1] += 1;
                }
                else if(connectpos == 4){
                    // object
                    // std::cout << "object number :" << tokenconnect << "\n";
                    obj2 = stoi(tokenconnect);
                    
                }
                else if(connectpos == 5){
                    // inlet number
                    // std::cout << "inlet number : " << tokenconnect << "\n";
                    objinlets[obj2] += 1;
                }
                connectpos++;
                }
            connectionmatrix[obj1][obj2] += 1;
            std::cout << "\n";
        }
    }
    printMatrix(connectionmatrix, "Connection matrix");
    //printvector(objinlets, "Inlets");
    //printvector(objoutlets, "Outlets");

    return std::make_tuple(objinlets, objoutlets);
}


