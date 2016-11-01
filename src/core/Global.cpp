#include "Global.h"
#include "DeviceManager.h"
#include "ModelManager.h"

int LOADED_PROTOCOL_VERSION;
QString LEGACY_DAQ_CLASS;

std::vector<CSynData> CSynp;
std::vector<abSynData> abSynp;
std::vector<GJunctData> ESynp;
std::vector<DestexheSynData> DxheSynp;

std::vector<mhHHData> mhHHp;
std::vector<abHHData> abHHp;

#include "DigiData.h"
#include "SimulDAQ.h"
std::vector<DigiDataData> &DigiDatap = DigiDataProxy::p;
std::vector<SDAQData> &SDAQp = SimulDAQProxy::p;
#ifdef NATIONAL_INSTRUMENTS
#include "Nidaq.h"
std::vector<NIDAQData> &NIDAQp = NIDAQProxy::p;
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
