#ifndef GLOBAL_H
#define GLOBAL_H

#include "Main.h"
#include <QVector>
#include "ObjectDataTypes.h"
#include "LUtables.h"

#define MAX_ST_HISTORY 20
#define MAX_SYN_NO 6
#define MAX_HH_NO 6
#define MAX_IN_CHN_NO 64
#define VTHRESH -30


extern int synType[MAX_SYN_NO];
extern CSynData CSynp[MAX_SYN_NO];
extern abSynData abSynp[MAX_SYN_NO];
extern GJunctData ESynp[MAX_SYN_NO];

extern int HHType[MAX_HH_NO];
extern mhHHData mhHHp[MAX_HH_NO];
extern abHHData abHHp[MAX_HH_NO];

extern inChnData *inChnp;
extern outChnData *outChnp;
extern inChnData inSpkGenChnp;
extern outChnData outSpkGenChnp;

extern DigiDataData DigiDatap;
extern SDAQData SDAQp;
#ifdef NATIONAL_INSTRUMENTS
extern NIDAQData NIDAQp;
#endif
extern SGData SGp;

extern graphData Graphp[2];

// LU tables
extern tanhLUtable tanhLU;
extern expLUtable expLU;
extern expSigmoidLUtable expSigmoidLU;

// adjustable parameters

enum APTYPE {DBLTYPE,INTTYPE,BOOLTYPE,STRTYPE,SHORTINTTYPE};

extern QVector<double *> dAP;
extern QVector<int *> iAP;
extern QVector<bool *> bAP;
extern QVector<QString *> sAP;
extern QVector<short int *> siAP;
extern QVector<QString> APname;
extern QVector<int> APindex;
extern QVector<APTYPE> APtype;


#endif
