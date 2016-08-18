#ifndef GLOBAL_H
#define GLOBAL_H

#include "Main.h"
#include <QVector>
#include <vector>
#include "ObjectDataTypes.h"
#include "LUtables.h"

#define MAX_ST_HISTORY 20
#define MAX_SYN_NO 6
#define MAX_HH_NO 6
#define MAX_IN_CHN_NO 64
#define VTHRESH -30
#define MAX_ELECTRODE_NO 4


extern int synType[MAX_SYN_NO];
extern std::vector<CSynData> CSynp;
extern std::vector<abSynData> abSynp;
extern std::vector<GJunctData> ESynp;
extern std::vector<DestexheSynData> DxheSynp;

extern int HHType[MAX_HH_NO];
extern std::vector<mhHHData> mhHHp;
extern std::vector<abHHData> abHHp;

extern std::vector<inChnData> inChnp;
extern std::vector<outChnData> outChnp;
extern inChnData inSpkGenChnp;
extern outChnData outSpkGenChnp;

extern DigiDataData DigiDatap;
extern SDAQData SDAQp;
#ifdef NATIONAL_INSTRUMENTS
extern NIDAQData NIDAQp;
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


#endif
