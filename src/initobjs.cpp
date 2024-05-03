#include "../dependencies/include/initobjs.hpp"
#include "../dependencies/include/fileoperations.hpp"
#include "stdio.h"
#include "iostream"
#include <vector>
#include "../dependencies/include/uielements.h"
#include "../dependencies/include/extras.h"

int initobjs(std::string path){
    auto result = readpdfile(path);
    globalinlets = std::get<0>(result);
    globaloutlets = std::get<1>(result);
    return 0;
}

