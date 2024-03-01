#include "../dependencies/include/createobjs.hpp"
#include "stdio.h"
#include <string>
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/creategeom.h"
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

NeuralObj createobj1(float x, float y, std::string objname, int neuralobjtype){
    NeuralObj MyObj3;
    if(neuralobjtype == 0){
        MyObj3 = createrect(MyObj3, x, y, 0, primary_color_4, objname);
    }
    else{
        MyObj3 = createfont(MyObj3, x, y, 0, primary_color_5, objname);
    }
    return MyObj3;
}
