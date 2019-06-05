/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "AbHH.h"
#include "DCThread.h"
#include "AlphaBetaHHDlg.h"

static abHHProxy *prox = abHHProxy::get();
std::vector<abHHData> abHHProxy::p;
abHHProxy *abHH::proxy() const { return prox; }
ConductanceDlg *abHHProxy::createDialog(size_t condID, QWidget *parent) { return new AlphaBetaHHDlg(condID, parent); }
IonicCurrent *abHHProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new abHH(conductanceID, assignID, multiID, in, out);
}

abHHProxy::abHHProxy()
{
    ConductanceManager::RegisterCurrent(this);

    addAP("abHHp[#].active", &p, &abHHData::active);
    addAP("abHHp[#].activeSettling", &p, &abHHData::activeSettling);
    addAP("abHHp[#].label", &p, &abHHData::label);
    addAP("abHHp[#].LUTables", &p, &abHHData::LUTables);
    addAP("abHHp[#].gMax", &p, &abHHData::gMax);
    addAP("abHHp[#].Vrev", &p, &abHHData::Vrev);
    addAP("abHHp[#].mExpo", &p, &abHHData::mExpo);
    addAP("abHHp[#].hExpo", &p, &abHHData::hExpo);
    addAP("abHHp[#].maFunc", &p, &abHHData::maFunc);
    addAP("abHHp[#].mka", &p, &abHHData::mka);
    addAP("abHHp[#].mVa", &p, &abHHData::mVa);
    addAP("abHHp[#].msa", &p, &abHHData::msa);
    addAP("abHHp[#].mbFunc", &p, &abHHData::mbFunc);
    addAP("abHHp[#].mkb", &p, &abHHData::mkb);
    addAP("abHHp[#].mVb", &p, &abHHData::mVb);
    addAP("abHHp[#].msb", &p, &abHHData::msb);
    addAP("abHHp[#].haFunc", &p, &abHHData::haFunc);
    addAP("abHHp[#].hka", &p, &abHHData::hka);
    addAP("abHHp[#].hVa", &p, &abHHData::hVa);
    addAP("abHHp[#].hsa", &p, &abHHData::hsa);
    addAP("abHHp[#].hbFunc", &p, &abHHData::hbFunc);
    addAP("abHHp[#].hkb", &p, &abHHData::hkb);
    addAP("abHHp[#].hVb", &p, &abHHData::hVb);
    addAP("abHHp[#].hsb", &p, &abHHData::hsb);
    addAP("abHHp[#].assign[#].active", &p, &abHHData::assign, &CurrentAssignment::active);
    addAP("abHHp[#].assign[#].VChannel", &p, &abHHData::assign, &CurrentAssignment::VChannel);
    addAP("abHHp[#].assign[#].IChannel", &p, &abHHData::assign, &CurrentAssignment::IChannel);
    addAP("abHHp[#].assign[#].save", &p, &abHHData::assign, &CurrentAssignment::save);

    addAP("abHHp[#].VChannel", &p, &abHHData::legacy_V);
    addAP("abHHp[#].IChannel", &p, &abHHData::legacy_I);
}


abHH::abHH(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    IonicCurrent(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment()),
    m(0.0),
    h(1.0),
    ma(0.0),
    mb(1.0),
    ha(0.0),
    hb(0.0)
{
    if (p->LUTables) {
        theExp= &expLU;
        expLU.require(-50.0, 50.0, 0.02);
        theExpSigmoid= &expSigmoidLU;
        expSigmoidLU.require(-50.0, 50.0, 0.02);
        theTanh= &tanhLU;
        tanhLU.require(-10.0, 10.0, 0.01);
    }
    else {
        theExp= &expFunc;
        theExpSigmoid= &expSigmoidFunc;
        theTanh= &tanhFunc;
    }

    // Can't do this in the initialiser list, whyeverthefuck
    mi = m;
    hi = h;
}

inline double abHH::mhFunc(double x, int thetype)
{
   switch (thetype) {
    case 0:
         return x/((*theExp)(x)-1.0);
    case 1:
         return (*theExp)(x);
    case 2:
         return (*theExpSigmoid)(x);
   }
   return 0.0;
}

void abHH::step(double, double dt, bool settling)
{
  static double powm, powh;
  static double V;
  static int i;
  
  if (p->active && a->active && in->active && out->active) {
    V= in->V;
    if (p->mExpo > 0) {
      // Linear Euler:
      m+= (ma*(1.0-m)-mb*m)*dt;    // calculate m with previous values of ma, m, mb
      ma= p->mka*mhFunc((V-p->mVa)/p->msa, p->maFunc);
      mb= p->mkb*mhFunc((V-p->mVb)/p->msb, p->mbFunc);
      powm= m;
      for (i= 0; i < p->mExpo-1; i++) powm*= m;
    }
    else powm= 1.0;
    
    if(p->hExpo > 0) {
      // Linear Euler:
      h+= (ha*(1.0-h)-hb*h)*dt;     // calculate h with previous values of ha, h, hb
      ha= p->hka*mhFunc((V-p->hVa)/p->hsa, p->haFunc);
      hb= p->hkb*mhFunc((V-p->hVb)/p->hsb, p->hbFunc);
      powh= h;
      for (i= 0; i < p->hExpo-1; i++) powh*= h;
    }
    else powh= 1.0;

    m_conductance = p->gMax * powm * powh;
    if ( !settling || p->activeSettling )
        out->I += m_conductance * (p->Vrev-V);
  } else {
      m_conductance = 0;
  }
}

void abHH::RK4(double, double dt, size_t n, bool settling)
{
    static double powm, powh;
    static double V;
    static int i;

    if (p->active && a->active && in->active && out->active) {
      V = in->V;
      if (p->mExpo > 0) {
          /// Calculate km[n]=dm/dt based on the previous intermediates (Vi, mi)
          ma= p->mka*mhFunc((V-p->mVa)/p->msa, p->maFunc);
          mb= p->mkb*mhFunc((V-p->mVb)/p->msb, p->mbFunc);
          km[n] = (ma*(1.0-mi)-mb*mi);

          /// Calculate output based on (still) the previous intermediate mi (to allow calculating kV[n]=dV/dt)
          powm = mi;
          for (i= 0; i < p->mExpo-1; i++) powm*= mi;

          /// Create a new intermediate mi(="m_n") based on km[n]=dm/dt and m0, the initial value of m in this RK cycle
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

      if(p->hExpo > 0) {
          ha= p->hka*mhFunc((V-p->hVa)/p->hsa, p->haFunc);
          hb= p->hkb*mhFunc((V-p->hVb)/p->hsb, p->hbFunc);
          kh[n] = ha*(1.0-hi)-hb*hi;

          powh = hi;
          for (i= 0; i < p->hExpo-1; i++) powh*= hi;

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
      else powh= 1.0;

      m_conductance = p->gMax * powm * powh;
      if ( !settling || p->activeSettling )
          out->I += m_conductance * (p->Vrev-V);
    } else {
        m_conductance = 0;
    }
}
