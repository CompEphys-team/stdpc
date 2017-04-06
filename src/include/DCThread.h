#ifndef DCTHREAD_H
#define DCTHREAD_H

#include <QThread>
#include <functional>
#include "Global.h"
#include "Channels.h"
#include "DataSaver.h"

#include "ChannelBufferHelper.h"

class GraphWidget;
class PerformanceMonitor;

class DCThread : public QThread 
{
     Q_OBJECT

 protected:
     void run();
     
 public slots:

private:
     DataSaver *dataSaver;

     inline void start(Priority pr = TimeCriticalPriority) { QThread::start(pr); }

 public:
     DCThread();
     virtual ~DCThread();
     bool LoadScript(QString &);
     void UnloadScript();

     void setGraph(GraphWidget * = nullptr, double dt = 0.0);
     void setPerformanceMonitor(PerformanceMonitor * = nullptr, double dt = 0.0);

     void setup_and_go();

     inChannel *getInChan(ChannelIndex const& dex);
     outChannel *getOutChan(ChannelIndex const& dex);
     std::vector<ChannelIndex> getChanIndices(ChannelIndex const& dex);

     QVector<const double *> valuesToSave;
     
     bool stopped;
     bool finished;
     bool scripting;

     outChnData outNoneData;
     outChannel outChnNone;

     std::shared_ptr<ChannelBufferHelper> bufferHelper;

private:
     bool initial;   
     double t;
     double dt;

     bool saving;
     double savingPeriod;
     double lastWrite;

     QVector<AECChannel*> aecChannels;
     QVector<inChannel*> aecIn;
     QVector<outChannel*> aecOut, aecCopy;

     GraphWidget *graph;
     QVector<const double *> graphVar;
     double graphDt;
     double graphDummy;

     PerformanceMonitor *perfmon;
     double perfDt;

     QList<QPair<double, std::function<void()>>> scriptq;
     QList<QPair<double, std::function<void()>>>::iterator scrIter;
    
 signals:
     void error(QString message);
     void message(QString message);
     void CloseToLimit(QString, QString, double, double, double);
     void updateRate(int Hz);
     void done();
     void saveData();
     void forceSaveData();
};


#endif
