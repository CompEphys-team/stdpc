#ifndef DCTHREAD_H
#define DCTHREAD_H

#include <QThread>
#include <functional>
#include "Global.h"
#include "ChemSyn.h"
#include "AbSyn.h"
#include "GapJunction.h"
#include "DestexheSyn.h"
#include "HH.h"
#include "AbHH.h"
#include "SpkGen.h"
#include "DigiData.h"
#include "AECChannel.h"
#include "DataSaver.h"


class DCThread : public QThread 
{
     Q_OBJECT

 protected:
     void run();
     
 public slots:

private:
     DataSaver *dataSaver;

 public:
     DCThread();
     virtual ~DCThread();
     void init(DAQ *);
     bool LoadScript(QString &);
     void UnloadScript();
     double WaitTillNextSampling(double);

     inChannel *getInChan(int idx);
     outChannel *getOutChan(int idx);

     // List of AEC channels (io-channels pairs, kernels and current buffers)
     QVector<AECChannel*> aecChannels;

     // Temporary data storing object
     QVector<double> data;
     QVector<int> inChnsToSave;
     QVector<int> outChnsToSave;
     
     bool stopped;
     bool finished;
     bool scripting;
     SpkGen SG;
     ChemSyn *csyn;
     abSyn *absyn;
     GapJunction *esyn;
     DestexheSyn *dsyn;
     std::vector<HH> hh;
     abHH *abhh;
     DAQ *board;
     inChannel *inChn;
     outChannel *outChn;
     short int *inIdx;
     short int *outIdx;
     short int inNo;
     short int outNo;
     
     short int csNo;
     short int absNo;
     short int esNo;
     short int dsNo;
     short int abhhNo;
     
     short int *csIdx;
     short int *absIdx;
     short int *esIdx;
     short int *dsIdx;
     short int *abhhIdx;
     
 private:
     bool initial;   
     double t;
     double lastT;
     double dt;
     double lastWrite[2];
    
     int grpNo[2];
     int pen[2][4];
     double *grp[2][4];


     QList<QPair<double, std::function<void()>>> scriptq;
     QList<QPair<double, std::function<void()>>>::iterator scrIter;
    
 signals:
     void error(QString message);
     void message(QString message);
     void addPoint1(double, double, int);
     void addPoint2(double, double, int);
     void CloseToLimit(QString, int, double, double, double);


};


#endif
