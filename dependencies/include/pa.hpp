#ifndef PA_HPP
#define PA_HPP
#include "portaudio.h"
#include <vector>

extern std::vector<std::string> audioinputdevicelist, audiooutputdevicelist;
extern std::vector<int> audioinputdeviceid, audiooutputdeviceid;
extern std::vector<const PaDeviceInfo*> pdi;

int getnumaudiodevices();
const PaDeviceInfo* getaudiodeviceinfo(int i);
void PrintSupportedStandardSampleRates(
        PaStreamParameters *inputParameters,
        PaStreamParameters *outputParameters );
int INITaudio();
#endif