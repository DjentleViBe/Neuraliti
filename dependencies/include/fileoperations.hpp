#ifndef fileoperations_hpp
#define fileoperations_hpp

#include <stdio.h>
#include <string>

extern std::vector<float> Xposition;
extern std::vector<float> Yposition;
extern std::vector<std::string> objectnames;
extern int objnumber;

std::tuple<std::vector<int>, std::vector<int>> readpdfile(std::string path);
#endif /* fileoperations_hpp */
