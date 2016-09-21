#include "Global.h"
#include "DeviceManager.h"

std::vector<CSynData> CSynp;
std::vector<abSynData> abSynp;
std::vector<GJunctData> ESynp;
std::vector<DestexheSynData> DxheSynp;

std::vector<mhHHData> mhHHp;
std::vector<abHHData> abHHp;

inChnData inSpkGenChnp;
outChnData outSpkGenChnp;

std::vector<HHNeuronData> HHNeuronp;

std::vector<DigiDataData> DigiDatap;
std::vector<SDAQData> SDAQp;
#ifdef NATIONAL_INSTRUMENTS
std::vector<NIDAQData> NIDAQp;
#endif
SGData SGp;

std::vector<graphData> Graphp(2);

dataSavingParams dataSavingPs;
std::vector<elecCalibParams> elecCalibPs(MAX_ELECTRODE_NO);

// Attilas Sample-and-Hold
SampleHoldData SampleHoldp;

// LU tables
tanhLUtable tanhLU;
expLUtable expLU;
expSigmoidLUtable expSigmoidLU;

Clock DAQClock;
DeviceManager Devices;
