#include "DestexheSyn.h"
#include <cmath>
#include "DCThread.h"
#include "DestexheSynDlg.h"

static DestexheSynProxy prox;
std::vector<DestexheSynData> DestexheSynProxy::p;
DestexheSynProxy *DestexheSyn::proxy() const { return &prox; }
ConductanceDlg *DestexheSynProxy::createDialog(size_t condID, QWidget *parent) { return new DestexheSynDlg(condID, parent); }
Synapse *DestexheSynProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *DCT,
                                          inChannel *pre, inChannel *post, outChannel *out) {
    return new DestexheSyn(conductanceID, assignID, multiID, DCT, pre, post, out);
}

DestexheSynProxy::DestexheSynProxy()
{
    ConductanceManager::RegisterSynapse(this);
    // DxheSynp STPlast
    addAP("DxheSynp[#].ST.AMinus", &p, &DestexheSynData::ST, &STPlast::AMinus);
    addAP("DxheSynp[#].ST.tauMinus", &p, &DestexheSynData::ST, &STPlast::tauMinus);
    addAP("DxheSynp[#].ST.APlus", &p, &DestexheSynData::ST, &STPlast::APlus);
    addAP("DxheSynp[#].ST.tauPlus", &p, &DestexheSynData::ST, &STPlast::tauPlus);
    addAP("DxheSynp[#].ST.Exponent", &p, &DestexheSynData::ST, &STPlast::Exponent);
    addAP("DxheSynp[#].ST.Shift", &p, &DestexheSynData::ST, &STPlast::Shift);
    addAP("DxheSynp[#].ST.History", &p, &DestexheSynData::ST, &STPlast::History);
    addAP("DxheSynp[#].ST.Table", &p, &DestexheSynData::ST, &STPlast::Table);
    addAP("DxheSynp[#].ST.tableDt", &p, &DestexheSynData::ST, &STPlast::tableDt);
    addAP("DxheSynp[#].ST.tableDgMin", &p, &DestexheSynData::ST, &STPlast::tableDgMin);
    addAP("DxheSynp[#].ST.gMax", &p, &DestexheSynData::ST, &STPlast::gMax);
    addAP("DxheSynp[#].ST.gMid", &p, &DestexheSynData::ST, &STPlast::gMid);
    addAP("DxheSynp[#].ST.gSlope", &p, &DestexheSynData::ST, &STPlast::gSlope);
    addAP("DxheSynp[#].ST.sigmoidTable", &p, &DestexheSynData::ST, &STPlast::sigmoidTable);
    addAP("DxheSynp[#].ST.sigmoidTableDg", &p, &DestexheSynData::ST, &STPlast::sigmoidTableDg);
    addAP("DxheSynp[#].ST.sigmoidTableMaxEntry", &p, &DestexheSynData::ST, &STPlast::sigmoidTableMaxEntry);

    // DxheSynp ODEPlast
    addAP("DxheSynp[#].ODE.InitialP", &p, &DestexheSynData::ODE, &ODEPlast::InitialP);
    addAP("DxheSynp[#].ODE.InitialD", &p, &DestexheSynData::ODE, &ODEPlast::InitialD);
    addAP("DxheSynp[#].ODE.betaP", &p, &DestexheSynData::ODE, &ODEPlast::betaP);
    addAP("DxheSynp[#].ODE.betaD", &p, &DestexheSynData::ODE, &ODEPlast::betaD);
    addAP("DxheSynp[#].ODE.gamma", &p, &DestexheSynData::ODE, &ODEPlast::gamma);
    addAP("DxheSynp[#].ODE.eta", &p, &DestexheSynData::ODE, &ODEPlast::eta);
    addAP("DxheSynp[#].ODE.highP", &p, &DestexheSynData::ODE, &ODEPlast::highP);
    addAP("DxheSynp[#].ODE.lowP", &p, &DestexheSynData::ODE, &ODEPlast::lowP);
    addAP("DxheSynp[#].ODE.highD", &p, &DestexheSynData::ODE, &ODEPlast::highD);
    addAP("DxheSynp[#].ODE.lowD", &p, &DestexheSynData::ODE, &ODEPlast::lowD);
    addAP("DxheSynp[#].ODE.gMax", &p, &DestexheSynData::ODE, &ODEPlast::gMax);
    addAP("DxheSynp[#].ODE.gMid", &p, &DestexheSynData::ODE, &ODEPlast::gMid);
    addAP("DxheSynp[#].ODE.gSlope", &p, &DestexheSynData::ODE, &ODEPlast::gSlope);
    addAP("DxheSynp[#].ODE.sigmoidTable", &p, &DestexheSynData::ODE, &ODEPlast::sigmoidTable);
    addAP("DxheSynp[#].ODE.sigmoidTableDg", &p, &DestexheSynData::ODE, &ODEPlast::sigmoidTableDg);
    addAP("DxheSynp[#].ODE.sigmoidTableMaxEntry", &p, &DestexheSynData::ODE, &ODEPlast::sigmoidTableMaxEntry);

    // main DxheSynp
    addAP("DxheSynp[#].active", &p, &DestexheSynData::active);
    addAP("DxheSynp[#].LUTables", &p, &DestexheSynData::LUTables);
    addAP("DxheSynp[#].gSyn", &p, &DestexheSynData::gSyn);
    addAP("DxheSynp[#].Vpre", &p, &DestexheSynData::Vpre);
    addAP("DxheSynp[#].Vrev", &p, &DestexheSynData::Vrev);
    addAP("DxheSynp[#].trelease", &p, &DestexheSynData::trelease);
    addAP("DxheSynp[#].alpha", &p, &DestexheSynData::alpha);
    addAP("DxheSynp[#].beta", &p, &DestexheSynData::beta);
    addAP("DxheSynp[#].fixVpost", &p, &DestexheSynData::fixVpost);
    addAP("DxheSynp[#].Vpost", &p, &DestexheSynData::Vpost);
    addAP("DxheSynp[#].Plasticity", &p, &DestexheSynData::Plasticity);
    addAP("DxheSynp[#].assign[#].active", &p, &DestexheSynData::assign, &SynapseAssignment::active);
    addAP("DxheSynp[#].assign[#].PreSynChannel", &p, &DestexheSynData::assign, &SynapseAssignment::PreSynChannel);
    addAP("DxheSynp[#].assign[#].PostSynChannel", &p, &DestexheSynData::assign, &SynapseAssignment::PostSynChannel);
    addAP("DxheSynp[#].assign[#].OutSynChannel", &p, &DestexheSynData::assign, &SynapseAssignment::OutSynChannel);
    addAP("DxheSynp[#].assign[#].delay", &p, &DestexheSynData::assign, &SynapseAssignment::delay);
    addAP("DxheSynp[#].assign[#].save", &p, &DestexheSynData::assign, &SynapseAssignment::save);

    addAP("DxheSynp[#].PreSynChannel", &p, &DestexheSynData::legacy_PreSyn);
    addAP("DxheSynp[#].PostSynChannel", &p, &DestexheSynData::legacy_PostSyn);
    addAP("DxheSynp[#].OutSynChannel", &p, &DestexheSynData::legacy_OutSyn);
}

    
DestexheSyn::DestexheSyn(size_t condID, size_t assignID, size_t multiID, DCThread *DCT, inChannel *pre, inChannel *post, outChannel *out) :
    Synapse(condID, assignID, multiID, pre, post, out),
    p(&params()),
    a(&assignment()),
    S(0.0),
    tlast(-1.0e10),
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

