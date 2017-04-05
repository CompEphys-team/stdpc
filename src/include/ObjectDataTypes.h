#ifndef OBJECTDATATYPES_H
#define OBJECTDATATYPES_H

using namespace std;

#include <QString>
#include <iostream>
#include <QColor>
#include "ChannelIndex.h"
#include "Util.h"

struct AssignmentData {
    bool active;
};

struct SynapseAssignment : public AssignmentData {
    ChannelIndex PreSynChannel;
    ChannelIndex PostSynChannel;
    ChannelIndex OutSynChannel;
    double delay;
};

struct GapJunctionAssignment : public AssignmentData {
    ChannelIndex preInChannel;
    ChannelIndex postInChannel;
    ChannelIndex preOutChannel;
    ChannelIndex postOutChannel;
};

struct CurrentAssignment : public AssignmentData {
    ChannelIndex VChannel;
    ChannelIndex IChannel;
};

struct ConductanceData {
    bool active;
    virtual const AssignmentData &assignment(size_t i) const = 0;
    virtual size_t numAssignments() const = 0;
    ConductanceData() : active(false) {}
};

struct SynapseData : public ConductanceData {
    inline const SynapseAssignment &assignment(size_t i) const { return assign[i]; }
    inline size_t numAssignments() const { return assign.size(); }
    std::vector<SynapseAssignment> assign;
    int legacy_PreSyn = -1;
    int legacy_PostSyn = -1;
    int legacy_OutSyn = -1;
};

typedef struct {
  double AMinus;
  double tauMinus;
  double APlus;
  double tauPlus;
  int Exponent;
  double Shift;
  int History;
  bool Table;
  double tableDt;
  double tableDgMin;
  double gMax;
  double gMid;
  double gSlope;
  bool sigmoidTable;
  double sigmoidTableDg;
  double sigmoidTableMaxEntry;
} STPlast;

typedef struct {
  double InitialP;
  double InitialD;
  double betaP;
  double betaD;
  double gamma;
  int eta;
  double highP;
  double lowP;
  double highD;
  double lowD;
  double gMax;
  double gMid;
  double gSlope;
  bool sigmoidTable;
  double sigmoidTableDg;
  double sigmoidTableMaxEntry;
} ODEPlast;

typedef struct {
  bool active;
  bool LUTables;
  double gSyn;
  double Vpre;
  double Vrev;
  double trelease;
  double alpha;
  double beta;
  int fixVpost;
  double Vpost;
  int Plasticity;
  STPlast ST;
  ODEPlast ODE;
  std::vector<SynapseAssignment> assign;
  int legacy_PreSyn = -1;
  int legacy_PostSyn = -1;
  int legacy_OutSyn = -1;
} DestexheSynData;

typedef struct {
  bool active;
  int type;
  double gSyn;
  std::vector<GapJunctionAssignment> assign;
  int legacy_PreIn = -1;
  int legacy_PostIn= -1;
  int legacy_PreOut = -1;
  int legacy_PostOut = -1;
} GJunctData;

typedef struct {
  bool active;
  bool LUTables;
  double gMax;
  double Vrev;
  int mExpo;
  int hExpo;
  double Vm;
  double sm;
  double Cm;
  int taumType;
  double taum;
  double taumAmpl;
  double Vtaum;
  double staum;
  double Vh;
  double sh;
  double Ch;
  int tauhType;
  double tauh;
  double tauhAmpl;
  double Vtauh;
  double stauh;
  std::vector<CurrentAssignment> assign;
  int legacy_V = -1;
  int legacy_I = -1;
} mhHHData;

typedef struct {
  bool active;
  bool LUTables;
  double gMax;
  double Vrev;
  int mExpo;
  int hExpo;
  int maFunc;
  double mka;
  double mVa;
  double msa;
  int mbFunc;
  double mkb;
  double mVb;
  double msb;
  int haFunc;
  double hka;
  double hVa;
  double hsa;
  int hbFunc;
  double hkb;
  double hVb;
  double hsb;
  std::vector<CurrentAssignment> assign;
  int legacy_V = -1;
  int legacy_I = -1;
} abHHData;

struct elecCalibParams {
  // channel copy parameters
  bool copyChnOn;
  ChannelIndex copyChn;

  // General params
  double samplingRate;
  ChannelIndex outputChannelNumber;

  // Electrode measurement params
  double iMaxElec;
  double iMinElec;
  int numberOfLevels;
  double injLenPerLevel;

  // Membrane measurement params
  double iMembStep;
  int numberOfRepeats;
  double injLenPerRepeat;

  // Calibration params
  double hyperpolCurr;
  double injCalAmp;
  double injCalLen;
  double fullKernelLen;
  double electrodeKernelLen;

  int legacy_in = -1;
  int legacy_out = -1;
};

typedef struct {
  bool enabled;
  QuotedString fileName;
  double savingFreq;
  bool isBinary;

} dataSavingParams;
 
struct inChnData {
  bool active;
  int gain;
  double gainFac = 0.1;
  bool spkDetect;
  double spkDetectThresh = -0.01;
  double bias;
  double minVoltage;
  double maxVoltage;
  bool chnlSaving;
  elecCalibParams calib;
};

struct outChnData {
  bool active;
  int gain;
  double gainFac = 0.1;
  double bias;
  double minCurrent;
  double maxCurrent;
  bool chnlSaving;
};

class DAQData {
public:
    bool active;
    std::vector<inChnData> inChn;
    std::vector<outChnData> outChn;
    DAQData() : active(false) {}
};

struct GraphData {
    bool active;
    ChannelIndex chan;
    bool isVoltage;
    QColor color;

    GraphData() : active(true) {}
};

struct PlotData {
    double interval;
    int bufferExp;
    std::vector<GraphData> graphs;
    PlotData() : interval(1e-3), bufferExp(4) {}
};

struct PerformanceMonitorData {
    bool active;
    double interval;
    PerformanceMonitorData() : active(false), interval(1) {}
};

typedef struct {
  bool active;
  double threshV;
  ChannelIndex trigChn;
} SampleHoldData;

struct vInstData {
    bool active;
    inChnData inChn;
    outChnData outChn;
};

struct ModelData {
    bool active;
    virtual vInstData &instance(size_t i) = 0;
    virtual size_t numInst() const = 0;
    ModelData() : active(false) {}
};

#endif
