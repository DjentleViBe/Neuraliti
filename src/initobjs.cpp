#include "../dependencies/include/initobjs.hpp"
#include "../dependencies/include/fileoperations.hpp"
#include "stdio.h"
#include "iostream"
#include <vector>

int initobjs(std::string path){
    readpdfile(path);
    return 0;
}

