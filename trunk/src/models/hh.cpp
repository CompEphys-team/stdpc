#include "HH.h"

HH::HH()
{
}

void HH::init(mhHHData *inp, short int *inIdx, short int *outIdx, inChannel *inChn, outChannel *outChn)
{
  p= inp;
  pre= &inChn[inIdx[p->VChannel]];
  out= &outChn[outIdx[p->IChannel]];
  if (p->LUTables) {
    theExp= &expLU;
    expLU.require(-1.0, 0.0, 0.001);
    theExpSigmoid= &expSigmoidLU;
    expSigmoidLU.require(-50.0, 50.0, 0.02);
  }
  else {
    theExp= &expFunc;
    theExpSigmoid= &expSigmoidFunc;
  }
  m= 0.0;
  minf= 0.0;
  taum= p->taum;
  h= 0.9;
  hinf= 0.9;
  tauh= p->tauh;
}

void HH::currentUpdate(double t, double dt) 
{
  static double powm, powh;
  static double V;
  static int i;
    
  if (p->active) {
    V= pre->V;
    if (p->mExpo > 0) {
      // linear Euler:
      // m+= (minf-m)*dt/taum;   // calculate m with previous values of minf, m, taum
      // exponential Euler:
      m= minf + (m - minf)*(*theExp)(-dt/taum);
      minf= (1.0-p->Cm)*(*theExpSigmoid)((V-p->Vm)/p->sm)+p->Cm;
      taum= p->taum - p->taumAmpl*(*theExpSigmoid)((V-p->Vtaum)/p->staum);
   
      powm = m;
      for(i= 0; i < p->mExpo-1; i++) powm*= m;
    }
    else powm= 1.0;
    
    if (p->hExpo > 0){
      // linear Euler: 
      // h+= (hinf-h)*dt/tauh;   // calculate h with previous values of hinf, h, tauh
      // exponential Euler:
      h= hinf + (h - hinf)*(*theExp)(-dt/tauh);
      hinf= (1.0-p->Ch)*(*theExpSigmoid)((V-p->Vh)/p->sh)+p->Ch;
      tauh= p->tauh - p->tauhAmpl*(*theExpSigmoid)((V-p->Vtauh)/p->stauh);

      powh= h;
      for(i= 0; i < p->hExpo-1; i++) powh*= h;   
    }
    else powh = 1.0;

    I= p->gMax*powm*powh*(p->Vrev - V);
    out->I+= I; 
  }
}
