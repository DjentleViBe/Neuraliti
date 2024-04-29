#include "createobjs.hpp"
#include "stdio.h"
#include <string>
#include "datatypes.hpp"
#include "creategeom.h"
float primary_color_4[]     = {0.12941, 0.12941, 0.15294};
float primary_color_5[]     = {1.0, 1.0, 1.0};
int createobjs(){
    return 0;
}

NeuralObj createrect(NeuralObj MyObj, float x, float y, float objtype, float color[], std::string objname){
    MyObj.x = x;
    MyObj.y = y;
    MyObj.objtype = objtype;
    MyObj.color = color;
    MyObj.objname = objname;
    MyObj = createobj(MyObj);
    
    return MyObj;
}

NeuralObj createfont(NeuralObj MyObj, float x, float y, float objtype, float color[], std::string objname){
    MyObj.x = x;
    MyObj.y = y;
    MyObj.objtype = objtype;
    MyObj.color = color;
    MyObj.objname = objname;
    MyObj = createobj(MyObj);
    
    return MyObj;
}

std::tuple<NeuralObj, NeuralObj> createobj1(float x, float y, std::string objname){
    NeuralObj MyObj3, MyObj4;
    MyObj3 = createrect(MyObj3, x, y, 0, primary_color_4, objname);
    MyObj4 = createfont(MyObj4, x, y, 0, primary_color_5, objname);
    
    return std::make_tuple(MyObj3, MyObj4);
}
