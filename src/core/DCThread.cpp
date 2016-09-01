#include "DCThread.h"
#include "AP.h"
#include "Global.h"
#include <cmath>
#include <climits>
#include "ChannelIndex.h"

  
#include "SimulDAQ.h" // for debugging only ...
 
DCThread::DCThread() 
{
  stopped= true;
  finished= true;

  inChn= NULL;
  outChn= NULL;
  inIdx= NULL;
  outIdx= NULL;
  scripting= false;

  aecChannels = QVector<AECChannel*>(MAX_ELECTRODE_NO);
  for(int i=0; i<aecChannels.size(); i++)
    aecChannels[i] = new AECChannel();

  dataSaver = new DataSaver();
}

DCThread::~DCThread() 
{
   if (inChn != NULL) delete[] inChn;
   if (outChn != NULL) delete[] outChn;
   if (inIdx != NULL) delete[] inIdx;
   if (outIdx != NULL) delete[] outIdx;  
}
 

void DCThread::init(DAQ *inBoard) 
{
   if (inChn != NULL) delete[] inChn;
   if (outChn != NULL) delete[] outChn;
   if (inIdx != NULL) delete[] inIdx;
   if (outIdx != NULL) delete[] outIdx;

   board= inBoard;
   inChn= new inChannel[board->inChnNo+1];  // +1 for the spike Generator
   outChn= new outChannel[board->outChnNo+1]; // +1 for "none" output
   inIdx= new short int[board->inChnNo+1];
   outIdx= new short int[board->outChnNo+1];
}


