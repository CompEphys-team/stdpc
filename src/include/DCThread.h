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
#include "DataSaver.h"

#include "ChannelBufferHelper.h"

class GraphDlg;

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

     void setGraph(GraphDlg * = nullptr, double dt = 0.0);

     void setup_and_go();

     inChannel *getInChan(ChannelIndex const& dex);
     outChannel *getOutChan(ChannelIndex const& dex);
     std::vector<ChannelIndex> getChanIndices(ChannelIndex const& dex);

     template <typename T>
     void instantiate(std::vector<T> &pre, std::vector<T> &in, typename T::param_type &, CurrentAssignment &);
     template <typename T>
     void instantiate(std::vector<T> &pre, std::vector<T> &post, typename T::param_type &, SynapseAssignment &);
     template <typename T>
     void instantiate(std::vector<T> &, typename T::param_type &, GapJunctionAssignment &);

     // Temporary data storing object
     QVector<double> data;
     QVector<inChannel *> inChnsToSave;
     QVector<outChannel *> outChnsToSave;
     
     bool stopped;
     bool finished;
     bool scripting;
     std::vector<ChemSyn> csynPre;
     std::vector<ChemSyn> csynPost;
     std::vector<abSyn> absynPre;
     std::vector<abSyn> absynPost;

     std::vector<GapJunction> esyn;

     std::vector<DestexheSyn> dsynPre;
     std::vector<DestexheSyn> dsynPost;

     std::vector<HH> hhPre;
     std::vector<HH> hhIn;
     std::vector<abHH> abhhPre;
     std::vector<abHH> abhhIn;

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

     GraphDlg *graph;
     QVector<double *> graphVar;
     double graphDt;
     double graphDummy;

     QList<QPair<double, std::function<void()>>> scriptq;
     QList<QPair<double, std::function<void()>>>::iterator scrIter;
    
 signals:
     void error(QString message);
     void message(QString message);
     void CloseToLimit(QString, QString, double, double, double);
     void updateRate(int Hz);
     void done();
};


#endif
