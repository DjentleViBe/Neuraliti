#ifndef initobjs_hpp
#define initobjs_hpp

#include <stdio.h>
#include <string>
#include "datatypes.hpp"

int initobjs(std::string path);
std::vector<NeuralLines> setupconnections(std::vector<NeuralObj> MyObj_rect, std::string path);
#endif /* initobjs_hpp */
