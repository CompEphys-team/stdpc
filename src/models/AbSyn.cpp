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

#include "AbSyn.h"
#include <cmath>
#include "DCThread.h"
#include "AbSynDlg.h"

static abSynProxy *prox = abSynProxy::get();
std::vector<abSynData> abSynProxy::p;
abSynProxy *abSyn::proxy() const { return prox; }
ConductanceDlg *abSynProxy::createDialog(size_t condID, QWidget *parent) { return new abSynDlg(condID, parent); }
Synapse *abSynProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *DCT,
                                    inChannel *pre, inChannel *post, outChannel *out) {
    return new abSyn(conductanceID, assignID, multiID, DCT, pre, post, out);
}

abSynProxy::abSynProxy()
{
    ConductanceManager::RegisterSynapse(this);
    // abSynp STPlast
    addAP("abSynp[#].ST.AMinus", &p, &abSynData::ST, &STPlast::AMinus);
    addAP("abSynp[#].ST.tauMinus", &p, &abSynData::ST, &STPlast::tauMinus);
    addAP("abSynp[#].ST.APlus", &p, &abSynData::ST, &STPlast::APlus);
    addAP("abSynp[#].ST.tauPlus", &p, &abSynData::ST, &STPlast::tauPlus);
    addAP("abSynp[#].ST.Exponent", &p, &abSynData::ST, &STPlast::Exponent);
    addAP("abSynp[#].ST.Shift", &p, &abSynData::ST, &STPlast::Shift);
    addAP("abSynp[#].ST.History", &p, &abSynData::ST, &STPlast::History);
    addAP("abSynp[#].ST.Table", &p, &abSynData::ST, &STPlast::Table);
    addAP("abSynp[#].ST.tableDt", &p, &abSynData::ST, &STPlast::tableDt);
    addAP("abSynp[#].ST.tableDgMin", &p, &abSynData::ST, &STPlast::tableDgMin);
    addAP("abSynp[#].ST.gMax", &p, &abSynData::ST, &STPlast::gMax);
    addAP("abSynp[#].ST.gMid", &p, &abSynData::ST, &STPlast::gMid);
    addAP("abSynp[#].ST.gSlope", &p, &abSynData::ST, &STPlast::gSlope);
    addAP("abSynp[#].ST.sigmoidTable", &p, &abSynData::ST, &STPlast::sigmoidTable);
    addAP("abSynp[#].ST.sigmoidTableDg", &p, &abSynData::ST, &STPlast::sigmoidTableDg);
    addAP("abSynp[#].ST.sigmoidTableMaxEntry", &p, &abSynData::ST, &STPlast::sigmoidTableMaxEntry);

    // abSynp ODEPlast
    addAP("abSynp[#].ODE.InitialP", &p, &abSynData::ODE, &ODEPlast::InitialP);
    addAP("abSynp[#].ODE.InitialD", &p, &abSynData::ODE, &ODEPlast::InitialD);
    addAP("abSynp[#].ODE.betaP", &p, &abSynData::ODE, &ODEPlast::betaP);
    addAP("abSynp[#].ODE.betaD", &p, &abSynData::ODE, &ODEPlast::betaD);
    addAP("abSynp[#].ODE.gamma", &p, &abSynData::ODE, &ODEPlast::gamma);
    addAP("abSynp[#].ODE.eta", &p, &abSynData::ODE, &ODEPlast::eta);
    addAP("abSynp[#].ODE.highP", &p, &abSynData::ODE, &ODEPlast::highP);
    addAP("abSynp[#].ODE.lowP", &p, &abSynData::ODE, &ODEPlast::lowP);
    addAP("abSynp[#].ODE.highD", &p, &abSynData::ODE, &ODEPlast::highD);
    addAP("abSynp[#].ODE.lowD", &p, &abSynData::ODE, &ODEPlast::lowD);
    addAP("abSynp[#].ODE.gMax", &p, &abSynData::ODE, &ODEPlast::gMax);
    addAP("abSynp[#].ODE.gMid", &p, &abSynData::ODE, &ODEPlast::gMid);
    addAP("abSynp[#].ODE.gSlope", &p, &abSynData::ODE, &ODEPlast::gSlope);
    addAP("abSynp[#].ODE.sigmoidTable", &p, &abSynData::ODE, &ODEPlast::sigmoidTable);
    addAP("abSynp[#].ODE.sigmoidTableDg", &p, &abSynData::ODE, &ODEPlast::sigmoidTableDg);
    addAP("abSynp[#].ODE.sigmoidTableMaxEntry", &p, &abSynData::ODE, &ODEPlast::sigmoidTableMaxEntry);

    // main abSynp
    addAP("abSynp[#].active", &p, &abSynData::active);
    addAP("abSynp[#].activeSettling", &p, &abSynData::activeSettling);
    addAP("abSynp[#].label", &p, &abSynData::label);
    addAP("abSynp[#].LUTables", &p, &abSynData::LUTables);
    addAP("abSynp[#].gSyn", &p, &abSynData::gSyn);
    addAP("abSynp[#].Vrev", &p, &abSynData::Vrev);
    addAP("abSynp[#].aS", &p, &abSynData::aS);
    addAP("abSynp[#].bS", &p, &abSynData::bS);
    addAP("abSynp[#].aR", &p, &abSynData::aR);
    addAP("abSynp[#].VaR", &p, &abSynData::VaR);
    addAP("abSynp[#].saR", &p, &abSynData::saR);
    addAP("abSynp[#].bR", &p, &abSynData::bR);
    addAP("abSynp[#].fixVpost", &p, &abSynData::fixVpost);
    addAP("abSynp[#].Vpost", &p, &abSynData::Vpost);
    addAP("abSynp[#].Plasticity", &p, &abSynData::Plasticity);
    addAP("abSynp[#].assign[#].active", &p, &abSynData::assign, &SynapseAssignment::active);
    addAP("abSynp[#].assign[#].PreSynChannel", &p, &abSynData::assign, &SynapseAssignment::PreSynChannel);
    addAP("abSynp[#].assign[#].PostSynChannel", &p, &abSynData::assign, &SynapseAssignment::PostSynChannel);
    addAP("abSynp[#].assign[#].OutSynChannel", &p, &abSynData::assign, &SynapseAssignment::OutSynChannel);
    addAP("abSynp[#].assign[#].delay", &p, &abSynData::assign, &SynapseAssignment::delay);
    addAP("abSynp[#].assign[#].save", &p, &abSynData::assign, &SynapseAssignment::save);

    addAP("abSynp[#].PreSynChannel", &p, &abSynData::legacy_PreSyn);
    addAP("abSynp[#].PostSynChannel", &p, &abSynData::legacy_PostSyn);
    addAP("abSynp[#].OutSynChannel", &p, &abSynData::legacy_OutSyn);

    addAP("abSynp[#].gSyn_channel", &p, &abSynData::gSyn_channel);
}

    
abSyn::abSyn(size_t condID, size_t assignID, size_t multiID, DCThread *DCT, inChannel *pre, inChannel *post, outChannel *out) :
    Synapse(condID, assignID, multiID, pre, post, out),
    p(&params()),
    a(&assignment()),
    gSyn_dynamic(DCT->getInChan(p->gSyn_channel)),
    S(0.0),
    R(0.0),
    g(p->gSyn)
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

    graw= invGFilter(g);
    if (p->Plasticity == 2) {
      P= p->ODE.InitialP;
      D= p->ODE.InitialD;
      Pslope= 1.0/(p->ODE.highP - p->ODE.lowP);
      Dslope= 1.0/(p->ODE.highD - p->ODE.lowD);
    }

    setupBuffer(DCT);
}

