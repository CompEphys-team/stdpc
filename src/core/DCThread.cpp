#include "DCThread.h"
#include "AP.h"
#include "Global.h"
#include <cmath>
#include <climits>
#include "ChannelIndex.h"
#include "DeviceManager.h"
#include "ModelManager.h"
#include "GraphWidget.h"
#include "PerformanceMonitor.h"

DCThread::DCThread() :
    graph(nullptr),
    perfmon(nullptr)
{
  stopped= true;
  finished= true;

  scripting= false;

  dataSaver = new DataSaver();
  connect(this, SIGNAL(saveData()), dataSaver, SLOT(SaveLine()), Qt::QueuedConnection);
  connect(this, SIGNAL(done()), dataSaver, SLOT(EndDataSaving()), Qt::QueuedConnection);
  connect(this, SIGNAL(forceSaveData()), dataSaver, SLOT(SaveLine()), Qt::BlockingQueuedConnection);

  outNoneData.active = true;
  outNoneData.gain = 0;
  outNoneData.gainFac = 1.0;
  outNoneData.bias = 0;
  outNoneData.chnlSaving = false;
  outChnNone.init(&outNoneData);
}

DCThread::~DCThread()
{
}

void DCThread::setGraph(GraphWidget *g, double dt)
{
    if ( stopped ) {
        graph = g;
        graphDt = dt;
    }
}

void DCThread::setPerformanceMonitor(PerformanceMonitor *pt, double dt)
{
    if ( stopped ) {
        perfmon = pt;
        perfDt = dt;
    }
}

void DCThread::setup_and_go()
{
   // Initialise devices
   for ( auto &b : Devices.active() )
       b->init_chans();

   // Populate Models in absence of UI doing it
   for ( ModelProxy *proxy : ModelManager::Register() )
       for ( size_t j = 0; j < proxy->size(); j++ )
           Models.initSingle(proxy, j);
   Models.initActive(this);

   // Prepare the buffer helper for delayed synapses
   bufferHelper.reset(new ChannelBufferHelper);

   // Populate synapses and currents
   Conductances.init(this);

   // set up the graphic display channels
   graphDummy = 0.0;
   if ( graph ) {
       inChannel *itmp;
       outChannel *otmp;
       graphVar.clear();
       graphVar.reserve(graph->q.size());
       for ( ChannelIndex &chan : graph->plottedChannels ) {
           if ( !chan.isValid || chan.isNone ) {
               graphVar.push_back(&graphDummy);
           } else if ( chan.isConductance ) {
               const double *ret = Conductances.conductance(chan);
               graphVar.push_back(ret ? ret : &graphDummy);
           } else if ( chan.isVirtual ) {
               graphVar.push_back(chan.isInChn
                       ? &((itmp = getInChan(chan)) ? itmp->V : graphDummy)
                       : &((otmp = getOutChan(chan)) ? otmp->I : graphDummy));
           } else if ( chan.isInChn ) {
               graphVar.push_back(&((itmp = getInChan(chan)) ? itmp->V : graphDummy));
           } else {
               graphVar.push_back(&((otmp = getOutChan(chan)) ? otmp->I : graphDummy));
           }
       }
       message(QString("Added %1 channels for display").arg(graphVar.size()));
   }

   for ( const QString &status : Conductances.getStatus() )
       message(status);
   for ( auto const& m : Models.active() )
       message(m->getStatus());

   // Generate lookup tables
   int sz;
   sz= tanhLU.generate();
   if (sz > 0) message(QString("Tanh lookup table (re)generated"));
   sz= expLU.generate();
   if (sz > 0) message(QString("Exp lookup table (re)generated"));
   sz= expSigmoidLU.generate();
   if (sz > 0) message(QString("Exponential sigmoid lookup table (re)generated"));

   // Set up AEC channels
   aecChannels.clear();
   aecIn.clear();
   aecOut.clear();
   aecCopy.clear();
   for ( auto &b : Devices.active() ) {
       for ( AECChannel *aec : b->aecChans() ) {
           DAQ *outB = Devices.getDevice(aec->outChnNum);
           outChannel *outC = getOutChan(aec->outChnNum);
           if ( !outB || !outB->initialized || !outC || !outC->active ) {
               message(QString("AEC output channel/device %1 is not active! AEC disabled on input channel %2")
                       .arg(aec->outChnNum.prettyName(), aec->inChnNum.prettyName()));
           } else {
               message(QString("AEC channel is active on input channel %1 and output channel %2")
                       .arg(aec->inChnNum.prettyName(), aec->outChnNum.prettyName()));
               aecChannels.push_back(aec);
               aecIn.push_back(getInChan(aec->inChnNum));
               aecOut.push_back(getOutChan(aec->outChnNum));
               if ( aecIn.back()->p->calib.copyChnOn )
                   aecCopy.push_back(getOutChan(aecIn.back()->p->calib.copyChn));
               else
                   aecCopy.push_back(nullptr);
           }
       }
   }

   // Init data saving
   saving = false;
   if ( dataSavingPs.enabled ) {
       saving = dataSaver->InitDataSaving(dataSavingPs.fileName, dataSavingPs.isBinary);
       if ( !saving )
           dataSaver->EndDataSaving();
   }
   if (saving) {
       savingPeriod = 1.0 / dataSavingPs.savingFreq;

       valuesToSave.clear();
       QVector<QString> header(1, "Time");

       for ( auto &b : Devices.active() ) {
           auto toSave = b->valuesToSave();
           header += toSave.first;
           valuesToSave += toSave.second;
       }
       for ( auto const& m : Models.active() ) {
           auto toSave = m->valuesToSave();
           header += toSave.first;
           valuesToSave += toSave.second;
       }
       {
           auto toSave = Conductances.toSave();
           header += toSave.first;
           valuesToSave += toSave.second;
       }

#ifdef TEST_VERSION
       for ( AECChannel *aec : aecChannels ) {
           header += QString("VAEC_%1").arg(aec->inChnNum.toString('_'));
           valuesToSave += &(aec->v_e);
       }
#endif

       if ( valuesToSave.empty() )
           saving = false;
       else
           dataSaver->SaveHeader(header, dataSavingPs.savingFreq);
   }

   start();
}

