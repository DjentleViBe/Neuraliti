#ifndef creategeom_h
#define creategeom_h

#include <stdio.h>
#include "datatypes.hpp"

extern int sentence_width;
NeuralObj createobj(NeuralObj &MyObj);
NeuralLines createlines(NeuralLines &MyObj);
NeuralLines createhorizontallines(NeuralLines &MyObj);
NeuralObj createhorizontallines(NeuralObj &MyObj);
#endif /* creategeom_h */
