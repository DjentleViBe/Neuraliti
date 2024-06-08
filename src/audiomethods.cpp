#include "../dependencies/include/portaudio.h"
#include "../dependencies/include/pa.hpp"
#include <iostream>

int getnumaudiodevices(){
    int numaudioDevices;
    numaudioDevices = Pa_GetDeviceCount();
    if( numaudioDevices < 0 )
    {
        // printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
        // PaError errnum = numDevices;
        std::cout << "device count error" << std::endl;
    }
    return numaudioDevices;
}

const PaDeviceInfo* getaudiodeviceinfo(int i){
    const PaDeviceInfo *audiodeviceInfo;
    audiodeviceInfo = Pa_GetDeviceInfo(i);
    return audiodeviceInfo;
}

void PrintSupportedStandardSampleRates(
        PaStreamParameters *inputParameters,
        PaStreamParameters *outputParameters )
{
    static double standardSampleRates[] = {
        8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
        44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
    };
    int     i, printCount;
    PaError err;

    printCount = 0;
    for( i=0; standardSampleRates[i] > 0; i++ )
    {
        err = Pa_IsFormatSupported( inputParameters, outputParameters, standardSampleRates[i] );
        if( err == paFormatIsSupported )
        {
            if( printCount == 0 )
            {
                printf( "\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else if( printCount == 4 )
            {
                printf( ",\n\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else
            {
                printf( ", %8.2f", standardSampleRates[i] );
                ++printCount;
            }
        }
    }
    if( !printCount )
        printf( "None\n" );
    else
        printf( "\n" );
}