double DestexheSyn::invGFilter(double ing)
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

double DestexheSyn::gFilter(double ingr)
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

void DestexheSyn::step(double t, double dt)
{
  static double rt, dS;

  if ( !p->active || !a->active || !pre->active || !post->active || !out->active || t < a->delay ) {
      m_conductance = 0;
      return;
  }
  
  // calculate synaptic current
  rt= t - tlast;
  if ((rt >= 0.0) && (rt <= p->trelease)) {
    // continue release from an old spike
    dS= p->alpha*(1.0-S) - p->beta*S;
  } else {
    if ((buffered ? pre->getBufferedV(bufferHandle) : pre->V) > p->Vpre) {
      // new spike ... start releasing
      tlast= t;
      dS= p->alpha*(1.0-S) - p->beta*S;
    }
    else {
      // no release
      dS= -p->beta*S;
    }
  }
  // Linear Euler:
  S+= dS*dt;
  if (S > 1.0) S= 1.0;
  else if (S < 0.0 || std::isnan(S)) S= 0.0;

  double postV = p->fixVpost ? p->Vpost : post->V;

  // if plastic, learn
  switch (p->Plasticity) {
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
  out->I += m_conductance * (p->Vrev - postV);
}

void DestexheSyn::STlearn(double t)
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

double DestexheSyn::STDPFunc(double indt)
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

double DestexheSyn::P_f(double V)
{
  if (V > p->ODE.highP) return 1.0;
  if (V < p->ODE.lowP) return 0.0;
  return Pslope*(V-p->ODE.lowP);
}

double DestexheSyn::D_f(double V)
{
  if (V > p->ODE.highD) return 1.0;
  if (V < p->ODE.lowD) return 0.0;
  return Dslope*(V-p->ODE.lowD); 
}

void DestexheSyn::ODElearn(double dt)
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
