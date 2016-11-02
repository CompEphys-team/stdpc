#include "DCThread.h"
#include "AP.h"
#include "Global.h"
#include <cmath>
#include <climits>
#include "ChannelIndex.h"
#include "DeviceManager.h"
#include "ModelManager.h"
#include "GraphDlg.h"

DCThread::DCThread() :
    graph(nullptr)
{
  stopped= true;
  finished= true;

  scripting= false;

  dataSaver = new DataSaver();

  outChnData outNoneData;
  outNoneData.active = true;
  outNoneData.gain = 0;
  outNoneData.gainFac = 1.0;
  outChnNone.init(&outNoneData);
}

DCThread::~DCThread()
{
}

void DCThread::setGraph(GraphDlg *g, double dt)
{
    if ( stopped ) {
        graph = g;
        graphDt = dt;
    }
}

void DCThread::run()
{
   double savingPeriod= 1.0;
   int rateCounter = 0;
   double lastRateReport = 0.;
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

   for ( auto &b : Devices.active() )
       b->init_chans();

   // Populate Models in absence of UI doing it
   for ( ModelProxy *proxy : ModelManager::Register() )
       for ( size_t j = 0; j < proxy->size(); j++ )
           Models.initSingle(proxy, j);
   Models.initActive(this);

   SampleHoldChan = getInChan(SampleHoldp.trigChn);
   SampleHoldBoard = Devices.getDevice(SampleHoldp.trigChn);

   // set up the graphic display channels
   double graphDummy = 0.0;
   if ( graph ) {
       inChannel *itmp;
       outChannel *otmp;
       i = 0;
       graphVar.resize(Graphp.size());
       for ( GraphData &p : Graphp ) {
           if ( !p.active || !p.chan.isValid || p.chan.isNone ) {
               graphVar[i] = &graphDummy;
           } else if ( p.chan.isVirtual ) {
               graphVar[i] = p.isVoltage
                       ? &((itmp = getInChan(p.chan)) ? itmp->V : graphDummy)
                       : &((otmp = getOutChan(p.chan)) ? otmp->I : graphDummy);
           } else if ( p.chan.isInChn ) {
               graphVar[i] =& ((itmp = getInChan(p.chan)) ? itmp->V : graphDummy);
           } else {
               graphVar[i] =& ((otmp = getOutChan(p.chan)) ? otmp->I : graphDummy);
           }
           ++i;
       }
       message(QString("Added %1 channels for display").arg(i));
   }

   bufferHelper.reset(new ChannelBufferHelper);

   csynPre.clear();
   csynPost.clear();
   for ( CSynData &p : CSynp ) {
       if ( p.active ) {
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(csynPre, csynPost, p, a);
       }
   }
   absynPre.clear();
   absynPost.clear();
   for ( abSynData &p : abSynp ) {
       if ( p.active ) {
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(absynPre, absynPost, p, a);
       }
   }
   dsynPre.clear();
   dsynPost.clear();
   for ( DestexheSynData &p : DxheSynp ) {
       if ( p.active ) {
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(dsynPre, dsynPost, p, a);
       }
   }
   esyn.clear();
   for ( GJunctData &p : ESynp ) {
       if ( p.active ) {
           for ( GapJunctionAssignment &a : p.assign )
               if ( a.active )
                   instantiate(esyn, p, a);
       }
   }
   hhPre.clear();
   hhIn.clear();
   for ( mhHHData &p : mhHHp ) {
       if ( p.active ) {
           for ( CurrentAssignment &a : p.assign ) {
               if ( a.active )
                   instantiate(hhPre, hhIn, p, a);
           }
       }
   }
   abhhPre.clear();
   abhhIn.clear();
   for ( abHHData &p : abHHp ) {
       if ( p.active ) {
           for ( CurrentAssignment &a : p.assign ) {
               if ( a.active )
                   instantiate(abhhPre, abhhIn, p, a);
           }
       }
   }
   if (csynPre.size() > 0) message(QString("DynClamp: %1 chemical synapse(s) (a2a/a2d/d2d) ").arg(csynPre.size()));
   if (csynPost.size() > 0) message(QString("DynClamp: %1 chemical synapse(s) (d2a) ").arg(csynPost.size()));
   if (absynPre.size() > 0) message(QString("DynClamp: %1 chemical synapse(s) (a2a/a2d/d2d) ").arg(absynPre.size()));
   if (absynPost.size() > 0) message(QString("DynClamp: %1 chemical synapse(s) (d2a) ").arg(absynPost.size()));
   if (esyn.size() > 0) message(QString("DynClamp: %1 gap junction(s) ").arg(esyn.size()));
   if (dsynPre.size() > 0) message(QString("DynClamp: %1 chemical synapse(s) (a2a/a2d/d2d) ").arg(dsynPre.size()));
   if (dsynPost.size() > 0) message(QString("DynClamp: %1 chemical synapse(s) (d2a) ").arg(dsynPost.size()));
   if (hhPre.size() > 0) message(QString("DynClamp: %1 HH conductance(s) (a) ").arg(hhPre.size()));
   if (hhIn.size() > 0) message(QString("DynClamp: %1 HH conductance(s) (d) ").arg(hhIn.size()));
   if (abhhPre.size() > 0) message(QString("DynClamp: %1 HH conductance(s) (a) ").arg(abhhPre.size()));
   if (abhhIn.size() > 0) message(QString("DynClamp: %1 HH conductance(s) (d) ").arg(abhhIn.size()));
   for ( auto const& m : Models.active() )
       message(m->getStatus());

   int sz;
   sz= tanhLU.generate();
   if (sz > 0) message(QString("Tanh lookup table (re)generated"));
   sz= expLU.generate();
   if (sz > 0) message(QString("Exp lookup table (re)generated"));
   sz= expSigmoidLU.generate();
   if (sz > 0) message(QString("Exponential sigmoid lookup table (re)generated"));

   // Checking validity of AEC channels
   QVector<AECChannel*> aecChannels;
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
           }
       }
   }
   QVector<inChannel*> aecIn(aecChannels.size(), nullptr);
   QVector<outChannel*> aecOut(aecChannels.size(), nullptr);
   QVector<outChannel*> aecCopy(aecChannels.size(), nullptr);
   for ( AECChannel *aec : aecChannels ) {
       aecIn[i] = getInChan(aec->inChnNum);
       aecOut[i] = getOutChan(aec->outChnNum);
       if ( aecIn[i]->p->calib.copyChnOn )
          aecCopy[i] = getOutChan(aecIn[i]->p->calib.copyChn);
   }

   // Init data saving
   bool saving = false;
   if ( dataSavingPs.enabled ) {
       saving = dataSaver->InitDataSaving(dataSavingPs.fileName, dataSavingPs.isBinary);
       if ( !saving )
           dataSaver->EndDataSaving();
   }
   if (saving) {
       savingPeriod = 1.0 / dataSavingPs.savingFreq;

       inChnsToSave.clear();
       outChnsToSave.clear();
       QVector<QString> headerIn, headerOut;

       for ( auto &b : Devices.active() ) {
           QPair<QVector<QString>, QVector<inChannel*>> its = b->inChans_to_save();
           QPair<QVector<QString>, QVector<outChannel*>> ots = b->outChans_to_save();
           headerIn.append(its.first);
           headerOut.append(ots.first);
           inChnsToSave.append(its.second);
           outChnsToSave.append(ots.second);
       }
       for ( auto const& m : Models.active() ) {
           QPair<QVector<QString>, QVector<inChannel*>> its = m->inChans_to_save();
           QPair<QVector<QString>, QVector<outChannel*>> ots = m->outChans_to_save();
           headerIn.append(its.first);
           headerOut.append(ots.first);
           inChnsToSave.append(its.second);
           outChnsToSave.append(ots.second);
       }

       QVector<QString> header(1, "Time");
       header.append(headerIn);
       header.append(headerOut);
#ifdef TEST_VERSION
       for ( AECChannel *aec : aecChannels ) {
           header.append(QString("VAEC_%1").arg(aec->inChnNum.toString('_')));
       }
#endif
       dataSaver->SaveHeader(header);

       data.clear();
       data.resize(header.size());
   }

   bool limitWarningEmitted = false;
   bool processAnalogs = false;

   message(QString("DynClamp: Clamping ..."));
   stopped= false;
   finished= false;
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
         for ( ChemSyn &obj : csynPre )
             obj.currentUpdate(t, dt);
         for ( abSyn &obj : absynPre )
             obj.currentUpdate(t, dt);
         for ( DestexheSyn &obj : dsynPre )
             obj.currentUpdate(t, dt);
         for ( GapJunction &obj : esyn )
             obj.currentUpdate(t, dt);

         for ( HH &obj : hhPre )
             obj.currentUpdate(t, dt);
         for ( abHH &obj : abhhPre )
             obj.currentUpdate(t, dt);

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

                     for ( HH &obj : hhIn )
                         obj.RK4(subT, rkDT, rkStep);
                     for ( abHH &obj : abhhIn )
                         obj.RK4(subT, rkDT, rkStep);

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
         for ( ChemSyn &obj : csynPost )
             obj.currentUpdate(t, dt);
         for ( abSyn &obj : absynPost )
             obj.currentUpdate(t, dt);
         for ( DestexheSyn &obj : dsynPost )
             obj.currentUpdate(t, dt);

         //--------------- Data saving ------------------------//
         if ( saving && dataSavingPs.enabled ) {
           if ( t >= lastSave + savingPeriod )
           {
              i = 0;
              data[i++] = t;
              for ( inChannel *in : inChnsToSave )
                  data[i++] = in->V;
              for ( outChannel *out : outChnsToSave )
                  data[i++] = out->I;
            #ifdef TEST_VERSION
              for ( AECChannel *aec : aecChannels )
                  data[i++] = aec->v_e;
            #endif

              dataSaver->SaveLine(data);
              lastSave = t;
           }
         }
         //--------------- Data saving end ---------------------//

         // Updated display
         if ( graph ) {
             if ( t - lastWrite > graphDt ) {
                 lastWrite = t;
                 i = 0;
                 for ( double *val : graphVar )
                    graph->q[i++]->push(GraphDlg::DataPoint {t, *val});
             }
         }

         // Rate update
         ++rateCounter;
         if ( t - lastRateReport > 1 ) {
             lastRateReport = t;
             emit updateRate(rateCounter);
             rateCounter = 0;
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
             if ( b->check_limits(!limitWarningEmitted, w) )
                 emit CloseToLimit(w.what, w.chan_label, w.loLim, w.hiLim, w.value);
         }

     // --- Calculate end --- //

     // copy AEC compensated input values to output channels if desired
     for ( int k=0; k<aecChannels.size(); k++ ){
         if ( aecCopy[k])
             aecCopy[k]->I= aecIn[k]->V;
     }

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

