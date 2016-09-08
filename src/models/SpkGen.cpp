#include <cassert>
#include "SpkGen.h"
#include "DCThread.h"
#include <cmath>
 
#define SGOFF 0
#define EXPLICITSG 1
#define FILESG 2
#define VSG 3
// time after spike expired to read a new one from file (file SpkGen w/o burst detect)
#define ISI_TIME_MAX 0.005

SpkGen::SpkGen()
{
  lastRead= QString("");
  inSpkGenChnp.active= true;
  inSpkGenChnp.gain= 0;
  inSpkGenChnp.gainFac= 1.0;
  inSpkGenChnp.spkDetect= true;
  inSpkGenChnp.spkDetectThresh= 0.0;
  outSpkGenChnp.active= true;
  outSpkGenChnp.gain= 0;
  outSpkGenChnp.gainFac= 1.0;
}


SpkGen::~SpkGen() 
{
}

void SpkGen::init(SGData *inp, inChannel *inChn, DCThread *dcthr)
{
  QString buf;
  double st, t, V;
  int sn;
     
  p= inp;
  VChn= inChn;
  bdChn= dcthr->getInChan(p->bdChannel);
  if (p->LUTables) {
    theExp= &expLU;
    expLU.require(-50.0, 50.0, 0.02);
    theTanh= &tanhLU;
    tanhLU.require(-50.0, 50.0, 0.02);
  }
  else {
    theExp= &expFunc;
    theTanh= &tanhFunc;
  }

  if (p->method == VSG) {
    if (lastRead != p->STInFName) {
      if (is.is_open()) is.close();
      is.clear();
      is.open(p->STInFName.toLatin1());
      if (!is.good()) {
        p->active= false;    
        lastRead= QString("");
        message(QString("Voltage data for spike generator not found"));
      }
      else {  
        snq.clear();
        stq.clear();
        tq.clear();         
        Vq.clear();
        is >> t;
        while (is.good()) {
          is >> V;
          tq.append(t);
          Vq.append(V);
          is >> t;
        }
        lastRead= p->STInFName;
      }
    }
  }
  if (p->method == FILESG) {
    if (is.is_open()) is.close();
    is.clear();
    is.open(p->STInFName.toLatin1());
    if (!is.good()) {
      p->active= false;    
    }
    else {
      if (p->bdType == 0) { //no burst detection
        stq.clear(); 
        is >> st;
        while (is.good()) {
          stq.append(st);
          is >> st;
        }
        burstSpikeNo= 10;
        stIter= stq.begin();
        for (int i= 0; i < burstSpikeNo; i++) {
          p->SpikeT[i]= *stIter;
          stIter++;
        }
        buf.setNum(stq.size());
        message(QString("SpikeGen: Loaded ") + buf + QString(" spike times "));
      }
      else {
        stq.clear();
        snq.clear();
        is >> sn;
        while (is.good()) {
          snq.append(sn);
          for (int i= 0; i < sn; i++) {
            is >> st;
            stq.append(st);
          }
          is >> sn;
        }
        snIter= snq.begin();
        stIter= stq.begin();
        buf.setNum(snq.size());
        message(QString("SpikeGen: Loaded ") + buf + QString(" bursts "));
        burstSpikeNo= 0;
        readSpikeNo= 0;
      }
    }
  }
  else {
    burstSpikeNo= p->SpikeNo;
    readSpikeNo= burstSpikeNo;
  }
  ISI_time= 0.0;
  nOverThresh= 0;
  nUnderThresh= 0;
  burstDetected= false;
  if (p->bdType == 0) SGactive= true;
  else SGactive= false;
  initial= true;
  is.close();
}

