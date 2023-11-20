#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ReadFile.h"

std::vector<std::string> readfile(char const *filename){
    std::string line;
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            //std::cout << line << "\n";
            lines.push_back(line);
        }
        file.close();
    }
    return lines;
}