double abSyn::invGFilter(double ing)
{
  static double gr;
  
  switch (p->Plasticity) {
    case 1:
      gr= p->ST.gSlope*atanh(2.0*ing/p->ST.gMax - 1.0) + p->ST.gMid;
      break; 
    case 2:
      gr= p->ODE.gSlope*atanh(2.0*ing/p->ODE.gMax - 1.0) + p->ODE.gMid;
      break; 
  }
  return gr;
}

double abSyn::gFilter(double ingr)
{
  static double ng;
  
  switch (p->Plasticity) {
    case 1:
      ng= p->ST.gMax*((*theTanh)((ingr- p->ST.gMid)/p->ST.gSlope)+1)/2.0;
      break; 
    case 2:
      ng= p->ODE.gMax*((*theTanh)((ingr- p->ODE.gMid)/p->ODE.gSlope)+1)/2.0;
      break;
  }
  return ng;
}

void abSyn::step(double t, double dt, bool settling)
{
  static double dS, dR;

  if ( !p->active || !a->active || !pre->active || !post->active || !out->active || t < a->delay ) {
      m_conductance = 0;
      return;
  }
  
  // calculate synaptic current
  dS= (1.0 - S)*p->aS*R - S*p->bS;
  dR= (1.0 - R)*p->aR*(*theExpSigmoid)(((buffered ? pre->getBufferedV(bufferHandle) : pre->V)-p->VaR)/p->saR) - R*p->bR;
  // Linear Euler:
  S+= dS*dt;   /// use previous values of Sinf, S
  if (S > 1.0) S= 1.0;
  else if (S < 0.0 || std::isnan(S)) S= 0.0;
  R+= dR*dt;
  if (R > 1.0) R= 1.0;
  else if (R < 0.0 || std::isnan(R)) R= 0.0;
  
  double postV = p->fixVpost ? p->Vpost : post->V;

  // if plastic, learn
  switch ( (settling && !p->activeSettling) ? 0 : p->Plasticity ) {
    case 0:
      m_conductance = p->gSyn * S;
      break;
    case 1:
      m_conductance = g * S;
      STlearn(t);   
      break; 
    case 2:
      m_conductance = g * S;
      ODElearn(dt);
      break; 
  }

  if ( gSyn_dynamic )
      m_conductance *= gSyn_dynamic->V;

  if ( !settling || p->activeSettling )
      out->I += m_conductance * (p->Vrev - postV);
}

