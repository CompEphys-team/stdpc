#include "Global.h"
#include "DeviceManager.h"
#include "ModelManager.h"
#include "ConductanceManager.h"

int LOADED_PROTOCOL_VERSION;
QString LEGACY_DAQ_CLASS;

std::vector<abHHData> abHHp;

PlotData Plotp;
PerformanceMonitorData PerfMonp;

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
ConductanceManager Conductances;
