#ifndef extras_h
#define extras_h

#include <iostream>
#include <stdio.h>
#include "picojson.h"

extern std::string logs;
extern std::string properties;
extern int verbose;
extern std::vector<std::string> fontlist, configlist, fontsizelist;
extern picojson::value v;

void addlogs(std::string logtoadd);
void addproperties(std::string proptoadd);
void clearproperties();
std::vector<std::string> listfiles(std::string path, std::string type);
std::vector<std::string> splitString(const std::string& input, char delimiter);
std::vector<std::string> readfile(char const *filename);
std::string readfileconcat(char const *filename);
int writefile(std::string file_path, std::string line_to_write);
std::string GetCurrentWorkingDirectory();
void overwriteLine(const std::string& filename, int lineNumber, const std::string& newContent);
void printMatrix(const std::vector<std::vector<int>>& matrix, std::string verbose);
void printvector(const std::vector<int> vec, std::string verbose);
std::string floatToString(float value, int precision);
std::string intToString(int value);
bool isFloat(const std::string& str);
int countSpaces(const std::string& str);
char* doubleToChar(double value);
#endif /* extras_h */
