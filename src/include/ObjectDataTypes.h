#ifndef OBJECTDATATYPES_H
#define OBJECTDATATYPES_H

using namespace std;

#include "Main.h"
#include <QString>
#include <iostream>

// Forward
class DCThread;

struct SynapseAssignment {
    bool active;
    int PreSynChannel;
    int PostSynChannel;
    int OutSynChannel;
};

struct GapJunctionAssignment {
    bool active;
    int preInChannel;
    int postInChannel;
    int preOutChannel;
    int postOutChannel;
};

struct CurrentAssignment {
    bool active;
    int VChannel;
    int IChannel;
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

typedef struct {

  // channel copy parameters
  bool copyChnOn;
  int copyChn;
  // General params
  double samplingRate;
  int inputChannelNumber;
  int outputChannelNumber;

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

} elecCalibParams;

typedef struct {
  bool enabled;
  QString fileName;
  double savingFreq;
  bool isBinary;

} dataSavingParams;

struct SGData {
  bool active;
  int method;
  bool saving;
  int LUTables;
  double VSpike;
  double spkTimeScaling;
  double VRest;
  int bdType;
  int bdChannel;
  double bdThresh;
  int bdNUnder;
  int bdNOver;
  double period;
  int SpikeNo;
  QString STInFName;
  std::vector<double> SpikeT;
public:
  SGData() : SpikeT(10) {}
};
 
typedef struct {
  bool active;
  int gain;
  double gainFac;
  bool spkDetect;
  double spkDetectThresh;
  double bias;
  double minVoltage;
  double maxVoltage;
  bool chnlSaving;
} inChnData;

typedef struct {
  bool active;
  int gain;
  double gainFac;
  double bias;
  double minCurrent;
  double maxCurrent;
  bool chnlSaving;
} outChnData;

class SDAQData {
  public: 
    QString inFileName;
    QString outFileName;
    short int inChnNo;
    short int outChnNo;
    double inTFac;
    double outDt;
    SDAQData &operator=(SDAQData);
};

class DigiDataData {
  public: 
    short int baseAddress;
    short int syncIOMask;
    DigiDataData &operator=(DigiDataData);
};

#ifdef NATIONAL_INSTRUMENTS
class NIDAQData {
  public: 
    QString deviceName;
    NIDAQData &operator=(NIDAQData);
};
#endif

typedef struct {
  bool active[4];
  QString color[4];
  int chn[4];
  double miny[4];
  double maxy[4];
  double xrange;
  double dt;
  int xtNo;
  int ytNo;
  double yfac[4];
} graphData;

typedef struct {
  bool active;
  double threshV;
  int trigChn;
} SampleHoldData;

struct vInstData {
    bool active;
    inChnData inChn;
    outChnData outChn;
};

struct HHNeuronData {
    bool active;
    double C;
    double gLeak;
    double ELeak;
    std::vector<vInstData> inst;
};

ostream &operator<<(ostream &os, SDAQData &p);
istream &operator>>(istream &is, SDAQData &p); 
ostream &operator<<(ostream &os, DigiDataData &p);
istream &operator>>(istream &is, DigiDataData &p);
#ifdef NATIONAL_INSTRUMENTS
ostream &operator<<(ostream &os, NIDAQData &p); 
istream &operator>>(istream &is, NIDAQData &p); 
#endif

#endif
