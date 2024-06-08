#include "../dependencies/include/portaudio.h"
#include "../dependencies/include/pa.hpp"
#include <iostream>
#include "../dependencies/include/extras.h"
#include "../dependencies/include/uielements.h"

std::vector<const PaDeviceInfo*> pdi;
std::vector<int> audioinputdeviceid, audiooutputdeviceid;

int audioobjs(){
    PaError err = Pa_Initialize();
    if( err != paNoError ) {
        std::cout << "error" << std::endl;
    }
    printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

    int numdevices = getnumaudiodevices();
    for (int n = 0; n < numdevices; n++){
        pdi.push_back(getaudiodeviceinfo(n));
        if(pdi[n]->maxInputChannels != 0){
            audioinputdevicelist.push_back(pdi[n]->name);
            audioinputdeviceid.push_back(n);
        }
        else{
            audiooutputdevicelist.push_back(pdi[n]->name);
            audiooutputdeviceid.push_back(n);
        }
    }

    return 0;
}

int INITaudio(){
    audioobjs();
    return 0;
}

int ENDaudio(){
    PaError err = Pa_Terminate();
    if( err != paNoError ){
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    return 0;
}