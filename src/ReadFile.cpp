#include <iostream>
#include <fstream>
#include <string>
#include "ReadFile.h"

void readfile(char const *filename){
    std::string line;
    std::ifstream file(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            std::cout << line << "\n";
        }
        file.close();
    }
}
