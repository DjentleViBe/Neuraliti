#include "../dependencies/include/portaudio.h"
#include "../dependencies/include/pa.hpp"
#include <iostream>
#include "../dependencies/include/extras.h"

int audioobjs(){
    PaError err = Pa_Initialize();
    if( err != paNoError ) {
        std::cout << "error" << std::endl;
    }
    printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    // addlogs("PortAudio error:" + std::string str(Pa_GetErrorText(err)));

    err = Pa_Terminate();
    if( err != paNoError ){
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    return 0;
}

int INITaudio(){
    audioobjs();
    return 0;
}