void DCThread::run()
{
   int rateCounter = 0;
   double lastRateReport = 0.;
   PerformanceMonitor::DataPoint perfData { 0, 0, perfDt, 0 };
   static int i;
   static double evt;
   bool SampleHoldOn= false;
   inChannel *SampleHoldChan;
   DAQ *SampleHoldBoard;

   // RK4 step size adaptation variables:
   constexpr int tDepth = 100;
   int tDepthUsed = -1;
   double tFull[tDepth] = {}, tComp[tDepth] = {};
   double sumTFull = 0., sumTComp = 0.;
   int tFullOff = 0, tCompOff = 0;
   double meanTComp, meanDT;
   double subDT, subT, tLoop;
   int nSubsteps;

   SampleHoldChan = getInChan(SampleHoldp.trigChn);
   SampleHoldBoard = Devices.getDevice(SampleHoldp.trigChn);

   bool limitWarningEmitted = false;
   bool processAnalogs = false;

   stopped= false;
   finished= false;

   DAQ *triggerDev;
   if ( Triggerp.active && (triggerDev = Devices.getDevice(Triggerp.channel)) ) {
       message(QString("DynClamp: Waiting for trigger on %1...").arg(Triggerp.channel.prettyName()));
       triggerDev->armTrigger(Triggerp.channel);
       while ( !stopped && !triggerDev->triggerFired() ) {
           // Wait patiently.
       }
       if ( !stopped )
           message(QString("DynClamp: Triggered."));
   }
   message(QString("DynClamp: Clamping ..."));

   t= 0.0;
   double lastSave = t;
   lastWrite = t;
   // init scripting
   if (scripting) {
     scrIter= scriptq.begin();
     evt= scrIter->first;
   }
   else evt= 1e10;

   for ( auto &b : Devices.active() )
       b->start();

   // Reset clock AFTER the time-consuming device start to get a more precise t0
   DAQClock.reset_RTC();

   // Dynamic clamp loop begins
   while (!stopped) {


     if (!SampleHoldOn) {
       // --- Read --- //
         for ( auto &b : Devices.active() )
             b->get_scan();
       // --- Read end --- //

       // --- Calculate --- //

        // Adaptive time step
        dt = DAQClock.get_RTC();
        t += dt;

        // AEC channel update part
        // AEC compensation
        for ( int k=0; k<aecChannels.size(); k++ ) {
            aecChannels[k]->CalculateVe(aecOut[k]->I, dt);
            aecIn[k]->V -= aecChannels[k]->v_e;
        }

        // Apply biases & detect spikes
        processAnalogs = true;
     }
     else {
         // Adaptive time step
         dt = DAQClock.get_RTC();
         t += dt;
     }
     if (SampleHoldp.active) {
         if (SampleHoldOn) {
             SampleHoldBoard->get_single_scan(SampleHoldChan);
             if (SampleHoldChan->V < SampleHoldp.threshV) {
                 SampleHoldOn= false;
             }
         }
         else {
             if (SampleHoldChan->V >= SampleHoldp.threshV) {
                 SampleHoldOn= true;
             }
         }
     }

         bufferHelper->advance(t);

         if ( processAnalogs ) {
             for ( auto &b : Devices.active() )
                 b->process_scan(t);
             processAnalogs = false;
         }
         for ( auto const& m : Models.active() )
             m->updateChannels(t);

         // Dynamic clamp: a2a/mixed currents, a2a/a2d/d2d synapses, all gap junctions
         for ( Conductance *c : Conductances.preDigital() )
             c->step(t, dt);

         // Dynamic clamp: models (d2d currents)
         if ( Models.active().size() ) { // Runge-Kutta 4
             for ( auto const& m : Models.active() )
                 m->retainCurrent(t);

             if ( tDepthUsed < 100 ) {
                 if ( ++tDepthUsed ) {
                     meanTComp = sumTComp/tDepthUsed;
                     meanDT = sumTFull/tDepthUsed;
                }
             } else {
                 meanTComp = sumTComp/tDepth;
                 meanDT = sumTFull/tDepth;
             }
             if ( tDepthUsed > 10 && meanTComp / dt < 0.5 * meanTComp / meanDT ) { // dt is unusually large
                 // Compute in substeps such that the entire computation takes roughly its usual fraction of the cycle time
                 nSubsteps = (int)(dt/meanDT);
                 subDT = dt/nSubsteps;
             } else {
                 nSubsteps = 1;
                 subDT = dt;
             }

             for ( i = 0, subT = t; i < nSubsteps; i++, subT+=subDT ) {
                 tLoop = DAQClock.get_RTC_const();

                 for ( auto const& m : Models.active() )
                     m->restoreCurrent(subT);

                 for ( size_t rkStep = 0; rkStep < 4; rkStep++ ) {
                     double rkDT = rkStep < 2 ? subDT/2 : subDT;

                     for ( Conductance *c : Conductances.inDigital() )
                         c->RK4(subT, rkDT, rkStep);

                     for ( auto const& m : Models.active() )
                         m->RK4(subT, rkDT, rkStep);
                 }

                 tLoop = DAQClock.get_RTC_const() - tLoop;

                 tComp[tCompOff] = tLoop;
                 tCompOff = (tCompOff+1) % tDepth;
                 sumTComp = sumTComp - tComp[tCompOff] + tLoop;
             }

             tFull[tFullOff] = dt;
             tFullOff = (tFullOff+1) % tDepth;
             sumTFull = sumTFull - tFull[tFullOff] + dt;
         } // end RK4

         // Dynamic clamp: d2a synapses
         for ( Conductance *c : Conductances.postDigital() )
             c->step(t, dt);

         // copy AEC compensated input values to output channels if desired
         for ( int k=0; k<aecChannels.size(); k++ ){
             if ( aecCopy[k])
                 aecCopy[k]->I= aecIn[k]->V;
         }

         //--------------- Data saving ------------------------//
         if ( saving && dataSavingPs.enabled ) {
           if ( t >= lastSave + savingPeriod )
           {
              i = 0;
              bool space = dataSaver->q[i++]->push(t);
              if ( !space ) { // Queues full
                  forceSaveData(); // BlockingQueuedConnection ensures completion of
                                   // DataSaver::SaveLine before this call returns
                  dataSaver->q[i-1]->push(t);
              }
              for ( const double *value : valuesToSave )
                  dataSaver->q[i++]->push(*value);
              if ( space )
                  emit saveData();
              lastSave = t;
           }
         }
         //--------------- Data saving end ---------------------//

         // Rate update
         ++rateCounter;
         if ( t - lastRateReport > 1 ) {
             lastRateReport = t;
             emit updateRate(rateCounter);
             rateCounter = 0;
         }

         // Performance monitor
         if ( perfmon ) {
             ++perfData.n;
             if ( dt < perfData.minDt )
                 perfData.minDt = dt;
             if ( dt > perfData.maxDt )
                 perfData.maxDt = dt;
             if ( t - perfData.t > perfDt ) {
                 perfData.t = t;
                 perfmon->q.push(perfData);
                 // Set min, max to mean rather than boundary to reduce initial flurry of updates
                 perfData.minDt = perfData.maxDt = perfDt / perfData.n;
                 perfData.n = 0;
             }
         }

         // Scripting
         if (t >= evt) { // event needs doing
           scrIter->second();
           scrIter++;
           if (scrIter == scriptq.end()) evt= 1e10;
           else {
             evt= scrIter->first;
           }
         }

         // Check channel limits
         DAQ::ChannelLimitWarning w;
         for ( auto &b : Devices.active() ) {
             if ( b->check_limits(!limitWarningEmitted, w) ) {
                 emit CloseToLimit(w.what, w.chan_label, w.loLim, w.hiLim, w.value);
                 limitWarningEmitted = true;
             }
         }

         // Updated display
         if ( graph ) {
             if ( t - lastWrite > graphDt ) {
                 lastWrite = t;
                 i = 0;
                 for ( const double *val : graphVar )
                    graph->q[i++]->push(GraphWidget::DataPoint {t, *val});
             }
         }

     // --- Calculate end --- //

     // --- Write --- //
     for ( auto &b : Devices.active() )
         b->write_analog_out();
     // --- Write end --- //


   }
   // Dynamic clamp loop ends


   // In the end, let's do the cleanup
   for ( auto &b : Devices.active() ) {
       b->reset_board();
       b->reset_chans();
   }
   for ( int i=0; i<aecChannels.size(); i++ ) if ( aecChannels[i]->IsActive() ) aecChannels[i]->ResetChannel();
   for ( auto const& m : Models.active() )
       m->reset();

   if ( saving ) {
       dataSaver->EndDataSaving();
   }

   finished= true;
   emit done();
}


