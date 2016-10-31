#include "Global.h"
#include "DeviceManager.h"
#include "ModelManager.h"

int LOADED_PROTOCOL_VERSION;
DAQClass LEGACY_DAQ_CLASS;

std::vector<CSynData> CSynp;
std::vector<abSynData> abSynp;
std::vector<GJunctData> ESynp;
std::vector<DestexheSynData> DxheSynp;

std::vector<mhHHData> mhHHp;
std::vector<abHHData> abHHp;


#include "HHNeuron.h"
#include "SpkGen.h"
std::vector<HHNeuronData> &HHNeuronp = HHNeuronProxy::p;
std::vector<SGData> &SGp = SpkGenProxy::p;

std::vector<DigiDataData> DigiDatap;
std::vector<SDAQData> SDAQp;
#ifdef NATIONAL_INSTRUMENTS
std::vector<NIDAQData> NIDAQp;
#endif

std::vector<GraphData> Graphp;

dataSavingParams dataSavingPs;

// Attilas Sample-and-Hold
SampleHoldData SampleHoldp;

// LU tables
tanhLUtable tanhLU;
expLUtable expLU;
expSigmoidLUtable expSigmoidLU;

Clock DAQClock;
DeviceManager Devices;
ModelManager Models;
