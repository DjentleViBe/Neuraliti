#ifndef Extras_h
#define Extras_h

#include <iostream>
#include <stdio.h>
#include "picojson.h"

extern std::string logs;
extern int verbose;
extern std::vector<std::string> fontlist, configlist;
extern picojson::value v;

void addlogs(std::string verbose);
std::vector<std::string> listfiles(std::string path, std::string type);
std::vector<std::string> splitString(const std::string& input, char delimiter);
std::vector<std::string> readfile(char const *filename);
std::string readfileconcat(char const *filename);
std::string GetCurrentWorkingDirectory();

#endif /* Extras_h */
