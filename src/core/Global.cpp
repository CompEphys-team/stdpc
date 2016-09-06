#include "Main.h"
#include "Global.h"

std::vector<CSynData> CSynp;
std::vector<abSynData> abSynp;
std::vector<GJunctData> ESynp;
std::vector<DestexheSynData> DxheSynp;

std::vector<mhHHData> mhHHp;
std::vector<abHHData> abHHp;

std::vector<inChnData> inChnp;
std::vector<outChnData> outChnp;
inChnData inSpkGenChnp;
outChnData outSpkGenChnp;

std::vector<HHNeuronData> HHNeuronp;

DigiDataData DigiDatap;
SDAQData SDAQp;
#ifdef NATIONAL_INSTRUMENTS
NIDAQData NIDAQp;
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
