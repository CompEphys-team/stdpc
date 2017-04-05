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

#define STDPC_PROTOCOL_VERSION 4
#define STDPC_PROTOCOL_HEADER "#StdpC_config_version"
extern int LOADED_PROTOCOL_VERSION;
extern QString LEGACY_DAQ_CLASS;

// Forward
class DeviceManager;
class ModelManager;
class ConductanceManager;

extern dataSavingParams dataSavingPs;

extern PlotData Plotp;
extern PerformanceMonitorData PerfMonp;

// Attilas Sample-and-Hold
extern SampleHoldData SampleHoldp;

// LU tables
extern tanhLUtable tanhLU;
extern expLUtable expLU;
extern expSigmoidLUtable expSigmoidLU;

extern Clock DAQClock;
extern DeviceManager Devices;
extern ModelManager Models;
extern ConductanceManager Conductances;

#endif
