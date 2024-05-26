#include <iostream>
#include "../dependencies/include/uielements.h"
#include <dlfcn.h>

// extern float add(float x, float y);
typedef float (*func_t)(float, float);

/*
void readlibrary(){
    std::cout << add(4.0, 5.0) << std::endl;
}*/

int sharedlibrary(){
    // Load the shared library
    void* handle = dlopen((CurrentDir + "./sharedlib/shrsub.so").c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return 1;
    }

    // Clear any existing error
    dlerror();

    // Define the function name you want to call
    const char* func_name = "add";  // Change to "subtract" to call the subtract function

    // Load the symbol
    func_t func = (func_t) dlsym(handle, func_name);
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol '" << func_name << "': " << dlsym_error << std::endl;
        dlclose(handle);
        return 1;
    }

    // Call the function
    func(10.0, 5.0);

    // Close the library
    dlclose(handle);
    return 0;
}