void abSyn::STlearn(double t)
{
  static int sn;
  static double tmp;
  
  if (pre->startSpiking) {
    for (int i= 0; i < p->ST.History; i++) {
      sn= (post->lastSpikeN-i)%MAX_ST_HISTORY;
      if (sn >= 0) {
        tmp= post->lastSpike[sn];     
        if (tmp > 0.0) graw+= STDPFunc(tmp-t);
      }
      g= gFilter(graw);
    }
  }
  if (post->startSpiking) {
    for (int i= 0; i < p->ST.History; i++) {
      sn= (pre->lastSpikeN-i)%MAX_ST_HISTORY;
      if (sn >= 0) {
        tmp= pre->lastSpike[sn];
        if (tmp > 0.0) graw+= STDPFunc(t-tmp);
      }
      g= gFilter(graw);
    }
  }
}

double abSyn::STDPFunc(double indt) 
{
  static double dt;     
  dt= indt-p->ST.Shift;
  
  if (p->ST.Exponent == 0.0) {
    if (dt > 0.0)
      return p->ST.APlus*(*theExp)(-dt/p->ST.tauPlus);
    else if (dt < 0.0)
      return -p->ST.AMinus*(*theExp)(dt/p->ST.tauMinus);
    else return 0.0;
  }
  else if (p->ST.Exponent == 1.0) {
    if (dt > 0.0)
      return p->ST.APlus*dt/p->ST.tauPlus*(*theExp)(-dt/p->ST.tauPlus);
    else if (dt < 0.0)
      return p->ST.AMinus*dt/p->ST.tauMinus*(*theExp)(dt/p->ST.tauMinus);
    else return 0.0;
  } else {
    if (dt > 0.0)
      return (p->ST.APlus*exp(p->ST.Exponent*log(dt/p->ST.tauPlus))*(*theExp)(-dt/p->ST.tauPlus));
    else if (dt < 0.0)
      return (-p->ST.AMinus*exp(p->ST.Exponent*log(-dt/p->ST.tauMinus))*(*theExp)(dt/p->ST.tauMinus));
    else return 0.0;
  }
}

double abSyn::P_f(double V) 
{
  if (V > p->ODE.highP) return 1.0;
  if (V < p->ODE.lowP) return 0.0;
  return Pslope*(V-p->ODE.lowP);
}

double abSyn::D_f(double V)
{
  if (V > p->ODE.highD) return 1.0;
  if (V < p->ODE.lowD) return 0.0;
  return Dslope*(V-p->ODE.lowD); 
}

void abSyn::ODElearn(double dt)
{
  static double tmp1, tmp2;

  tmp1= 1.0;
  tmp2= 1.0;
  for (int i= 0; i < p->ODE.eta; i++) {
    tmp1*= D;
    tmp2*= P;
  }
  graw+= dt * p->ODE.gamma*(P*tmp1 - D*tmp2);
  g= gFilter(graw);

  P+= dt * (P_f(buffered ? pre->getBufferedV(bufferHandle) : pre->V) - p->ODE.betaP * P);
  D+= dt * (D_f(post->V) - p->ODE.betaD * D);
}
