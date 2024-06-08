#ifndef PA_HPP
#define PA_HPP
#include "portaudio.h"

int getnumaudiodevices();
const PaDeviceInfo* getaudiodeviceinfo(int i);
void PrintSupportedStandardSampleRates(
        PaStreamParameters *inputParameters,
        PaStreamParameters *outputParameters );
int INITaudio();
#endif