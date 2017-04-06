#include "ChemSyn.h"
#include <cmath>
#include "DCThread.h"
#include "ChemSynDlg.h"

static ChemSynProxy prox;
std::vector<CSynData> ChemSynProxy::p;
ChemSynProxy *ChemSyn::proxy() const { return &prox; }
ConductanceDlg *ChemSynProxy::createDialog(size_t condID, QWidget *parent) { return new ChemSynDlg(condID, parent); }
Synapse *ChemSynProxy::createAssigned(size_t conductanceID, size_t assignID, DCThread *DCT,
                                      inChannel *pre, inChannel *post, outChannel *out) {
    return new ChemSyn(conductanceID, assignID, DCT, pre, post, out);
}

ChemSynProxy::ChemSynProxy()
{
    ConductanceManager::RegisterSynapse(this);
    // CSynp STPlast
    addAP("CSynp[#].ST.AMinus", &p, &CSynData::ST, &STPlast::AMinus);
    addAP("CSynp[#].ST.tauMinus", &p, &CSynData::ST, &STPlast::tauMinus);
    addAP("CSynp[#].ST.APlus", &p, &CSynData::ST, &STPlast::APlus);
    addAP("CSynp[#].ST.tauPlus", &p, &CSynData::ST, &STPlast::tauPlus);
    addAP("CSynp[#].ST.Exponent", &p, &CSynData::ST, &STPlast::Exponent);
    addAP("CSynp[#].ST.Shift", &p, &CSynData::ST, &STPlast::Shift);
    addAP("CSynp[#].ST.History", &p, &CSynData::ST, &STPlast::History);
    addAP("CSynp[#].ST.Table", &p, &CSynData::ST, &STPlast::Table);
    addAP("CSynp[#].ST.tableDt", &p, &CSynData::ST, &STPlast::tableDt);
    addAP("CSynp[#].ST.tableDgMin", &p, &CSynData::ST, &STPlast::tableDgMin);
    addAP("CSynp[#].ST.gMax", &p, &CSynData::ST, &STPlast::gMax);
    addAP("CSynp[#].ST.gMid", &p, &CSynData::ST, &STPlast::gMid);
    addAP("CSynp[#].ST.gSlope", &p, &CSynData::ST, &STPlast::gSlope);
    addAP("CSynp[#].ST.sigmoidTable", &p, &CSynData::ST, &STPlast::sigmoidTable);
    addAP("CSynp[#].ST.sigmoidTableDg", &p, &CSynData::ST, &STPlast::sigmoidTableDg);
    addAP("CSynp[#].ST.sigmoidTableMaxEntry", &p, &CSynData::ST, &STPlast::sigmoidTableMaxEntry);

    // CSynp ODEPlast
    addAP("CSynp[#].ODE.InitialP", &p, &CSynData::ODE, &ODEPlast::InitialP);
    addAP("CSynp[#].ODE.InitialD", &p, &CSynData::ODE, &ODEPlast::InitialD);
    addAP("CSynp[#].ODE.betaP", &p, &CSynData::ODE, &ODEPlast::betaP);
    addAP("CSynp[#].ODE.betaD", &p, &CSynData::ODE, &ODEPlast::betaD);
    addAP("CSynp[#].ODE.gamma", &p, &CSynData::ODE, &ODEPlast::gamma);
    addAP("CSynp[#].ODE.eta", &p, &CSynData::ODE, &ODEPlast::eta);
    addAP("CSynp[#].ODE.highP", &p, &CSynData::ODE, &ODEPlast::highP);
    addAP("CSynp[#].ODE.lowP", &p, &CSynData::ODE, &ODEPlast::lowP);
    addAP("CSynp[#].ODE.highD", &p, &CSynData::ODE, &ODEPlast::highD);
    addAP("CSynp[#].ODE.lowD", &p, &CSynData::ODE, &ODEPlast::lowD);
    addAP("CSynp[#].ODE.gMax", &p, &CSynData::ODE, &ODEPlast::gMax);
    addAP("CSynp[#].ODE.gMid", &p, &CSynData::ODE, &ODEPlast::gMid);
    addAP("CSynp[#].ODE.gSlope", &p, &CSynData::ODE, &ODEPlast::gSlope);
    addAP("CSynp[#].ODE.sigmoidTable", &p, &CSynData::ODE, &ODEPlast::sigmoidTable);
    addAP("CSynp[#].ODE.sigmoidTableDg", &p, &CSynData::ODE, &ODEPlast::sigmoidTableDg);
    addAP("CSynp[#].ODE.sigmoidTableMaxEntry", &p, &CSynData::ODE, &ODEPlast::sigmoidTableMaxEntry);

    // main Csynp
    addAP("CSynp[#].active", &p, &CSynData::active);
    addAP("CSynp[#].LUTables", &p, &CSynData::LUTables);
    addAP("CSynp[#].MgBlock", &p, &CSynData::MgBlock);
    addAP("CSynp[#].gSyn", &p, &CSynData::gSyn);
    addAP("CSynp[#].VSyn", &p, &CSynData::VSyn);
    addAP("CSynp[#].tauSyn", &p, &CSynData::tauSyn);
    addAP("CSynp[#].VThresh", &p, &CSynData::VThresh);
    addAP("CSynp[#].VSlope", &p, &CSynData::VSlope);
    addAP("CSynp[#].STD", &p, &CSynData::STD);
    addAP("CSynp[#].STDAmpl", &p, &CSynData::STDAmpl);
    addAP("CSynp[#].STDVThresh", &p, &CSynData::STDVThresh);
    addAP("CSynp[#].STDVSlope", &p, &CSynData::STDVSlope);
    addAP("CSynp[#].STDtauAmpl", &p, &CSynData::STDtauAmpl);
    addAP("CSynp[#].STDtau0", &p, &CSynData::STDtau0);
    addAP("CSynp[#].STDtauVThresh", &p, &CSynData::STDtauVThresh);
    addAP("CSynp[#].STDtauVSlope", &p, &CSynData::STDtauVSlope);
    addAP("CSynp[#].fixVpost", &p, &CSynData::fixVpost);
    addAP("CSynp[#].Vpost", &p, &CSynData::Vpost);
    addAP("CSynp[#].Mgfac", &p, &CSynData::Mgfac);
    addAP("CSynp[#].Mgexpo", &p, &CSynData::Mgexpo);
    addAP("CSynp[#].Plasticity", &p, &CSynData::Plasticity);
    addAP("CSynp[#].assign[#].active", &p, &CSynData::assign, &SynapseAssignment::active);
    addAP("CSynp[#].assign[#].PreSynChannel", &p, &CSynData::assign, &SynapseAssignment::PreSynChannel);
    addAP("CSynp[#].assign[#].PostSynChannel", &p, &CSynData::assign, &SynapseAssignment::PostSynChannel);
    addAP("CSynp[#].assign[#].OutSynChannel", &p, &CSynData::assign, &SynapseAssignment::OutSynChannel);
    addAP("CSynp[#].assign[#].delay", &p, &CSynData::assign, &SynapseAssignment::delay);
    addAP("CSynp[#].assign[#].save", &p, &CSynData::assign, &SynapseAssignment::save);

    addAP("CSynp[#].PreSynChannel", &p, &CSynData::legacy_PreSyn);
    addAP("CSynp[#].PostSynChannel", &p, &CSynData::legacy_PostSyn);
    addAP("CSynp[#].OutSynChannel", &p, &CSynData::legacy_OutSyn);
}



