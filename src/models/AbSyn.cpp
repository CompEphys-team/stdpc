
#include "AbSyn.h"
#include <cmath>
#include "DCThread.h"
    
abSyn::abSyn(abSynData *inp, DCThread *t, SynapseAssignment *a) :
    p(inp),
    S(0.0),
    R(0.0),
    g(p->gSyn)
{
    if ( !a ) {
        pre = t->getInChan(p->PreSynChannel);
        post = t->getInChan(p->PostSynChannel);
        out = t->getOutChan(p->OutSynChannel);
    } else {
        pre = t->getInChan(a->PreSynChannel);
        post = t->getInChan(a->PostSynChannel);
        out = t->getOutChan(a->OutSynChannel);
    }

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

void abSyn::currentUpdate(double t, double dt)
{
  static double dS, dR;
  
  // calculate synaptic current
  dS= (1.0 - S)*p->aS*R - S*p->bS;
  dR= (1.0 - R)*p->aR*(*theExpSigmoid)((pre->V-p->VaR)/p->saR) - R*p->bR;
  // Linear Euler:
  S+= dS*dt;   /// use previous values of Sinf, S
  if (S > 1.0) S= 1.0;
  if (S < 0.0) S= 0.0;
  R+= dR*dt;
  if (R > 1.0) R= 1.0;
  if (R < 0.0) R= 0.0;
  

  // if plastic, learn
  switch (p->Plasticity) {
    case 0:
      if (p->fixVpost) I= p->gSyn * S * (p->Vrev - p->Vpost);
      else I= p->gSyn * S * (p->Vrev - post->V);
      break;
    case 1:
      if (p->fixVpost) I= g * S * (p->Vrev - p->Vpost);
      else I= g * S * (p->Vrev - post->V);
      STlearn(t);   
      break; 
    case 2:
      if (p->fixVpost) I= g * S * (p->Vrev - p->Vpost);
      else I= g * S * (p->Vrev - post->V);
      ODElearn(dt);
      break; 
  }
  out->I+= I;
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

  P+= dt * (P_f(pre->V) - p->ODE.betaP * P);
  D+= dt * (D_f(post->V) - p->ODE.betaD * D);
}
