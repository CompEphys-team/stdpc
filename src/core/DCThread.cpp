#include "DCThread.h"
#include "AP.h"
#include "Global.h"
#include <cmath>
#include <climits>

  
#include "SimulDAQ.h" // for debugging only ...
 
DCThread::DCThread() 
{
  stopped= true;
  finished= true;
  csyn= new ChemSyn[MAX_SYN_NO];
  absyn= new abSyn[MAX_SYN_NO];
  esyn= new GapJunction[MAX_SYN_NO];
  dsyn= new DestexheSyn[MAX_SYN_NO];
  hh= new HH[MAX_HH_NO];
  abhh= new abHH[MAX_HH_NO];
  csIdx= new short int[MAX_SYN_NO];
  absIdx= new short int[MAX_SYN_NO];
  esIdx= new short int[MAX_SYN_NO];
  dsIdx= new short int[MAX_SYN_NO];
  hhIdx= new short int[MAX_HH_NO];
  abhhIdx= new short int[MAX_HH_NO];

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
   delete[] csyn;
   delete[] absyn;
   delete[] esyn;
   delete[] dsyn;
   delete[] hh;
   delete[] abhh;
   delete[] csIdx;
   delete[] absIdx;
   delete[] esIdx;
   delete[] dsIdx;
   delete[] hhIdx;
   delete[] abhhIdx;
   
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
   
   // set up the graphic display channels
   int idx;
   QString lb;
   for (i= 0; i < 2; i++) {
     grpNo[i]= 0;
     for (int j= 0; j < 4; j++) {
       if (Graphp[i].active[j]) {
         idx= Graphp[i].chn[j];
         if (idx == inNo+outNo+1) { // SG
           grp[i][grpNo[i]]= &(SG.V);
           pen[i][grpNo[i]++]= j;
         } else {
           if (idx > inNo) { // we are doing an out channel
             grp[i][grpNo[i]]= &(outChn[outIdx[idx-inNo-1]].I);
             pen[i][grpNo[i]++]= j;
           }
           else { // we are doing an in channel  
             grp[i][grpNo[i]]= &(inChn[inIdx[idx-1]].V);
             pen[i][grpNo[i]++]= j;
           }
         }
       }             
     }
     lb.setNum(grpNo[i]);
     message(QString("Added ")+lb+QString(" channels for Display"));
   }
                 
   csNo= 0;
   absNo= 0;
   esNo= 0;
   dsNo= 0;
   for (i= 0; i < MAX_SYN_NO; i++) {
     if (CSynp[i].active) {
       csyn[i].init(&CSynp[i],inIdx,outIdx,inChn,outChn);
       csIdx[csNo++]= i;
     }
     if (abSynp[i].active) {
       absyn[i].init(&abSynp[i],inIdx,outIdx,inChn,outChn);
       absIdx[absNo++]= i;
     }
     if (ESynp[i].active) {
       esyn[i].init(&ESynp[i],inIdx,outIdx,inChn,outChn);
       esIdx[esNo++]= i;
     }
     if (DxheSynp[i].active) {
       dsyn[i].init(&DxheSynp[i],inIdx,outIdx,inChn,outChn);
       dsIdx[dsNo++]= i;
     }
   }
   hhNo= 0;
   abhhNo= 0;
   for (i= 0; i < MAX_HH_NO; i++) {
     if (mhHHp[i].active) {
       hh[i].init(&mhHHp[i],inIdx,outIdx,inChn,outChn);
       hhIdx[hhNo++]= i;
     }
     if (abHHp[i].active) {
       abhh[i].init(&abHHp[i],inIdx,outIdx,inChn,outChn);
       abhhIdx[abhhNo++]= i;
     }
   }
   QString cN, abN, eN, dN, hN, aN;
   cN.setNum(csNo);
   abN.setNum(absNo);
   eN.setNum(esNo);
   dN.setNum(dsNo);
   hN.setNum(hhNo);
   aN.setNum(abhhNo);
   if (csNo > 0) message(QString("DynClamp: ")+cN+QString(" chemical synapse(s) "));
   if (absNo > 0) message(QString("DynClamp: ")+abN+QString(" ab synapse(s) "));
   if (esNo > 0) message(QString("DynClamp: ")+eN+QString(" gap junction(s) "));
   if (dsNo > 0) message(QString("DynClamp: ")+dN+QString(" Destexhe synapse(s) "));
   if (hhNo > 0) message(QString("DynClamp: ")+hN+QString(" HH conductance(s) "));
   if (abhhNo > 0) message(QString("DynClamp: ")+aN+QString(" abHH conductance(s) ..."));

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
     iHiLim[i] = outChnp[i].minCurrent + upTolFac*(outChnp[i].maxCurrent-outChnp[i].minCurrent);
     iLoLim[i] = outChnp[i].minCurrent + loTolFac*(outChnp[i].maxCurrent-outChnp[i].minCurrent);
   }
   for ( int i=0; i<inNo; i++ ){
     vHiLim[i] = inChnp[i].minVoltage + upTolFac*(inChnp[i].maxVoltage-inChnp[i].minVoltage);
     vLoLim[i] = inChnp[i].minVoltage + loTolFac*(inChnp[i].maxVoltage-inChnp[i].minVoltage);
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
     evt= scrIter->t;
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
         // Adaptive time step
         dt = board->get_RTC();
         t += dt;
     }

     //--------------- Data saving ------------------------//
         if (dataSavingPs.enabled) {
           if ( t >= lastSave + savingPeriod )
           {
              data[0] = t;

              for (i= 0; i < inChnsToSave.size(); i++)   data[i+1] = inChn[inChnsToSave[i]].V;    // voltages
              if ( SGp.saving ) data[inChnsToSave.size()+1] = SG.V; // spike generator
              for (i= 0; i < outChnsToSave.size(); i++)  data[inChnsToSave.size()+1+SGp.saving+i] = outChn[outChnsToSave[i]].I;    // currents

            #ifdef TEST_VERSION
              int aecChannelNum = 0;
              for ( int i=0; i<aecChannels.size(); i++ )
                  if ( aecChannels[i]->IsActive() )
                  {
                      data[inChnsToSave.size()+1+SGp.saving+outChnsToSave.size()+aecChannelNum] = aecChannels[i]->v_e;
                      aecChannelNum++;
                  }
            #endif

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
         for (i= 0; i < csNo; i++)
           csyn[csIdx[i]].currentUpdate(t, dt);
         for (i= 0; i < absNo; i++)
           absyn[absIdx[i]].currentUpdate(t, dt);
         for (i= 0; i < esNo; i++)
           esyn[esIdx[i]].currentUpdate(t, dt);
         for (i= 0; i < dsNo; i++)
           dsyn[dsIdx[i]].currentUpdate(t, dt);
         for (i= 0; i < hhNo; i++)
           hh[hhIdx[i]].currentUpdate(t, dt);
         for (i= 0; i < abhhNo; i++)
           abhh[abhhIdx[i]].currentUpdate(t, dt);
         // spike generator ... write stuff to DAQ
         // outChn[1].I= inChn[inIdx[inNo]].V;

         // Updated display
         if (Graphp[0].active) {
             if (t-lastWrite[0] > Graphp[0].dt) {
               lastWrite[0]= t;
               for (i= 0; i < grpNo[0]; i++) {
                 addPoint1(t, *grp[0][i], pen[0][i]);
               }
             }
         }
         if (Graphp[1].active) {
             if (t-lastWrite[1] > Graphp[1].dt) {
               lastWrite[1]= t;
               for (i= 0; i < grpNo[1]; i++) {
                 addPoint2(t, *grp[1][i], pen[1][i]);
               }
             }
         }

         // Scripting
         if (t >= evt) { // event needs doing
           switch (scrIter->type) {
             case DBLTYPE: *dAP[scrIter->index]= *((double *) scrIter->value); break;
             case INTTYPE: *iAP[scrIter->index]= *((int *) scrIter->value); break;
             case BOOLTYPE: *bAP[scrIter->index]= *((bool *) scrIter->value); break;
             case STRTYPE: *sAP[scrIter->index]= *((QString *) scrIter->value); break;
	     case SHORTINTTYPE: *siAP[scrIter->index]= *((short int *) scrIter->value); break;
           }
           scrIter++;
           if (scrIter == scriptq.end()) evt= 1e10;
           else {
             evt= scrIter->t;
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


// Scripting support

bool DCThread::LoadScript(QString &fname)
{
  double et; 
  QString pname;
  double dvalue;
  int ivalue, index;
  short int sivalue;
  bool bvalue, done, success;
  QString svalue, qn, vn;
  scriptInstruction inst;
  char buf[80];
  
  ifstream is(fname.toAscii());
     
  is >> et;
  done= false;
  success= true;
  while (!done)
  {
    is >> buf;
    pname= QString(buf);
    index= APname.indexOf(pname);
    if (index == -1) { 
      success= false;
    }
    else {
      switch (APtype[index]) {
        case DBLTYPE: is >> dvalue; break;
        case INTTYPE: is >> ivalue; break;
        case BOOLTYPE: is >> bvalue; break;
        case STRTYPE: is >> buf; svalue= QString(buf); break;
        case SHORTINTTYPE: is >> sivalue; break;
      }
      if (is.good()) {
        switch (APtype[index]) {
          case DBLTYPE: inst.set(et, APtype[index], APindex[index], &dvalue); break;
          case INTTYPE: inst.set(et, APtype[index], APindex[index], &ivalue); break;
          case BOOLTYPE: inst.set(et, APtype[index], APindex[index], &bvalue); break;
          case STRTYPE: inst.set(et, APtype[index], APindex[index], &svalue); break;
          case SHORTINTTYPE: inst.set(et, APtype[index], APindex[index], &sivalue); break;
         }
        scriptq.append(inst);
      }
      else success= false;
    }
    is >> et;
    if ((!is.good()) || (!success)) done= true;
  }
  if (!success) {
    scriptq.clear();
    scripting= false;
    qn.setNum(et);
    message(QString("error in script file at instruction \"")+qn+QString(" ")+pname+QString("\""));
    message(QString("script not loaded"));
  }
  else {
    qn.setNum(scriptq.size());
    message(QString("script read with ")+qn+QString(" instructions"));
    scripting= true;
/*    for (scrIter= scriptq.begin(); scrIter != scriptq.end(); scrIter++) {
      qn.setNum((*scrIter).t);
      vn.setNum(*((double *) ((*scrIter).value)));
      message(QString("t= ")+qn+QString(" something with value ")+vn);
    }
*/  }
  return success;
}

void DCThread::UnloadScript()
{
  scriptq.clear();
  scripting= false;
  message(QString("script unloaded"));
}


