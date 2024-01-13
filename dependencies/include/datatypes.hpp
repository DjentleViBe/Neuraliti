#ifndef datatypes_hpp
#define datatypes_hpp
#include "stdio.h"
#include <string>

class NeuralObj {       // The class
  public:               // Access specifier
    int objtype;        // 0: Object, 1: bang, 2: float
    float x;            // x position
    float y;            // y position
    std::string myString;  // Attribute (string variable)
};

#endif
