#include <math.h>
#include <iostream>

extern "C" {
    float add(float a, float b) {
        std::cout << a + b << std::endl;
        return a + b;
    }

    int subtract(int a, int b) {
        return a - b;
    }
}