inChannel *DCThread::getInChan(ChannelIndex const& dex)
{
    if ( !dex.isValid || dex.isPrototype ) {
        return nullptr;
    } else if ( dex.isVirtual ) {
        return Models.getInChan(dex);
    } else if ( dex.isAnalog && dex.isInChn ) {
        return Devices.getInChan(dex);
    }
    return nullptr;
}

outChannel *DCThread::getOutChan(ChannelIndex const& dex)
{
    if ( !dex.isValid || dex.isPrototype ) {
        return nullptr;
    } else if ( dex.isVirtual ) {
        return Models.getOutChan(dex);
    } else if ( dex.isNone ) {
        return &outChnNone;
    } else if ( dex.isAnalog && !dex.isInChn ) {
        return Devices.getOutChan(dex);
    }
    return nullptr;
}

std::vector<ChannelIndex> DCThread::getChanIndices(ChannelIndex const& dex)
{
    std::vector<ChannelIndex> ret;
    if ( !dex.isValid ) {
        return ret;
    } else if ( dex.isPrototype ) {
        if ( Models.all().value(dex.modelClass).size() > (int)dex.modelID )
            for ( size_t i = 0, end = Models.all().value(dex.modelClass)[dex.modelID]->params().numInst(); i < end; i++ )
                ret.push_back(dex.toInstance(i));
    } else {
        ret.push_back(dex);
    }
    return ret;
}

