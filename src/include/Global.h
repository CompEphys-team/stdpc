#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVector>
#include <vector>
#include "ObjectDataTypes.h"
#include "LUtables.h"
#include "Clock.h"

#define MAX_ST_HISTORY 20
#define MAX_IN_CHN_NO 64
#define VTHRESH -30
#define MAX_ELECTRODE_NO 4

#define STDPC_PROTOCOL_VERSION 1
#define STDPC_PROTOCOL_HEADER "#StdpC_config_version"
extern int LOADED_PROTOCOL_VERSION;
extern DAQClass LEGACY_DAQ_CLASS;

// Forward
class DeviceManager;


extern std::vector<CSynData> CSynp;
extern std::vector<abSynData> abSynp;
extern std::vector<GJunctData> ESynp;
extern std::vector<DestexheSynData> DxheSynp;

extern std::vector<mhHHData> mhHHp;
extern std::vector<abHHData> abHHp;

extern inChnData inSpkGenChnp;
extern outChnData outSpkGenChnp;

extern std::vector<HHNeuronData> HHNeuronp;

extern std::vector<DigiDataData> DigiDatap;
extern std::vector<SDAQData> SDAQp;
#ifdef NATIONAL_INSTRUMENTS
extern std::vector<NIDAQData> NIDAQp;
#endif
extern SGData SGp;

extern dataSavingParams dataSavingPs;
extern std::vector<elecCalibParams> elecCalibPs;


extern std::vector<graphData> Graphp;

// Attilas Sample-and-Hold
extern SampleHoldData SampleHoldp;

// LU tables
extern tanhLUtable tanhLU;
extern expLUtable expLU;
extern expSigmoidLUtable expSigmoidLU;

extern Clock DAQClock;
extern DeviceManager Devices;

#endif
