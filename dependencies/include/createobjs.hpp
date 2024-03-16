#ifndef createobjs_hpp
#define createobjs_hpp

#include "createobjs.hpp"
#include "stdio.h"
#include <string>
#include "datatypes.hpp"
#include "creategeom.h"

NeuralObj createobj1(int ind, float x, float y, std::string objname, int neuralobjtype);
NeuralLines createline1(float startx, float starty, float endx, float endy);
#endif /* createobjs_hpp */
