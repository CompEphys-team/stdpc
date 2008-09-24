#ifndef DCTHREAD_H
#define DCTHREAD_H

#include <QThread>
#include "global.h"
#include "ChemSyn.h"
#include "GapJunction.h"
#include "HH.h"
#include "abHH.h"
#include "SpkGen.h"
#include "DigiData.h"
#include "scripting.h"


class DCThread : public QThread 
{
     Q_OBJECT

 protected:
     void run();
     
 public slots:
 
 public:
     DCThread();
     virtual ~DCThread();
     void init(DAQ *);
     bool LoadScript(QString &);
     void UnloadScript();
     
     bool stopped;
     bool finished;
     bool scripting;
     SpkGen SG;
     ChemSyn *csyn;
     GapJunction *esyn;
     HH *hh;
     abHH *abhh;
     DAQ *board;
     inChannel *inChn;
     outChannel *outChn;
     short int *inIdx;
     short int *outIdx;
     short int inNo;
     short int outNo;
     
     short int csNo;
     short int esNo;
     short int hhNo;
     short int abhhNo;
     
     short int *csIdx;
     short int *esIdx;
     short int *hhIdx;
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

     QList<scriptInstruction> scriptq;
     QList<scriptInstruction>::iterator scrIter;
    
 signals:
     void error(QString message);
     void message(QString message);
     void addPoint1(double, double, int);
     void addPoint2(double, double, int);
     
};


#endif
