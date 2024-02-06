#include "fileoperations.hpp"
#include <string>
#include "stdio.h"
#include "stdlib.h"
#include "extras.h"

int readpdfile(std::string path){
    std::vector<std::string> lines;
    lines = readfile(path.c_str());
    return 0;
}
