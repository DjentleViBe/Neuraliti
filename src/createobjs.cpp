#include "../dependencies/include/createobjs.hpp"
#include "stdio.h"
#include <string>
#include "../dependencies/include/datatypes.hpp"
#include "../dependencies/include/creategeom.h"
#include "../dependencies/include/uielements.h"

int createobjs(){
    return 0;
}

NeuralObj createrect(int ind, NeuralObj MyObj, float x, float y, float objtype, float* color, std::string objname){
    MyObj.x = x;
    MyObj.y = y;
    MyObj.objtype = objtype;
    MyObj.color = color;
    MyObj.objname = objname;
    MyObj.Inletnum = globalinlets[ind];
    MyObj.Outletnum = globaloutlets[ind];
    MyObj = createobj(MyObj);

    return MyObj;
}

NeuralObj createfont(NeuralObj MyObj, float x, float y, float objtype, float* color, std::string objname){
    MyObj.x = x;
    MyObj.y = y;
    MyObj.objtype = objtype;
    MyObj.color = color;
    MyObj.objname = objname;
    MyObj = createobj(MyObj);
    
    return MyObj;
}

NeuralObj createobj1(int ind, float x, float y, std::string objname, int neuralobjtype){
    NeuralObj MyObj3;
    if(neuralobjtype == 0){
        MyObj3 = createrect(ind, MyObj3, x, y, 0, primary_color_2, objname);
    }
    else{
        MyObj3 = createfont(MyObj3, x, y, 0, primary_color_3, objname);
    }
    // create inlets and outlets

    return MyObj3;
}
