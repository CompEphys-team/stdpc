#ifndef SPKGEN_H
#define SPKGEN_H

using namespace std;

#include <fstream>
#include "Global_func.h"
#include "ObjectDataTypes.h"
#include "Channels.h"
#include <QObject>
#include <QList>

class SpkGen: public QObject 
{
      Q_OBJECT
      
  private:
    SGData *p;
    stdpc::function *theExp;
    stdpc::function *theTanh;
    inChannel *VChn;
    inChannel *bdChn;
    bool SGactive;
    bool burstDetected;
    int nOverThresh;
    int nUnderThresh;
    int readSpikeNo;
    int burstSpikeNo;
    double ISI_time;
    
    QList<double> tq;
    QList<double>::iterator titer;
    QList<double> Vq;
    QList<double>::iterator Viter;
    QList<double> stq;
    QList<int> snq;
    QList<double>::iterator stIter;
    QList<int>::iterator snIter;
    ifstream is;
    bool initial;
    QString lastRead;
    
  protected:
    
  public:
    SpkGen();
    virtual ~SpkGen();
    void init(SGData *, inChannel *VChn, DCThread *);
    void VUpdate(double, double);
    double VSpike(double);

    double V;
    
  signals:
    void message(QString message);
};

#endif