template <class T>
void DCThread::instantiate(std::vector<T> &preModel, std::vector<T> &inModel,
                           typename T::param_type &p, CurrentAssignment &a)
{
    CurrentAssignment tmp;
    tmp.actP = &a.active;
    if ( a.VChannel == a.IChannel ) {
        // Input/Output on the same model => connect instances 1-to-1 rather than all-to-all
        for ( ChannelIndex VIChan : getChanIndices(a.VChannel) ) {
            tmp.VChannel = VIChan;
            tmp.IChannel = VIChan;
            inModel.push_back(T(&p, this, tmp));
        }
    } else { // NOTE: These channels are assumed to be analog only, although technically, a/d combos are permitted
        for ( ChannelIndex VChan : getChanIndices(a.VChannel) ) {
            tmp.VChannel = VChan;
            for ( ChannelIndex IChan : getChanIndices(a.IChannel) ) {
                tmp.IChannel = IChan;
                preModel.push_back(T(&p, this, tmp));
            }
        }
    }
}

template <typename T>
void DCThread::instantiate(std::vector<T> &preModel, std::vector<T> &postModel,
                           typename T::param_type &p, SynapseAssignment &a)
{
    std::vector<ChannelIndex> preSynInst = getChanIndices(a.PreSynChannel);
    std::vector<ChannelIndex> postSynInst = getChanIndices(a.PostSynChannel);
    std::vector<ChannelIndex> outSynInst = getChanIndices(a.OutSynChannel);
    inChannel *preC, *postC;
    outChannel *outC;

    if ( a.PostSynChannel == a.OutSynChannel ) {
        for ( ChannelIndex post : postSynInst ) {
            for ( ChannelIndex pre : preSynInst ) {
                if ( (preC=getInChan(pre)) && (postC=getInChan(post)) && (outC=getOutChan(post)) ) {
                    T tmp(&p, this, a, preC, postC, outC);
                    if ( pre.isVirtual && post.isAnalog )
                        postModel.push_back(tmp);
                    else
                        preModel.push_back(tmp);
                }
            }
        }
    } else {
        for ( ChannelIndex post : postSynInst ) {
            for ( ChannelIndex out : outSynInst ) {
                for ( ChannelIndex pre : preSynInst ) {
                    if ( (preC=getInChan(pre)) && (postC=getInChan(post)) && (outC=getOutChan(out)) ) {
                        T tmp(&p, this, a, preC, postC, outC);
                        if ( (pre.isVirtual || post.isVirtual) && out.isAnalog )
                            postModel.push_back(tmp);
                        else
                            preModel.push_back(tmp);
                    }
                }
            }
        }
    }
}

