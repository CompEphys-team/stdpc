#include "HH.h"
#include "DCThread.h"
#include "HHDlg.h"

static HHProxy prox;
std::vector<mhHHData> HHProxy::p;
HHProxy *HH::proxy() const { return &prox; }
ConductanceDlg *HHProxy::createDialog(size_t condID, QWidget *parent) { return new HHDlg(condID, parent); }
IonicCurrent *HHProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new HH(conductanceID, assignID, multiID, in, out);
}

HHProxy::HHProxy()
{
    ConductanceManager::RegisterCurrent(this);

    addAP("mhHHp[#].active", &p, &mhHHData::active);
    addAP("mhHHp[#].activeSettling", &p, &mhHHData::activeSettling);
    addAP("mhHHp[#].LUTables", &p, &mhHHData::LUTables);
    addAP("mhHHp[#].gMax", &p, &mhHHData::gMax);
    addAP("mhHHp[#].Vrev", &p, &mhHHData::Vrev);
    addAP("mhHHp[#].mExpo", &p, &mhHHData::mExpo);
    addAP("mhHHp[#].hExpo", &p, &mhHHData::hExpo);
    addAP("mhHHp[#].Vm", &p, &mhHHData::Vm);
    addAP("mhHHp[#].sm", &p, &mhHHData::sm);
    addAP("mhHHp[#].Cm", &p, &mhHHData::Cm);
    addAP("mhHHp[#].taumType", &p, &mhHHData::taumType);
    addAP("mhHHp[#].taum", &p, &mhHHData::taum);
    addAP("mhHHp[#].taumAmpl", &p, &mhHHData::taumAmpl);
    addAP("mhHHp[#].Vtaum", &p, &mhHHData::Vtaum);
    addAP("mhHHp[#].staum", &p, &mhHHData::staum);
    addAP("mhHHp[#].Vh", &p, &mhHHData::Vh);
    addAP("mhHHp[#].sh", &p, &mhHHData::sh);
    addAP("mhHHp[#].Ch", &p, &mhHHData::Ch);
    addAP("mhHHp[#].tauhType", &p, &mhHHData::tauhType);
    addAP("mhHHp[#].tauh", &p, &mhHHData::tauh);
    addAP("mhHHp[#].tauhAmpl", &p, &mhHHData::tauhAmpl);
    addAP("mhHHp[#].Vtauh", &p, &mhHHData::Vtauh);
    addAP("mhHHp[#].stauh", &p, &mhHHData::stauh);
    addAP("mhHHp[#].assign[#].active", &p, &mhHHData::assign, &CurrentAssignment::active);
    addAP("mhHHp[#].assign[#].VChannel", &p, &mhHHData::assign, &CurrentAssignment::VChannel);
    addAP("mhHHp[#].assign[#].IChannel", &p, &mhHHData::assign, &CurrentAssignment::IChannel);
    addAP("mhHHp[#].assign[#].save", &p, &mhHHData::assign, &CurrentAssignment::save);

    addAP("mhHHp[#].VChannel", &p, &mhHHData::legacy_V);
    addAP("mhHHp[#].IChannel", &p, &mhHHData::legacy_I);
}


