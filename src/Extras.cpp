#include "Extras.h"
#include <stdio.h>
#include <iostream>

int verbose;
std::string logs;

void printverbose(std::string verb, bool check){
    if(check == true){
        std::cout << verb;
    }
    else{
        
    }
}

void addlogs(std::string logtoadd){
    if(verbose==0){
        logs += logtoadd;
    }
}