void DCThread::run()
{
   int numOfAecChannels;
   double savingPeriod= 1.0;

   static int i;
   static double evt;
   // reset Lookup tables; they not be needed in this run ...
//   tanhLU.reset();
//   expLU.reset();
//   expSigmoidLU.reset();
   // collect the active channels
   //exit(1);
   // for Attila's Sample-and-hold
   bool SampleHoldOn= false;

   inNo= 0;  
   for (i= 0; i < board->inChnNo; i++) {  
     if (inChnp[i].active) {
       inIdx[inNo++]= i;
       inChn[i].init(&inChnp[i]);
     }
   }
   inIdx[inNo]= board->inChnNo;  // this is for the Spike Generator
   inChn[board->inChnNo].init(&inSpkGenChnp);
   outNo= 0;
   for (i=0; i < board->outChnNo; i++) {
     if (outChnp[i].active) {
       outIdx[outNo++]= i;
       outChn[i].init(&outChnp[i]);
     }
   }
   outIdx[outNo]= board->outChnNo;  // this is for the None output
   outChn[board->outChnNo].init(&outSpkGenChnp);
   
   board->generate_scan_list(inNo, inIdx);
   board->generate_analog_out_list(outNo, outIdx);

   if (SGp.active) {
     SG.init(&SGp, inNo, inIdx, inChn);
     if (!SGp.active) {    
       message(QString("SpkGen: Could not open ")+SGp.STInFName+QString(" ... SpkGen disabled ..."));
       inChn[inIdx[inNo]].V= 0.0;
     }
     else {
       message(QString("SpkGen active ..."));
     }
   }
   else {
     inChn[inIdx[inNo]].V= 0.0;
   }

   hhNeuron.clear();
   for ( HHNeuronData &p : HHNeuronp ) {
       hhNeuron.push_back(HHNeuronModel(&p));
   }
   
   // set up the graphic display channels
   QString lb;
   for (i= 0; i < 2; i++) {
     grpNo[i]= 0;
     for (int j= 0; j < 4; j++) {
       if (Graphp[i].active[j]) {
         ChannelIndex dex(Graphp[i].chn[j]);
         if ( dex.isSG ) {
             grp[i][grpNo[i]]= &(SG.V);
         } else if ( dex.isVirtual ) {
             // Use inchan for mV units, outchan for nA units
             grp[i][grpNo[i]] = Graphp[i].yfac[j]==1e3
                     ? &hhNeuron[dex.modelID].inst[dex.instID].in.V
                     : &hhNeuron[dex.modelID].inst[dex.instID].out.I;
         } else if ( dex.index >= OUTCHN_OFFSET ) {
             grp[i][grpNo[i]] = &(outChn[outIdx[dex.index - OUTCHN_OFFSET]].I);
         } else if ( dex.index >= INCHN_OFFSET ) {
             grp[i][grpNo[i]] = &(inChn[inIdx[dex.index - INCHN_OFFSET]].V);
         } else {
             continue; // Oughtn't happen
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
           if ( !p.noLegacyAssign )
               csyn.push_back(ChemSyn(&p, this));
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(csyn, p, a);
       }
   }
   absyn.clear();
   for ( abSynData &p : abSynp ) {
       if ( p.active ) {
           if ( !p.noLegacyAssign )
               absyn.push_back(abSyn(&p, this));
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(absyn, p, a);
       }
   }
   dsyn.clear();
   for ( DestexheSynData &p : DxheSynp ) {
       if ( p.active ) {
           if ( !p.noLegacyAssign )
               dsyn.push_back(DestexheSyn(&p, this));
           for ( SynapseAssignment &a : p.assign )
               if ( a.active )
                   instantiate(dsyn, p, a);
       }
   }
   esyn.clear();
   for ( GJunctData &p : ESynp ) {
       if ( p.active ) {
           if ( !p.noLegacyAssign )
               esyn.push_back(GapJunction(&p, this));
           for ( GapJunctionAssignment &a : p.assign )
               if ( a.active )
                   instantiate(esyn, p, a);
       }
   }
   hh.clear();
   for ( mhHHData &p : mhHHp ) {
       if ( p.active ) {
           if ( !p.noLegacyAssign ) {
               hh.push_back(HH(&p, this));
           }
           for ( CurrentAssignment &a : p.assign ) {
               if ( a.active )
                   instantiate(hh, p, a);
           }
       }
   }
   abhh.clear();
   for ( abHHData &p : abHHp ) {
       if ( p.active ) {
           if ( !p.noLegacyAssign ) {
               abhh.push_back(abHH(&p, this));
           }
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
   for ( int i=0; i<aecChannels.size(); i++ ){
     if ( aecChannels[i]->IsActive() ){
       if ( inChnp[aecChannels[i]->inChnNum].active == true &&
         outChnp[aecChannels[i]->outChnNum].active == true ){
         message(QString("AEC channel is active on input channel ")+QString::number(aecChannels[i]->inChnNum)+" and output channel "+QString::number(aecChannels[i]->outChnNum));
       #ifdef TEST_VERSION
         numOfAecChannels++;
       #endif
       }
       else {
         if ( inChnp[aecChannels[i]->inChnNum].active == false ){
           message(QString("Input channel ")+QString::number(aecChannels[i]->inChnNum)+QString(" is not active! AEC disabled on that channel."));
           aecChannels[i]->Inactivate();
         }
         if ( outChnp[aecChannels[i]->outChnNum].active == false ){
           message(QString("Output channel ")+QString::number(aecChannels[i]->outChnNum)+QString(" is not active! AEC disabled on that channel."));
           aecChannels[i]->Inactivate();
         }
       }
     }
   }

   // Create tolerance limits for all AEC channels
   QVector<double> iHiLim = QVector<double>(outNo);   // high limit for current
   QVector<double> iLoLim = QVector<double>(outNo);   // low limit for currents
   QVector<double> vHiLim = QVector<double>(inNo);    // high limit for voltages
   QVector<double> vLoLim = QVector<double>(inNo);    // low limit for voltages
   double upTolFac = 0.99;   // upper tolerance factor
   double loTolFac = 0.01;   // lower  tolerance factor
   for ( int i=0; i<outNo; i++ ){
     iHiLim[i] = outChnp[outIdx[i]].minCurrent + upTolFac*(outChnp[outIdx[i]].maxCurrent-outChnp[outIdx[i]].minCurrent);
     iLoLim[i] = outChnp[outIdx[i]].minCurrent + loTolFac*(outChnp[outIdx[i]].maxCurrent-outChnp[outIdx[i]].minCurrent);
   }
   for ( int i=0; i<inNo; i++ ){
     vHiLim[i] = inChnp[inIdx[i]].minVoltage + upTolFac*(inChnp[inIdx[i]].maxVoltage-inChnp[inIdx[i]].minVoltage);
     vLoLim[i] = inChnp[inIdx[i]].minVoltage + loTolFac*(inChnp[inIdx[i]].maxVoltage-inChnp[inIdx[i]].minVoltage);
   }

   // Generate the index vector of in and out channels to be saved
   inChnsToSave = QVector<int>(0);
   for( i= 0; i < inNo; i++ )
     if( inChn[inIdx[i]].save ) inChnsToSave.append(inIdx[i]);
   outChnsToSave = QVector<int>(0);
   for( i= 0; i < outNo; i++ )
     if( outChn[outIdx[i]].save ) outChnsToSave.append(outIdx[i]);

   // Init data saving
   if (dataSavingPs.enabled) {
     dataSaver->InitDataSaving(dataSavingPs.fileName, dataSavingPs.isBinary);
     savingPeriod = 1.0 / dataSavingPs.savingFreq;

     // numOfAecChannels is only nonzero if TEST_VERSION is defined
     data = QVector<double>(1+inChnsToSave.size()+outChnsToSave.size()+SGp.saving+numOfAecChannels); // time + in + out + SG (which can be 0)
     QVector<QString> header = QVector<QString>(data.size()); // write file header
     header[0] = "Time";
     QString tmp;
     for( i = 0; i < inChnsToSave.size();  i++) header[i+1]           = "V"+tmp.setNum(inChnsToSave[i]);
     if( SGp.saving ) header[1+inChnsToSave.size()] = "SpikeGenerator";
     for( i = 0; i < outChnsToSave.size(); i++) header[1+inChnsToSave.size()+SGp.saving+i] = "I"+tmp.setNum(outChnsToSave[i]);

     // numOfAecChannels is only nonzero if TEST_VERSION is defined
     for( i = 0; i < numOfAecChannels; i++) header[1+inChnsToSave.size()+SGp.saving+outChnsToSave.size()+i] = "Ve_"+tmp.setNum(i);

     // Add virtual neuron channels as needed
     int modelNum = 0;
     for ( HHNeuronModel &model : hhNeuron ) {
         header.append(model.labels(modelNum++));
     }
     data.resize(header.size());

     dataSaver->SaveHeader(header);
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
   board->reset_RTC();

   // Dynamic clamp loop begins
   while (!stopped) {       


     if (!SampleHoldOn) {
       // --- Read --- //
          board->get_scan(inChn);
       // --- Read end --- //

       // --- Calculate --- //

        // Adaptive time step
        dt = board->get_RTC();
        t += dt;

        // AEC channel update part
        for ( int k=0; k<aecChannels.size(); k++ )
            if ( aecChannels[k]->IsActive() ) aecChannels[k]->CalculateVe(outChn[aecChannels[k]->outChnNum].I, dt);

        // AEC compensation
        for ( int k=0; k<aecChannels.size(); k++ ){
              if ( aecChannels[k]->IsActive() ) inChn[aecChannels[k]->inChnNum].V -= aecChannels[k]->v_e;
        }
        // Apply channel bias voltages
        for (int k= 0; k < inNo; k++) {
            inChn[k].V+= inChnp[inIdx[k]].bias;
        }
     }
     else {
         // Adaptive time step
         dt = board->get_RTC();
         t += dt;
     }
     if (SampleHoldp.active) {
         if (SampleHoldOn) {
             board->get_single_scan(inChn, SampleHoldp.trigChn);
             if (inChn[SampleHoldp.trigChn].V < SampleHoldp.threshV) {
                 SampleHoldOn= false;
             }
         }
         else {
             if (inChn[SampleHoldp.trigChn].V >= SampleHoldp.threshV) {
                 SampleHoldOn= true;
             }
         }
     }

     //--------------- Data saving ------------------------//
         if (dataSavingPs.enabled) {
           if ( t >= lastSave + savingPeriod )
           {
              data[0] = t;

              for (i= 0; i < inChnsToSave.size(); i++)   data[i+1] = inChn[inChnsToSave[i]].V;    // voltages
              if ( SGp.saving ) data[inChnsToSave.size()+1] = SG.V; // spike generator
              for (i= 0; i < outChnsToSave.size(); i++)  data[inChnsToSave.size()+1+SGp.saving+i] = outChn[outChnsToSave[i]].I;    // currents

              QVector<double>::iterator it = data.begin();
              it += inChnsToSave.size()+1+SGp.saving+outChnsToSave.size();
            #ifdef TEST_VERSION
              int aecChannelNum = 0;
              for ( int i=0; i<aecChannels.size(); i++ )
                  if ( aecChannels[i]->IsActive() )
                  {
                      data[inChnsToSave.size()+1+SGp.saving+outChnsToSave.size()+aecChannelNum] = aecChannels[i]->v_e;
                      aecChannelNum++;
                  }
              it += aecChannelNum;
            #endif

              for ( HHNeuronModel &model : hhNeuron )
                  model.putData(it);

              dataSaver->SaveLine(data);
              lastSave = t;
           }
         }
         //--------------- Data saving end ---------------------//


         // Dynamic clamp current generation
         if (SGp.active) { // SpkGen active
           SG.VUpdate(t, dt);
         }
         for (i= 0; i <= inNo; i++) {
           inChn[inIdx[i]].spike_detect(t);  // the inChn decides whether to do anything or not
         }
         for (i= 0; i <= outNo; i++) {
           outChn[outIdx[i]].I= outChnp[outIdx[i]].bias;
         }

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

         // spike generator ... write stuff to DAQ
         // outChn[1].I= inChn[inIdx[inNo]].V;

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
         for (i= 0; i < inNo; i++)
             if( inChn[inIdx[i]].V > vHiLim[i] || inChn[inIdx[i]].V < vLoLim[i])
                  if ( limitWarningEmitted == false){
                        emit CloseToLimit(QString("Voltage"), inIdx[i], inChnp[inIdx[i]].minVoltage, inChnp[inIdx[i]].maxVoltage, inChn[inIdx[i]].V);
                        limitWarningEmitted = true;
                    }

          for (i= 0; i < outNo; i++) {
              if( outChn[outIdx[i]].I > iHiLim[i] ) {
                outChn[outIdx[i]].I = iHiLim[i];
                if ( limitWarningEmitted == false){
                        emit CloseToLimit(QString("Current"), outIdx[i], outChnp[outIdx[i]].minCurrent, outChnp[outIdx[i]].maxCurrent, outChn[outIdx[i]].I);
                        limitWarningEmitted = true;
                }
              }
              if( outChn[outIdx[i]].I < iLoLim[i] ) {
                 outChn[outIdx[i]].I = iLoLim[i];
                 if ( limitWarningEmitted == false){
                        emit CloseToLimit(QString("Current"), outIdx[i], outChnp[outIdx[i]].minCurrent, outChnp[outIdx[i]].maxCurrent, outChn[outIdx[i]].I);
                        limitWarningEmitted = true;
                 }
              }

           }

     // --- Calculate end --- //

     // copy AEC compensated input values to output channels if desired
     for ( int k=0; k<aecChannels.size(); k++ ){
         if ( aecChannels[k]->IsActive() && elecCalibPs[k].copyChnOn)
             outChn[elecCalibPs[k].copyChn].I= inChn[aecChannels[k]->inChnNum].V;
     }

     // --- Write --- //
         board->write_analog_out(outChn);
     // --- Write end --- //


   }
   // Dynamic clamp loop ends


   // In the end, let's do the cleanup
   for (i= 0; i < outNo; i++) outChn[outIdx[i]].I = 0.0;
   board->reset_board();
   for ( int i=0; i<aecChannels.size(); i++ ) if ( aecChannels[i]->IsActive() ) aecChannels[i]->ResetChannel();

   if (dataSavingPs.enabled) {
       dataSaver->EndDataSaving();
   }

   finished= true;

} 


// Completes the sampling cycle by waiting till the end of the sampling period
// Not used for adaptive sampling
double DCThread::WaitTillNextSampling(double time)
{
    double t = 0.0;

    do
    {
        t += board->get_RTC();
    } while (t < time);

    return t;
}


inChannel *DCThread::getInChan(int idx)
{
    ChannelIndex dex(idx, true);
    if ( dex.isPrototype ) {
        return nullptr;
    } else if ( dex.isVirtual ) {
        switch ( dex.modelClass ) {
        case ChannelIndex::HH:
            return &(hhNeuron[dex.modelID].inst[dex.instID].in);
        }
    } else if ( dex.isSG ) {
        return &inChn[inIdx[inNo]];
    } else if ( idx >= 0 ) {
        return &inChn[inIdx[idx]];
    }
    return nullptr;
}

outChannel *DCThread::getOutChan(int idx)
{
    ChannelIndex dex(idx, false);
    if ( dex.isPrototype ) {
        return nullptr;
    } else if ( dex.isVirtual ) {
        switch ( dex.modelClass ) {
        case ChannelIndex::HH:
            return &(hhNeuron[dex.modelID].inst[dex.instID].out);
        }
    } else if ( dex.isNone ) {
        return &outChn[outIdx[outNo]];
    } else if ( idx >= 0 ) {
        return &outChn[outIdx[idx]];
    }
    return nullptr;
}

std::vector<std::pair<int, bool>> DCThread::getChanIndices(int index)
{
    ChannelIndex dex(index);
    if ( dex.isPrototype ) {
        std::vector<std::pair<int, bool>> ret;
        size_t i = 0;
        switch ( dex.modelClass ) {
        case ChannelIndex::HH:
            for ( vInstData &vc : HHNeuronp[dex.modelID].inst ) {
                ret.push_back(make_pair(dex.toInstance(i++), vc.active));
            }
            break;
        }
        return ret;
    } else {
        return std::vector<std::pair<int, bool>>(1, make_pair(index, true));
    }
}

template <class T>
void DCThread::instantiate(std::vector<T> &inst, typename T::param_type &p, CurrentAssignment &a)
{
    CurrentAssignment tmp;
    tmp.active = true;
    if ( a.VChannel == a.IChannel ) {
        // Input/Output on the same model => connect instances 1-to-1 rather than all-to-all
        for ( std::pair<int, bool> VIChan : getChanIndices(a.VChannel) ) {
            if ( !VIChan.second )
                continue;
            tmp.VChannel = VIChan.first;
            tmp.IChannel = VIChan.first;
            inst.push_back(T(&p, this, &tmp));
        }
    } else {
        for ( std::pair<int, bool> VChan : getChanIndices(a.VChannel) ) {
            if ( !VChan.second )
                continue;
            tmp.VChannel = VChan.first;
            for ( std::pair<int, bool> IChan : getChanIndices(a.IChannel) ) {
                if ( !IChan.second )
                    continue;
                tmp.IChannel = IChan.first;
                inst.push_back(T(&p, this, &tmp));
            }
        }
    }
}

template <typename T>
void DCThread::instantiate(std::vector<T> &inst, typename T::param_type &p, SynapseAssignment &a)
{
    SynapseAssignment tmp;
    tmp.active = true;
    if ( a.PostSynChannel == a.OutSynChannel ) {
        for ( std::pair<int, bool> post : getChanIndices(a.PostSynChannel) ) {
            if ( !post.second )
                continue;
            tmp.PostSynChannel = post.first;
            tmp.OutSynChannel = post.first;
            for ( std::pair<int, bool> pre : getChanIndices(a.PreSynChannel) ) {
                if ( !pre.second )
                    continue;
                tmp.PreSynChannel = pre.first;
                inst.push_back(T(&p, this, &tmp));
            }
        }
    } else {
        for ( std::pair<int, bool> post : getChanIndices(a.PostSynChannel) ) {
            if ( !post.second )
                continue;
            tmp.PostSynChannel = post.first;
            for ( std::pair<int, bool> out : getChanIndices(a.OutSynChannel) ) {
                if ( !out.second )
                    continue;
                tmp.OutSynChannel = out.first;
                for ( std::pair<int, bool> pre : getChanIndices(a.PreSynChannel) ) {
                    if ( !pre.second )
                        continue;
                    tmp.PreSynChannel = pre.first;
                    inst.push_back(T(&p, this, &tmp));
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
    tmp.active = true;
    if ( a.postInChannel == a.postOutChannel ) {
        for ( std::pair<int, bool> post : getChanIndices(a.postInChannel) ) {
            if ( !post.second )
                continue;
            tmp.postInChannel = post.first;
            tmp.postOutChannel = post.first;
            vec.push_back(tmp);
        }
    } else {
        for ( std::pair<int, bool> in : getChanIndices(a.postInChannel) ) {
            if ( !in.second )
                continue;
            tmp.postInChannel = in.first;
            for ( std::pair<int, bool> out : getChanIndices(a.postOutChannel) ) {
                if ( !out.second )
                    continue;
                tmp.postOutChannel = out.first;
                vec.push_back(tmp);
            }
        }
    }

    if ( a.preInChannel == a.preOutChannel ) {
        for ( std::pair<int, bool> pre : getChanIndices(a.preInChannel) ) {
            if ( !pre.second )
                continue;
            for ( GapJunctionAssignment &tmp : vec ) {
                tmp.preInChannel = pre.first;
                tmp.preOutChannel = pre.first;
                inst.push_back(T(&p, this, &tmp));
            }
        }
    } else {
        for ( std::pair<int, bool> in : getChanIndices(a.preInChannel) ) {
            if ( !in.second )
                continue;
            for ( std::pair<int, bool> out : getChanIndices(a.preOutChannel) ) {
                if ( !out.second )
                    continue;
                for ( GapJunctionAssignment &tmp : vec ) {
                    tmp.preInChannel = in.first;
                    tmp.preOutChannel = out.first;
                    inst.push_back(T(&p, this, &tmp));
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
    auto const& it = AP::find(rawname);
    if ( it != *params.end() ) {
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


