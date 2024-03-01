#ifndef createobjs_hpp
#define createobjs_hpp

#include "createobjs.hpp"
#include "stdio.h"
#include <string>
#include "datatypes.hpp"
#include "creategeom.h"

NeuralObj createrect(NeuralObj MyObj, float x, float y, float objtype, float color[], std::string objname);
NeuralObj createobj1(float x, float y, std::string objname, int neuralobjtype);

#endif /* createobjs_hpp */
