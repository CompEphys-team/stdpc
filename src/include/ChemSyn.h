#ifndef CHEMSYN_H
#define CHEMSYN_H

#include "Global_func.h"
#include "Channels.h"

class ChemSyn {
  private:
    CSynData *p;
    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;
    void learn();
    inChannel *pre;
    inChannel *post;
    outChannel *out;
    SynapseAssignment a;

    size_t bufferHandle;
    bool buffered;

    double P_f(double);
    double D_f(double);
    double invGFilter(double);
    double gFilter(double);
    
  protected:
    double I;
    double Sinf;
    double S;
    double hinf, tauh;
    double h;
    double g;
    double graw;
    double gfac;
    double P;  // "potentiation var" in ODE STDP
    double D;  // "depression var" in ODE STDP
    double Pslope; // slope of the sigmoid for P
    double Dslope; // slope of the sigmoid for D

  public:
    ChemSyn(CSynData *, DCThread *, SynapseAssignment);
    void currentUpdate(double, double);
    void STlearn(double);
    double STDPFunc(double);
    void ODElearn(double);

    typedef CSynData param_type;
};

#endif
