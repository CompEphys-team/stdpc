
#include "AbSyn.h"
#include <cmath>
    
abSyn::abSyn() 
{
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

void abSyn::init(abSynData *inp, short int *inIdx, short int *outIdx, inChannel *inChn, outChannel *outChn)
{
  p= inp;
  pre= &inChn[inIdx[p->PreSynChannel]];
  post= &inChn[inIdx[p->PostSynChannel]];
  out= &outChn[outIdx[p->OutSynChannel]];

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
  S= 0.0;
  R= 0.0;
  g= p->gSyn;
  graw= invGFilter(g);
  if (p->Plasticity == 2) {
    P= p->ODE.InitialP;
    D= p->ODE.InitialD;
    Pslope= 1.0/(p->ODE.highP - p->ODE.lowP);
    Dslope= 1.0/(p->ODE.highD - p->ODE.lowD);
  }
}

void abSyn::currentUpdate(double t, double dt)
{
  static double dS, dR;
  
  // calculate synaptic current
  dS= (1.0 - S)*p->aS*(*theExpSigmoid)((pre->V-p->VaS)/p->saS) - S*p->bS;
  dR= (1.0 - R)*p->aR*(*theExpSigmoid)((pre->V-p->VaR)/p->saR) - R*p->bS;
  // Linear Euler:
  S+= dS*dt;   /// use previous values of Sinf, S
  if (S > 1.0) S= 1.0;
  if (S < 0.0) S= 0.0;
  R+= dR*dt;
  if (R > 1.0) R= 1.0;
  if (R < 0.0) R= 0.0;
  
  if(p->fixVpost) I= g * S * (1.0 - R) * (p->Vrev - p->Vpost);
  else I= g * S * (1.0 - R) * (p->Vrev - post->V);
  out->I+= I;

  // if plastic, learn
  switch (p->Plasticity) {
    case 1:
      STlearn(t);   
      break; 
    case 2:
      ODElearn(dt);
      break; 
  }
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
