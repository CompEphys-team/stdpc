#include "main.h"
#include "global.h"

int synType[MAX_SYN_NO];
CSynData CSynp[MAX_SYN_NO];
abSynData abSynp[MAX_SYN_NO];
GJunctData ESynp[MAX_SYN_NO];

int HHType[MAX_HH_NO];
mhHHData mhHHp[MAX_HH_NO];
abHHData abHHp[MAX_HH_NO];

inChnData *inChnp;
outChnData *outChnp;
inChnData inSpkGenChnp;
outChnData outSpkGenChnp;

DigiDataData DigiDatap;
SDAQData SDAQp;
#ifdef NIDAQ
NIDAQData NIDAQp;
#endif
SGData SGp;

graphData Graphp[2];

// LU tables
tanhLUtable tanhLU;
expLUtable expLU;
expSigmoidLUtable expSigmoidLU;

// adjustable parameters

QVector<double *> dAP;
QVector<int *> iAP;
QVector<bool *> bAP;
QVector<QString *> sAP;
QVector<short int *> siAP;
QVector<QString> APname;
QVector<int> APindex;
QVector<APTYPE> APtype;


