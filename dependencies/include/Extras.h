#ifndef Extras_h
#define Extras_h

#include <iostream>
#include <stdio.h>

extern std::string logs;
extern int verbose;
extern std::vector<std::string> fontlist;

void addlogs(std::string verbose);
std::vector<std::string> listfiles(std::string path);
std::vector<std::string> splitString(const std::string& input, char delimiter);
#endif /* Extras_hpp */
