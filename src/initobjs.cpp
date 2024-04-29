#include "initobjs.hpp"
#include "fileoperations.hpp"
#include "stdio.h"
#include "iostream"
#include <vector>

int initobjs(std::string path){
    readpdfile(path);
    for(int k = 0; k < objectnames.size(); k++){
        std::cout << objectnames[k] << "\n";
    }
    return 0;
}