HH::HH(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    IonicCurrent(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment()),
    m(0.0),
    h(0.9),
    minf(0.0),
    taum(p->taum),
    hinf(0.9),
    tauh(p->tauh)
{
    if (p->LUTables) {
      theExp= &expLU;
      expLU.require(-1.0, 0.0, 0.001);
      theTanh= &tanhLU;
      tanhLU.require(-50.0, 50.0, 0.02);
      theExpSigmoid= &expSigmoidLU;
      expSigmoidLU.require(-50.0, 50.0, 0.02);
    }
    else {
      theExp= &expFunc;
      theTanh= &tanhFunc;
      theExpSigmoid= &expSigmoidFunc;
    }

    mi = m;
    hi = h;
}

void HH::step(double, double dt, bool settling)
{
  static double powm, powh;
  static double V, tmp;
  static int i;
    
  if (p->active && a->active && in->active && out->active) {
    V= in->V;
    if (p->mExpo > 0) {
      // linear Euler:
      // m+= (minf-m)*dt/taum;   // calculate m with previous values of minf, m, taum
      // exponential Euler:
      m= minf + (m - minf)*(*theExp)(-dt/taum);
      minf= (1.0-p->Cm)*(*theExpSigmoid)((V-p->Vm)/p->sm)+p->Cm;
      if (p->taumType == 0) {
          taum= p->taum - p->taumAmpl*(*theExpSigmoid)((V-p->Vtaum)/p->staum);
      } else if (p->taumType == 1) {
          tmp= (*theTanh)((V-p->Vtaum)/p->staum);
          taum= p->taum+p->taumAmpl*(1.0-tmp*tmp);
      } else if (p->taumType == 2) {
          tmp= (*theExp)((V-p->Vtaum)/p->staum);
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
          tmp= (*theTanh)((V-p->Vtauh)/p->stauh);
          tauh= p->tauh+p->tauhAmpl*(1.0-tmp*tmp);
      } else if (p->tauhType == 2) {
          tmp= (*theExp)((V-p->Vtauh)/p->stauh);
          tauh= p->tauh + p->tauhAmpl*tmp*hinf;
      } else if (p->tauhType == 3) {
          tauh = p->tauh + p->tauhAmpl*V*1e3;
      }
      powh= h;
      for(i= 0; i < p->hExpo-1; i++) powh*= h;   
    }
    else powh = 1.0;

    m_conductance = p->gMax * powm * powh;
    if ( !settling || p->activeSettling )
        out->I += m_conductance * (p->Vrev - V);
  } else {
      m_conductance = 0;
  }
}

void HH::RK4(double, double dt, size_t n, bool settling)
{
    static double powm, powh;
    static double V, tmp;
    static int i;

    if (p->active && a->active && in->active && out->active) {
      V= in->V;
      if (p->mExpo > 0) {
          minf = (1.0-p->Cm)*(*theExpSigmoid)((V-p->Vm)/p->sm)+p->Cm;
          switch ( p->taumType ) {
          case 0:
              taum = p->taum - p->taumAmpl*(*theExpSigmoid)((V-p->Vtaum)/p->staum);
              break;
          case 1:
              tmp= (*theTanh)((V-p->Vtaum)/p->staum);
              taum= p->taum+p->taumAmpl*(1.0-tmp*tmp);
              break;
          case 2:
              tmp= (*theExp)((V-p->Vtaum)/p->staum);
              taum= p->taum + p->taumAmpl*tmp*minf;
              break;
          case 3:
              taum = p->taum + p->taumAmpl*V*1e3;
              break;
          }
          km[n] = (minf-mi)/taum;

          powm = mi;
          for(i= 0; i < p->mExpo-1; i++) powm*= mi;

          if ( n < 3 ) {
              mi = m + km[n]*dt;
          } else {
              mi = m + (km[0] + 2*km[1] + 2*km[2] + km[3]) * dt / 6;
          }

          if ( mi > 1.0 ) mi = 1.0;
          else if ( mi < 0.0 || std::isnan(mi) ) mi = 0.0;

          if ( n == 3 ) {
              m = mi;
          }
      }
      else powm= 1.0;

      if (p->hExpo > 0){
          hinf = (1.0-p->Ch)*(*theExpSigmoid)((V-p->Vh)/p->sh)+p->Ch;
          switch ( p->tauhType ) {
          case 0:
              tauh = p->tauh - p->tauhAmpl*(*theExpSigmoid)((V-p->Vtauh)/p->stauh);
              break;
          case 1:
              tmp= (*theTanh)((V-p->Vtauh)/p->stauh);
              tauh= p->tauh+p->tauhAmpl*(1.0-tmp*tmp);
              break;
          case 2:
              tmp= (*theExp)((V-p->Vtauh)/p->stauh);
              tauh= p->tauh + p->tauhAmpl*tmp*hinf;
              break;
          case 3:
              tauh = p->tauh + p->tauhAmpl*V*1e3;
              break;
          }
          kh[n] = (hinf-hi)/tauh;

          powh = hi;
          for(i= 0; i < p->hExpo-1; i++) powh*= hi;

          if ( n < 3 ) {
              hi = h + kh[n]*dt;
          } else {
              hi = h + (kh[0] + 2*kh[1] + 2*kh[2] + kh[3]) * dt / 6;
          }

          if ( hi > 1.0 ) hi = 1.0;
          else if ( hi < 0.0 || std::isnan(hi) ) hi = 0.0;

          if ( n == 3 ) {
              h = hi;
          }
      }
      else powh = 1.0;

      m_conductance = p->gMax * powm * powh;
      if ( !settling || p->activeSettling )
          out->I += m_conductance * (p->Vrev - V);
    } else {
        m_conductance = 0;
    }
}
