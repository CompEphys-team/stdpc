#include "DCThread.h"
#include "AP.h"
#include "Global.h"
#include <cmath>
#include <climits>
#include "ChannelIndex.h"
#include "DeviceManager.h"

#include "SimulDAQ.h" // for debugging only ...

DCThread::DCThread()
{
  stopped= true;
  finished= true;

  scripting= false;

  aecChannels = QVector<AECChannel*>(MAX_ELECTRODE_NO);
  for(int i=0; i<aecChannels.size(); i++)
    aecChannels[i] = new AECChannel();

  dataSaver = new DataSaver();
}

DCThread::~DCThread()
{
}


void DCThread::run()
{
   int numOfAecChannels;
   double savingPeriod= 1.0;

   static int i;
   static double evt;
   bool SampleHoldOn= false;
   inChannel *SampleHoldChan;
   DAQ *SampleHoldBoard;

   inChnSG.init(&inSpkGenChnp);
   outChnNone.init(&outSpkGenChnp);

   for ( DAQ *b : Devices.actdev )
       b->init_chans();

   hhNeuron.clear();
   for ( HHNeuronData &p : HHNeuronp ) {
       hhNeuron.push_back(HHNeuronModel(&p));
   }

   if (SGp.active) {
     SG.init(&SGp, &inChnSG, this);
     if (!SGp.active) {
       message(QString("SpkGen: Could not open ")+SGp.STInFName+QString(" ... SpkGen disabled ..."));
       inChnSG.V= 0.0;
     }
     else {
       message(QString("SpkGen active ..."));
     }
   }
   else {
     inChnSG.V= 0.0;
   }
   inChnSG.active = SGp.active;

   SampleHoldChan = getInChan(SampleHoldp.trigChn);
   SampleHoldBoard = Devices.getDevice(SampleHoldp.trigChn);

   // set up the graphic display channels
   QString lb;
   for (i= 0; i < 2; i++) {
     grpNo[i]= 0;
     for (int j= 0; j < 4; j++) {
       if (Graphp[i].active[j]) {
         ChannelIndex &dex = Graphp[i].chn[j];
         if ( !dex.isValid || dex.isNone ) {
             continue;
         } else if ( dex.isSG ) {
             grp[i][grpNo[i]]= &(SG.V);
         } else if ( dex.isVirtual ) {
             // Use inchan for mV units, outchan for nA units
             grp[i][grpNo[i]] = Graphp[i].yfac[j]==1e3
                     ? &(getInChan(dex)->V)
                     : &(getOutChan(dex)->I);
         } else if ( dex.isInChn ) {
             grp[i][grpNo[i]] = &(getInChan(dex)->V);
         } else {
             grp[i][grpNo[i]] = &(getOutChan(dex)->I);
         }
         pen[i][grpNo[i]++] = j;
       }
     }
     lb.setNum(grpNo[i]);
     message(QString("Added ")+lb+QString(" channels for Display"));
   }

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

   int sz;
   sz= tanhLU.generate();
   if (sz > 0) message(QString("Tanh lookup table (re)generated"));
   sz= expLU.generate();
   if (sz > 0) message(QString("Exp lookup table (re)generated"));
   sz= expSigmoidLU.generate();
   if (sz > 0) message(QString("Exponential sigmoid lookup table (re)generated"));

   // Checking validity of AEC channels
   numOfAecChannels = 0;
   QVector<inChannel*> aecIn(aecChannels.size(), nullptr);
   QVector<outChannel*> aecOut(aecChannels.size(), nullptr);
   QVector<outChannel*> aecCopy(aecChannels.size(), nullptr);
   for ( int i=0; i<aecChannels.size(); i++ ){
     if ( aecChannels[i]->IsActive() ){
       aecIn[i] = getInChan(aecChannels[i]->inChnNum);
       aecOut[i] = getOutChan(aecChannels[i]->outChnNum);
       aecCopy[i] = getOutChan(elecCalibPs[i].copyChn);
       if ( aecIn[i] && aecOut[i] && aecIn[i]->active && aecOut[i]->active ) {
         message(QString("AEC channel is active on input channel ")+aecChannels[i]->inChnNum.toString()+" and output channel "+aecChannels[i]->outChnNum.toString());
       #ifdef TEST_VERSION
         numOfAecChannels++;
       #endif
       }
       else {
         if ( !aecIn[i] || !aecIn[i]->active ){
           message(QString("Input channel ")+aecChannels[i]->inChnNum.toString()+QString(" is not active! AEC disabled on that channel."));
           aecChannels[i]->Inactivate();
         }
         if ( !aecOut[i] || !aecOut[i]->active ){
           message(QString("Output channel ")+aecChannels[i]->outChnNum.toString()+QString(" is not active! AEC disabled on that channel."));
           aecChannels[i]->Inactivate();
         }
       }
     }
   }

   // Init data saving
   if (dataSavingPs.enabled) {
       dataSaver->InitDataSaving(dataSavingPs.fileName, dataSavingPs.isBinary);
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
       if ( SGp.saving ) {
           headerIn.append("SpikeGenerator");
           inChnsToSave.append(&inChnSG);
       }

       QVector<QString> header(1, "Time");
       header.append(headerIn);
       header.append(headerOut);
       for ( int i = 0; i < numOfAecChannels; i++ ) {
           header.append(QString("Ve_%1").arg(i));
       }
       dataSaver->SaveHeader(header);

       data.clear();
       data.resize(header.size());
   }

   bool limitWarningEmitted = false;

   message(QString("DynClamp: Clamping ..."));
   stopped= false;
   finished= false;
   t= 0.0;
   double lastSave = t;
   for (int i= 0; i < 2; i++) lastWrite[i]= t;
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
            if ( aecChannels[k]->IsActive() ) {
                aecChannels[k]->CalculateVe(aecOut[k]->I, dt);
                aecIn[k]->V -= aecChannels[k]->v_e;
            }
        }

        // Apply biases & detect spikes
        for ( DAQ *b : Devices.actdev )
            b->process_scan(t);
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
         if (dataSavingPs.enabled) {
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
                  if ( aec->IsActive() )
                      data[i++] = aec->v_e;
            #endif

              dataSaver->SaveLine(data);
              lastSave = t;
           }
         }
         //--------------- Data saving end ---------------------//


         // Dynamic clamp current generation
         if (SGp.active) { // SpkGen active
           SG.VUpdate(t, dt);
           inChnSG.spike_detect(t);
         }
         inChnSG.active = SGp.active;

         for ( HHNeuronModel &model : hhNeuron )
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

         // Updated display
         if ( grpNo[0] ) {
             if (t-lastWrite[0] > Graphp[0].dt) {
               lastWrite[0]= t;
               for (i= 0; i < grpNo[0]; i++) {
                 addPoint1(t, *grp[0][i], pen[0][i]);
               }
             }
         }
         if ( grpNo[1] ) {
             if (t-lastWrite[1] > Graphp[1].dt) {
               lastWrite[1]= t;
               for (i= 0; i < grpNo[1]; i++) {
                 addPoint2(t, *grp[1][i], pen[1][i]);
               }
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
         if ( aecChannels[k]->IsActive() && elecCalibPs[k].copyChnOn && aecCopy[k])
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

   if (dataSavingPs.enabled) {
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
        }
    } else if ( dex.isSG ) {
        return &inChnSG;
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