ChemSyn::ChemSyn(size_t condID, size_t assignID, DCThread *DCT, inChannel *pre, inChannel *post, outChannel *out) :
    Synapse(condID, assignID, pre, post, out),
    p(&params()),
    a(&assignment()),
    Sinf(0.0),
    S(0.0),
    hinf(1.0),
    tauh(1.0),
    h(1.0),
    g(p->gSyn),
    gfac(1.0)
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

void ChemSyn::step(double t, double dt)
{
    if ( !p->active || !a->active || !pre->active || !post->active || !out->active || t < a->delay )
        return;

    // calculate synaptic current
    double tmp = (1.0 - Sinf)*p->tauSyn;
    //  if (tmp < 1e-8) tmp= 1e-8;
    // Linear Euler:
    S+= (Sinf - S)/tmp*dt;   /// use previous values of Sinf, S
    if (S > 1.0) S= 1.0;
    else if (S < 0.0 || std::isnan(S)) S= 0.0;

    double preV = buffered ? pre->getBufferedV(bufferHandle) : pre->V;

    if (preV > p->VThresh)
      Sinf= (*theTanh)((preV - p->VThresh)/p->VSlope);
    else Sinf= 0.0;

    if(p->STD){
      // Linear Euler:
      h+= (hinf - h)/tauh*dt;   // use previous values of hinf, h, tauh
      tmp= (*theExpSigmoid)((preV - p->STDVThresh)/p->STDVSlope);
      hinf= p->STDAmpl*tmp;
      tmp= (*theExpSigmoid)((preV - p->STDtauVThresh)/p->STDtauVSlope);
      tauh= p->STDtau0 - p->STDtauAmpl*tmp;
    }
    else h= 1.0;

    double postV = p->fixVpost ? p->Vpost : post->V;

    if (p->MgBlock) {
      gfac= 1.0/(1.0+p->Mgfac * exp(p->Mgexpo*postV));
    }


    // if plastic, learn
    switch (p->Plasticity) {
      case 0:
        m_conductance = p->gSyn * gfac * S * h;
        break;
      case 1:
        m_conductance = g * gfac * S * h;
        STlearn(t);
        break;
      case 2:
        m_conductance = g * gfac * S * h;
        ODElearn(dt);
        break;
    }

    out->I += m_conductance * (p->VSyn - postV);
}

double ChemSyn::invGFilter(double ing)
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

double ChemSyn::gFilter(double ingr)
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

void ChemSyn::STlearn(double t)
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

double ChemSyn::STDPFunc(double indt) 
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

double ChemSyn::P_f(double V) 
{
  if (V > p->ODE.highP) return 1.0;
  if (V < p->ODE.lowP) return 0.0;
  return Pslope*(V-p->ODE.lowP);
}

double ChemSyn::D_f(double V)
{
  if (V > p->ODE.highD) return 1.0;
  if (V < p->ODE.lowD) return 0.0;
  return Dslope*(V-p->ODE.lowD); 
}

void ChemSyn::ODElearn(double dt)
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
