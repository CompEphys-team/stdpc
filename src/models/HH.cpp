#include "HH.h"
#include "DCThread.h"

HH::HH(mhHHData *inp, DCThread *t, CurrentAssignment *ina) :
    p(inp),
    a(ina),
    m(0.0),
    h(0.9),
    minf(0.0),
    taum(p->taum),
    hinf(0.9),
    tauh(p->tauh)
{
    if ( !a ) {
        legacy.active = !p->noLegacyAssign;
        legacy.VChannel = p->VChannel;
        legacy.IChannel = p->IChannel;
        a =& legacy;
    }
    pre = t->getInChan(a->VChannel);
    out = t->getOutChan(a->IChannel);

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

void HH::instantiate(std::vector<HH> &instances, mhHHData *inp, DCThread *t, CurrentAssignment *ina)
{
    CurrentAssignment tmp;
    tmp.active = true;
    if ( ina->VChannel == ina->IChannel ) {
        // Input/Output on the same model => connect instances 1-to-1 rather than all-to-all
        for ( std::pair<size_t, bool> VIChan : t->getChanIndices(ina->VChannel) ) {
            if ( !VIChan.second )
                continue;
            tmp.VChannel = VIChan.first;
            tmp.IChannel = VIChan.first;
            instances.push_back(HH(inp, t, &tmp));
        }
    } else {
        for ( std::pair<size_t, bool> VChan : t->getChanIndices(ina->VChannel) ) {
            if ( !VChan.second )
                continue;
            tmp.VChannel = VChan.first;
            for ( std::pair<size_t, bool> IChan : t->getChanIndices(ina->IChannel) ) {
                if ( !IChan.second )
                    continue;
                tmp.IChannel = IChan.first;
                instances.push_back(HH(inp, t, &tmp));
            }
        }
    }
}

void HH::currentUpdate(double t, double dt) 
{
  static double powm, powh;
  static double V, tmp;
  static int i;
    
  if (p->active && a->active) {
    V= pre->V;
    if (p->mExpo > 0) {
      // linear Euler:
      // m+= (minf-m)*dt/taum;   // calculate m with previous values of minf, m, taum
      // exponential Euler:
      m= minf + (m - minf)*(*theExp)(-dt/taum);
      minf= (1.0-p->Cm)*(*theExpSigmoid)((V-p->Vm)/p->sm)+p->Cm;
      if (p->taumType == 0) {
          taum= p->taum - p->taumAmpl*(*theExpSigmoid)((V-p->Vtaum)/p->staum);
      }
      if (p->taumType == 1) {
          tmp= tanh((V-p->Vtaum)/p->staum);
          taum= p->taum+p->taumAmpl*(1.0-tmp*tmp);
      }
      if (p->taumType == 2) {
          tmp= exp((V-p->Vtaum)/p->staum);
          taum= p->taum + p->taumAmpl*tmp*minf;
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
      }
      if (p->tauhType == 1) {
          tmp= tanh((V-p->Vtauh)/p->stauh);
          tauh= p->tauh+p->tauhAmpl*(1.0-tmp*tmp);
      }
      if (p->tauhType == 2) {
          tmp= exp((V-p->Vtauh)/p->stauh);
          tauh= p->tauh + p->tauhAmpl*tmp*hinf;
      }
      powh= h;
      for(i= 0; i < p->hExpo-1; i++) powh*= h;   
    }
    else powh = 1.0;

    I= p->gMax*powm*powh*(p->Vrev - V);
    out->I+= I; 
  }
}
