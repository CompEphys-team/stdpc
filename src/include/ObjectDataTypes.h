#ifndef OBJECTDATATYPES_H
#define OBJECTDATATYPES_H

using namespace std;

#include <QString>
#include <iostream>
#include <QColor>
#include "ChannelIndex.h"
#include "Util.h"

struct SynapseAssignment {
    bool active;
    ChannelIndex PreSynChannel;
    ChannelIndex PostSynChannel;
    ChannelIndex OutSynChannel;
    double delay;
};

struct GapJunctionAssignment {
    bool active;
    bool *actP;
    ChannelIndex preInChannel;
    ChannelIndex postInChannel;
    ChannelIndex preOutChannel;
    ChannelIndex postOutChannel;
};

struct CurrentAssignment {
    bool active;
    ChannelIndex VChannel;
    ChannelIndex IChannel;
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
  bool MgBlock;
  double gSyn;
  double VSyn;
  double tauSyn;
  double VThresh;
  double VSlope;
  int STD;
  double STDAmpl;
  double STDVThresh;
  double STDVSlope;
  double STDtauAmpl;
  double STDtau0;
  double STDtauVThresh;
  double STDtauVSlope;
  int fixVpost;
  double Vpost;
  double Mgfac;
  double Mgexpo;
  int Plasticity;
  STPlast ST;
  ODEPlast ODE;
  std::vector<SynapseAssignment> assign;
} CSynData;

typedef struct {
  bool active;
  bool LUTables;
  double gSyn;
  double Vrev;
  double aS;
  double bS;
  double aR;
  double VaR;
  double saR;
  double bR;
  int fixVpost;
  double Vpost;
  int Plasticity;
  STPlast ST;
  ODEPlast ODE;
  std::vector<SynapseAssignment> assign;
} abSynData;

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
} DestexheSynData;

typedef struct {
  bool active;
  int type;
  double gSyn;
  std::vector<GapJunctionAssignment> assign;
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
  double gainFac;
  bool spkDetect;
  double spkDetectThresh;
  double bias;
  double minVoltage;
  double maxVoltage;
  bool chnlSaving;
  elecCalibParams calib;
};

struct outChnData {
  bool active;
  int gain;
  double gainFac;
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
