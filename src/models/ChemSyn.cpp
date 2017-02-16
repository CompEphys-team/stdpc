
#include "ChemSyn.h"
#include <cmath>
#include "DCThread.h"
    
ChemSyn::ChemSyn(CSynData *inp, DCThread *t, SynapseAssignment *a, inChannel *pre, inChannel *post, outChannel *out) :
    p(inp),
    pre(pre),
    post(post),
    out(out),
    a(a),
    buffered(false),
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

    if ( a->delay > 0. ) {
        bufferHandle = pre->getBufferHandle(a->delay, t->bufferHelper);
        buffered = true;
    }
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

void ChemSyn::currentUpdate(double t, double dt)
{
  static double tmp, V;
  
  if ( !p->active || !a->active || !pre->active || !post->active || !out->active || t < a->delay )
      return;

  // calculate synaptic current
  tmp= (1.0 - Sinf)*p->tauSyn;
  //  if (tmp < 1e-8) tmp= 1e-8;
  // Linear Euler:
  S+= (Sinf - S)/tmp*dt;   /// use previous values of Sinf, S
  if (S > 1.0) S= 1.0;
  if (S < 0.0) S= 0.0;

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

  if(p->fixVpost) V= p->Vpost;
  else V= post->V;

  if (p->MgBlock) {
    gfac= 1.0/(1.0+p->Mgfac * exp(p->Mgexpo*V));
  }


  // if plastic, learn
  switch (p->Plasticity) {
    case 0:
      I= p->gSyn * gfac * S * h * (p->VSyn - V);
      break;
    case 1:
      I= g * gfac * S * h * (p->VSyn - V);
      STlearn(t);   
      break; 
    case 2:
      I= g * gfac * S * h * (p->VSyn - V);
      ODElearn(dt);
      break; 
  }
  out->I+= I;
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
