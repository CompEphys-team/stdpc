#include "DCThread.h"
#include "AP.h"
#include "Global.h"
#include <cmath>
#include <climits>
#include "ChannelIndex.h"
#include "DeviceManager.h"
#include "GraphDlg.h"

#include "SimulDAQ.h" // for debugging only ...

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

   static int i;
   static double evt;
   bool SampleHoldOn= false;
   inChannel *SampleHoldChan;
   DAQ *SampleHoldBoard;

   for ( DAQ *b : Devices.actdev )
       b->init_chans();

   hhNeuron.clear();
   for ( HHNeuronData &p : HHNeuronp ) {
       hhNeuron.push_back(HHNeuronModel(&p));
   }
   SGProto.clear();
   for ( SGData &p : SGp )
       SGProto.push_back(SpkGenPrototype(&p, this));

   SampleHoldChan = getInChan(SampleHoldp.trigChn);
   SampleHoldBoard = Devices.getDevice(SampleHoldp.trigChn);

   // set up the graphic display channels
   double graphDummy = 0.0;
   if ( graph ) {
       i = 0;
       graphVar.resize(Graphp.size());
       for ( GraphData &p : Graphp ) {
           if ( !p.active || !p.chan.isValid || p.chan.isNone ) {
               graphVar[i] = &graphDummy;
           } else if ( p.chan.isVirtual ) {
               graphVar[i] = p.isVoltage
                       ? &getInChan(p.chan)->V
                       : &getOutChan(p.chan)->I;
           } else if ( p.chan.isInChn ) {
               graphVar[i] =& getInChan(p.chan)->V;
           } else {
               graphVar[i] =& getOutChan(p.chan)->I;
           }
           ++i;
       }
       message(QString("Added %1 channels for display").arg(i));
   }

   bufferHelper.reset(new ChannelBufferHelper);
   csyn.clear();
   for ( CSynData &p : CSynp ) {
       if ( p.active ) {
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(csyn, p, a);
       }
   }
   absyn.clear();
   for ( abSynData &p : abSynp ) {
       if ( p.active ) {
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(absyn, p, a);
       }
   }
   dsyn.clear();
   for ( DestexheSynData &p : DxheSynp ) {
       if ( p.active ) {
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(dsyn, p, a);
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
   hh.clear();
   for ( mhHHData &p : mhHHp ) {
       if ( p.active ) {
           for ( CurrentAssignment &a : p.assign ) {
               if ( a.active )
                   instantiate(hh, p, a);
           }
       }
   }
   abhh.clear();
   for ( abHHData &p : abHHp ) {
       if ( p.active ) {
           for ( CurrentAssignment &a : p.assign ) {
               if ( a.active )
                   instantiate(abhh, p, a);
           }
       }
   }
   if (csyn.size() > 0) message(QString("DynClamp: %1 chemical synapse(s) ").arg(csyn.size()));
   if (absyn.size() > 0) message(QString("DynClamp: %1 ab synapse(s) ").arg(absyn.size()));
   if (esyn.size() > 0) message(QString("DynClamp: %1 gap junction(s) ").arg(esyn.size()));
   if (dsyn.size() > 0) message(QString("DynClamp: %1 Destexhe synapse(s) ").arg(dsyn.size()));
   if (hh.size() > 0) message(QString("DynClamp: %1 HH conductance(s) ").arg(hh.size()));
   if (abhh.size() > 0) message(QString("DynClamp: %1 abHH conductance(s) ...").arg(abhh.size()));
   if (hhNeuron.size() > 0) {
       int nModels = 0, nInst = 0;
       for ( HHNeuronModel &model : hhNeuron ) {
           auto rep = model.numActiveInst();
           nModels += rep.first;
           nInst += rep.second;
       }
       message(QString("DynClamp: %1 HH neuron model(s) with %2 total instance(s) ").arg(nModels).arg(nInst));
   }
   if (SGProto.size() > 0) {
       int nModels = 0, nInst = 0;
       for ( SpkGenPrototype &model : SGProto ) {
           auto rep = model.numActiveInst();
           nModels += rep.first;
           nInst += rep.second;
       }
       message(QString("DynClamp: %1 Spike generator model(s) with %2 total instance(s) ").arg(nModels).arg(nInst));
   }

   int sz;
   sz= tanhLU.generate();
   if (sz > 0) message(QString("Tanh lookup table (re)generated"));
   sz= expLU.generate();
   if (sz > 0) message(QString("Exp lookup table (re)generated"));
   sz= expSigmoidLU.generate();
   if (sz > 0) message(QString("Exponential sigmoid lookup table (re)generated"));

   // Checking validity of AEC channels
   QVector<AECChannel*> aecChannels;
   for ( DAQ *b : Devices.actdev ) {
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

       for ( DAQ *b : Devices.actdev ) {
           QPair<QVector<QString>, QVector<inChannel*>> its = b->inChans_to_save();
           QPair<QVector<QString>, QVector<outChannel*>> ots = b->outChans_to_save();
           headerIn.append(its.first);
           headerOut.append(ots.first);
           inChnsToSave.append(its.second);
           outChnsToSave.append(ots.second);
       }
       int i = 0;
       for ( HHNeuronModel &m : hhNeuron ) {
           QPair<QVector<QString>, QVector<inChannel*>> its = m.inChans_to_save(i);
           QPair<QVector<QString>, QVector<outChannel*>> ots = m.outChans_to_save(i++);
           headerIn.append(its.first);
           headerOut.append(ots.first);
           inChnsToSave.append(its.second);
           outChnsToSave.append(ots.second);
       }
       for ( SpkGenPrototype &m : SGProto ) {
           QPair<QVector<QString>, QVector<inChannel*>> its = m.inChans_to_save(i);
           headerIn.append(its.first);
           inChnsToSave.append(its.second);
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

   DAQClock.reset_RTC();
   for ( DAQ *b : Devices.actdev )
       b->start();

   // Dynamic clamp loop begins
   while (!stopped) {


     if (!SampleHoldOn) {
       // --- Read --- //
         for ( DAQ *b : Devices.actdev )
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

         if ( processAnalogs ) {
             for ( DAQ *b : Devices.actdev )
                 b->process_scan(t);
             processAnalogs = false;
         }

         // Dynamic clamp current generation

         for ( HHNeuronModel &model : hhNeuron )
             model.updateChannels(t);
         for ( SpkGenPrototype &model : SGProto )
             model.updateChannels(t);

         for ( ChemSyn &obj : csyn )
             obj.currentUpdate(t, dt);
         for ( abSyn &obj : absyn )
             obj.currentUpdate(t, dt);
         for ( DestexheSyn &obj : dsyn )
             obj.currentUpdate(t, dt);
         for ( GapJunction &obj : esyn )
             obj.currentUpdate(t, dt);
         for ( HH &obj : hh )
             obj.currentUpdate(t, dt);
         for ( abHH &obj : abhh )
             obj.currentUpdate(t, dt);

         for ( HHNeuronModel &model : hhNeuron )
             model.updateNeurons(t, dt);
         for ( SpkGenPrototype &model : SGProto )
             model.updateInstances(t, dt);

         bufferHelper->advance(t);

         // Updated display
         if ( graph ) {
             if ( t - lastWrite > graphDt ) {
                 lastWrite = t;
                 i = 0;
                 for ( double *val : graphVar )
                    graph->q[i++]->push(GraphDlg::DataPoint {t, *val});
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
         for ( DAQ *b : Devices.actdev ) {
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
     for ( DAQ *b : Devices.actdev )
         b->write_analog_out();
     // --- Write end --- //


   }
   // Dynamic clamp loop ends


   // In the end, let's do the cleanup
   for ( DAQ *b : Devices.actdev ) {
       b->reset_board();
       b->reset_chans();
   }
   for ( int i=0; i<aecChannels.size(); i++ ) if ( aecChannels[i]->IsActive() ) aecChannels[i]->ResetChannel();

   if ( saving ) {
       dataSaver->EndDataSaving();
   }

   finished= true;

}


inChannel *DCThread::getInChan(ChannelIndex const& dex)
{
    if ( !dex.isValid || dex.isPrototype ) {
        return nullptr;
    } else if ( dex.isVirtual ) {
        switch ( dex.modelClass ) {
        case ModelClass::HH:
            if ( (int)hhNeuron.size() <= dex.modelID || (int)hhNeuron[dex.modelID].inst.size() <= dex.instID )
                return nullptr;
            return &(hhNeuron[dex.modelID].inst[dex.instID].in);
        case ModelClass::SG:
            if ( (int)SGProto.size() <= dex.modelID || (int)SGProto[dex.modelID].inst.size() <= dex.instID )
                return nullptr;
            return &(SGProto[dex.modelID].inst[dex.instID]->in);
        }
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
        switch ( dex.modelClass ) {
        case ModelClass::HH:
            if ( (int)hhNeuron.size() <= dex.modelID || (int)hhNeuron[dex.modelID].inst.size() <= dex.instID )
                return nullptr;
            return &(hhNeuron[dex.modelID].inst[dex.instID].out);
        case ModelClass::SG:
            return nullptr; // No SG input channels
        }
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
        switch ( dex.modelClass ) {
        case ModelClass::HH:
            if ( dex.modelID < (int)HHNeuronp.size() )
                for ( size_t i = 0; i < HHNeuronp[dex.modelID].inst.size(); i++ )
                    ret.push_back(dex.toInstance(i));
            break;
        case ModelClass::SG:
            if ( dex.modelID < (int)SGp.size() )
                for ( size_t i = 0; i < SGp[dex.modelID].inst.size(); i++ )
                    ret.push_back(dex.toInstance(i));
            break;
        }
    } else {
        ret.push_back(dex);
    }
    return ret;
}

template <class T>
void DCThread::instantiate(std::vector<T> &inst, typename T::param_type &p, CurrentAssignment &a)
{
    CurrentAssignment tmp;
    tmp.actP = &a.active;
    if ( a.VChannel == a.IChannel ) {
        // Input/Output on the same model => connect instances 1-to-1 rather than all-to-all
        for ( ChannelIndex VIChan : getChanIndices(a.VChannel) ) {
            tmp.VChannel = VIChan;
            tmp.IChannel = VIChan;
            inst.push_back(T(&p, this, tmp));
        }
    } else {
        for ( ChannelIndex VChan : getChanIndices(a.VChannel) ) {
            tmp.VChannel = VChan;
            for ( ChannelIndex IChan : getChanIndices(a.IChannel) ) {
                tmp.IChannel = IChan;
                inst.push_back(T(&p, this, tmp));
            }
        }
    }
}

template <typename T>
void DCThread::instantiate(std::vector<T> &inst, typename T::param_type &p, SynapseAssignment &a)
{
    SynapseAssignment tmp;
    tmp.actP = &a.active;
    if ( a.PostSynChannel == a.OutSynChannel ) {
        for ( ChannelIndex post : getChanIndices(a.PostSynChannel) ) {
            tmp.PostSynChannel = post;
            tmp.OutSynChannel = post;
            for ( ChannelIndex pre : getChanIndices(a.PreSynChannel) ) {
                tmp.PreSynChannel = pre;
                inst.push_back(T(&p, this, tmp));
            }
        }
    } else {
        for ( ChannelIndex post : getChanIndices(a.PostSynChannel) ) {
            tmp.PostSynChannel = post;
            for ( ChannelIndex out : getChanIndices(a.OutSynChannel) ) {
                tmp.OutSynChannel = out;
                for ( ChannelIndex pre : getChanIndices(a.PreSynChannel) ) {
                    tmp.PreSynChannel = pre;
                    inst.push_back(T(&p, this, tmp));
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


