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

    int numdevices = getnumaudiodevices();
    for (int n = 0; n < numdevices; n++){
        const PaDeviceInfo* pdi= getaudiodeviceinfo(n);
        if(pdi->maxInputChannels != 0){
            audioinputdevicelist.push_back(pdi->name);
        }
        else{
            audiooutputdevicelist.push_back(pdi->name);
        }
    }

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