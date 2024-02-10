#ifndef fileoperations_hpp
#define fileoperations_hpp

#include <stdio.h>
#include <string>

extern std::vector<float> Xposition;
extern std::vector<float> Yposition;
extern std::vector<std::string> objectnames;

int readpdfile(std::string path);
#endif /* fileoperations_hpp */
