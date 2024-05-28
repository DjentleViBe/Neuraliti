#include <math.h>
#include <iostream>
#include <cstdarg>

extern "C" {
    float add_(int count, ...) {
        va_list args;
        va_start(args, count);
        float sum = 0;
        for (int i = 0; i < count; ++i) {
            sum += va_arg(args, double);
        }
        va_end(args);
        std::cout << sum << std::endl;
        return sum;
    }
}

