#include "Main.h"
#include "Global.h"

int synType[MAX_SYN_NO];
std::vector<CSynData> CSynp(MAX_SYN_NO);
std::vector<abSynData> abSynp(MAX_SYN_NO);
std::vector<GJunctData> ESynp(MAX_SYN_NO);
std::vector<DestexheSynData> DxheSynp(MAX_SYN_NO);

int HHType[MAX_HH_NO];
std::vector<mhHHData> mhHHp(MAX_HH_NO);
std::vector<abHHData> abHHp(MAX_HH_NO);

std::vector<inChnData> inChnp;
std::vector<outChnData> outChnp;
inChnData inSpkGenChnp;
outChnData outSpkGenChnp;

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