template <typename T>
void DCThread::instantiate(std::vector<T> &inst, typename T::param_type &p, GapJunctionAssignment &a)
{
    std::vector<GapJunctionAssignment> vec;
    GapJunctionAssignment tmp;
    tmp.actP = &a.active;
    if ( a.postInChannel == a.postOutChannel ) {
        for ( ChannelIndex post : getChanIndices(a.postInChannel) ) {
            tmp.postInChannel = post;
            tmp.postOutChannel = post;
            vec.push_back(tmp);
        }
    } else {
        for ( ChannelIndex in : getChanIndices(a.postInChannel) ) {
            tmp.postInChannel = in;
            for ( ChannelIndex out : getChanIndices(a.postOutChannel) ) {
                tmp.postOutChannel = out;
                vec.push_back(tmp);
            }
        }
    }

    if ( a.preInChannel == a.preOutChannel ) {
        for ( ChannelIndex pre : getChanIndices(a.preInChannel) ) {
            for ( GapJunctionAssignment &tmp : vec ) {
                tmp.preInChannel = pre;
                tmp.preOutChannel = pre;
                inst.push_back(T(&p, this, tmp));
            }
        }
    } else {
        for ( ChannelIndex in : getChanIndices(a.preInChannel) ) {
            for ( ChannelIndex out : getChanIndices(a.preOutChannel) ) {
                for ( GapJunctionAssignment &tmp : vec ) {
                    tmp.preInChannel = in;
                    tmp.preOutChannel = out;
                    inst.push_back(T(&p, this, tmp));
                }
            }
        }
    }
}


// Scripting support

bool DCThread::LoadScript(QString &fname)
{
  double et;
  QString rawname;
  bool done, success;
  QString qn;
  char buf[80];

  ifstream is(fname.toLatin1());

  is >> et;
  done= false;
  success= true;
  while (!done)
  {
    is >> buf;
    rawname= QString(buf);
    AP *it = AP::find(rawname);
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