void SpkGen::VUpdate(double t, double dt)
{
  static int i;
  
  if (initial) {
    if (p->method == VSG) {
      titer= tq.begin();
      Viter= Vq.begin();
    }
    ISI_time= t;
    initial= false;
  }
  else {
    if (p->method != VSG) ISI_time+= dt;
  }
  
  if (p->method == VSG) { // voltage from file
    while ((titer != tq.end()) && (*titer < t-ISI_time)) {
      titer++;
      Viter++;
    }
    if (titer == tq.end()) {
      p->active= false;
      message(QString("Voltage data exhausted ... stopping Generator"));
      return;
    }         
  }
  else {
    if (p->bdType > 0) { // taking care of the burst detection options
      if(!SGactive) { // burst detection hasn't been completed yet ... spike generation inactive
        if (burstDetected) { // burst (first crossing) was detected, waiting for opposite crossing
          if (readSpikeNo < burstSpikeNo) {
            if (stIter == stq.end()) {
              p->active= false;
              message(QString("Spike time data exhausted ... stopping spike Generator"));
              return;
            }
            p->SpikeT[readSpikeNo++]= *stIter;
            stIter++;
            if (readSpikeNo == burstSpikeNo) {
              p->period= p->SpikeT[burstSpikeNo-1]+(20.0/p->spkTimeScaling);
            }
          }
          if (((p->bdType == 1) && (bdChn->V > p->bdThresh)) || 
              ((p->bdType == 2) && (bdChn->V < p->bdThresh))) {
            nUnderThresh++;     // "under" and "over" only phrases for first and second cond.
            if (nUnderThresh > p->bdNUnder) {
              SGactive= true;
              nOverThresh= 0;
              burstDetected= false;
             ISI_time= 0.0;
            }
          }
        }
        else {  // burst (first crossing) not yet detected ...
          if (((p->bdType == 1) && (bdChn->V < p->bdThresh)) || 
              ((p->bdType == 2) && (bdChn->V > p->bdThresh))) {
            nOverThresh++;
            if (nOverThresh > p->bdNOver) {
              nUnderThresh= 0;
              burstDetected= true;
              if (p->method == FILESG) {
                if (snIter == snq.end()) {
                  p->active= false;
                  message(QString("Spike time data exhausted ... stopping spike Generator"));
                  return;
                }
                else {
                  burstSpikeNo= *snIter;
                  snIter++;
                  readSpikeNo= 0;
                }
              }
            }
          }
        }
      }
    }
  }
  // calculating SpkGen voltage for all cases:
                 
  if (SGactive) {// the spike generation is active
// routine that calculates the membrane potential of the CN taking
// in account all the spikes (superimposing) the voltage due to each spike
    if (p->method == VSG) {
      V= *Viter;
      assert(Viter != Vq.end());
    }
    else {
      V= 0.0;
      for (i= 0; i < burstSpikeNo; i++) {
        V+= VSpike((ISI_time - p->SpikeT[i])*p->spkTimeScaling);
      }
      V*= p->VSpike;
      V+= p->VRest;

      if (ISI_time > p->period) {
        if (p->bdType > 0) {
          SGactive= false;
        }
        else {
          if (p->method == EXPLICITSG) ISI_time-= p->period;
        }
      }                             
    }
  }
  else {
    V= p->VRest;
  }
  VChn->V= V;
  
  if ((p->method == FILESG) && (p->bdType == 0)) {
    if ((ISI_time - p->SpikeT[0]) > ISI_TIME_MAX) {
      for (i= 0; i < burstSpikeNo-1; i++) {
        p->SpikeT[i]=p->SpikeT[i+1];
      }
      if (stIter == stq.end()) {
        burstSpikeNo--;
        if (burstSpikeNo == 0) {
          p->active= false;
          message(QString("Spike time data exhausted ... stopping spike Generator"));
        }
      }
      else {
        p->SpikeT[burstSpikeNo-1]= *stIter;
        stIter++;
      }
    }
  }
}

// how to generate a spike

double SpkGen::VSpike(double t){
  #define A 1.0
  #define B 4.0
  #define TAU1 4.0
  #define TAU2 4.0
  #define T0 -0.576
  #define A1 2.0
  #define A2 2.0
  #define NORMALIZE 3.25394

  double f1, f2, f_spike;
  
  if ((t-T0)/TAU1 < 100.0)
    f1= (((*theTanh)(A1*(T0-t))+1.0)/2.0)*(*theExp)((t-T0)/TAU1);
  else f1= 0.0;

  if ((T0-t)/TAU2 < 100.0)
    f2= (((*theTanh)(A2*(t-T0))+1.0)/2.0)*(*theExp)((T0-t)/TAU2);
  else f2= 0.0;

  f_spike= (A*f1 + B*f2)/NORMALIZE;
  return(f_spike);

  #undef A
  #undef B
  #undef TAU1
  #undef TAU2
  #undef T0
  #undef A1
  #undef A2
  #undef NORMALIZE
}


#undef SGOFF
#undef EXPLICITSG
#undef FILESG