// Scripting support

bool DCThread::LoadScript(QString &fname)
{
  double et;
  QString rawname;
  bool done, success;
  QString qn;
  char buf[80];

  std::vector<std::unique_ptr<AP>> deprecChannels;

  ifstream is(fname.toLatin1());

  is >> et;
  done= false;
  success= true;
  while (!done)
  {
    is >> buf;
    rawname= QString(buf);
    AP *it = AP::find(rawname);
    if ( !it && (rawname.startsWith("inChnp") || rawname.startsWith("outChnp")) ) {
        if ( deprecChannels.empty() && !Devices.active().empty() ) {
            // Get the first active device
            DAQ *dev = Devices.active().first().get();
            // Take its first coreAP's name, use the first part of that, e.g. "NIDAQp[#]"
            QString prefix = dev->proxy->coreAPs().at(0)->name().split('.').at(0);
            deprecChannels = deprecateChannelsTo(prefix);
            message(QString("Note: Deprecated channels assigned to %1 %2")
                    .arg(dev->proxy->prettyName())
                    .arg(dev->devID));
            message(QString("Update script from e.g. \"inChnp[0].bias\" to \"%1.inChn[0].bias\" to prevent confusion.")
                    .arg(prefix.replace("[#]", "[%2]"))
                    .arg(dev->devID));
        }
        it = AP::find(rawname, &deprecChannels);
        if ( it )
            message(QString("Note: %1 (%2) -> %3").arg(rawname, it->name(), it->canonicalName()));
    }
    if ( it ) {
        scriptq.append(qMakePair(et, it->readLater(rawname, is, &success)));
    } else {
        success = false;
    }
    is >> et;
    if ((!is.good()) || (!success)) done= true;
  }
  if (!success) {
    scriptq.clear();
    scripting= false;
    qn.setNum(et);
    message(QString("error in script file at instruction \"")+qn+QString(" ")+rawname+QString("\""));
    message(QString("script not loaded"));
  }
  else {
    qn.setNum(scriptq.size());
    message(QString("script read with ")+qn+QString(" instructions"));
    scripting= true;
  }
  return success;
}

void DCThread::UnloadScript()
{
  scriptq.clear();
  scripting= false;
  message(QString("script unloaded"));
}


