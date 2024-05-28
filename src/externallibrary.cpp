#include <iostream>
#include "../dependencies/include/uielements.h"
#include <dlfcn.h>

// extern float add(float x, float y);
typedef float (*func_t)(int, ...);

int sharedlibrary(int objindex){
    // Load the shared library
    void* handle = dlopen((CurrentDir + "/sharedlib/shrsub.so").c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return 1;
    }
    // std::cout << NC.MyObj_font[objindex].objname << std::endl;
    // Clear any existing error
    dlerror();

    // search for the function name 
    // Define the function name you want to call
    // const char* func_name = "add";  // Change to "subtract" to call the subtract function

    // Load the symbol : check object type
    if(NC.MyObj_rect[objindex].objtype == 0){
        func_t func = (func_t) dlsym(handle, NC.MyObj_font[objindex].funcname.c_str());
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "Cannot load symbol '" << NC.MyObj_font[objindex].funcname << "': " << dlsym_error << std::endl;
            dlclose(handle);
            return 1;
        }
    }
    else if(NC.MyObj_rect[objindex].objtype == 2){
        // map inlet value to outlet. If inlet value is null, map objvalue to outlet, inlet(if) -> objname -> outlet
        //std::cout << NC.MyObj_font[objindex].funcname << std::endl;
    }
    // Call the function
    //func(2, 10.0, 5.0);

    // Close the library
    dlclose(handle);
    return 0;
}