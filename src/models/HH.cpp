#include "HH.h"
#include "DCThread.h"

HH::HH(mhHHData *inp, DCThread *t, CurrentAssignment *a) :
    p(inp),
    m(0.0),
    h(0.9),
    minf(0.0),
    taum(p->taum),
    hinf(0.9),
    tauh(p->tauh)
{
    if ( !a ) {
        pre = t->getInChan(p->VChannel);
        out = t->getOutChan(p->IChannel);
    } else {
        pre = t->getInChan(a->VChannel);
        out = t->getOutChan(a->IChannel);
    }

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
}

void HH::currentUpdate(double t, double dt) 
{
  static double powm, powh;
  static double V, tmp;
  static int i;
    
  if (p->active) {
    V= pre->V;
    if (p->mExpo > 0) {
      // linear Euler:
      // m+= (minf-m)*dt/taum;   // calculate m with previous values of minf, m, taum
      // exponential Euler:
      m= minf + (m - minf)*(*theExp)(-dt/taum);
      minf= (1.0-p->Cm)*(*theExpSigmoid)((V-p->Vm)/p->sm)+p->Cm;
      if (p->taumType == 0) {
          taum= p->taum - p->taumAmpl*(*theExpSigmoid)((V-p->Vtaum)/p->staum);
      } else if (p->taumType == 1) {
          tmp= tanh((V-p->Vtaum)/p->staum);
          taum= p->taum+p->taumAmpl*(1.0-tmp*tmp);
      } else if (p->taumType == 2) {
          tmp= exp((V-p->Vtaum)/p->staum);
          taum= p->taum + p->taumAmpl*tmp*minf;
      } else if (p->taumType == 3) {
          taum = p->taum + p->taumAmpl*V*1e3;
      }

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
      if (p->tauhType == 0) {
          tauh= p->tauh - p->tauhAmpl*(*theExpSigmoid)((V-p->Vtauh)/p->stauh);
      } else if (p->tauhType == 1) {
          tmp= tanh((V-p->Vtauh)/p->stauh);
          tauh= p->tauh+p->tauhAmpl*(1.0-tmp*tmp);
      } else if (p->tauhType == 2) {
          tmp= exp((V-p->Vtauh)/p->stauh);
          tauh= p->tauh + p->tauhAmpl*tmp*hinf;
      } else if (p->tauhType == 3) {
          tauh = p->tauh + p->tauhAmpl*V*1e3;
      }
      powh= h;
      for(i= 0; i < p->hExpo-1; i++) powh*= h;   
    }
    else powh = 1.0;

    I= p->gMax*powm*powh*(p->Vrev - V);
    out->I+= I; 
  }
}