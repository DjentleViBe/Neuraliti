#ifndef fileoperations_hpp
#define fileoperations_hpp

#include <stdio.h>
#include <string>

extern std::vector<float> Xposition;
extern std::vector<float> Yposition;
extern std::vector<std::string> objectnames;
extern std::vector<int> objecttypes;
extern int objnumber;
extern int connectnumber;
extern std::vector<std::vector<int>> connectionmatrix;
std::tuple<std::vector<int>, std::vector<int>> readpdfile(std::string path);
#endif /* fileoperations_hpp */
