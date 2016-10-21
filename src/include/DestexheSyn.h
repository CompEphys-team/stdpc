#ifndef DESTEXHESYN_H
#define DESTEXHESYN_H

#include "Global_func.h"
#include "Channels.h"

class DestexheSyn {
  private:
    DestexheSynData *p;
    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;
    void learn();
    inChannel *pre;
    inChannel *post;
    outChannel *out;
    SynapseAssignment &a;

    size_t bufferHandle;
    bool buffered;

    double P_f(double);
    double D_f(double);
    double invGFilter(double);
    double gFilter(double);
    
  protected:
    double I;
    double S;
    double tlast;
    double g;
    double graw;
    double P;  // "potentiation var" in ODE STDP
    double D;  // "depression var" in ODE STDP
    double Pslope; // slope of the sigmoid for P
    double Dslope; // slope of the sigmoid for D

  public:
    DestexheSyn(DestexheSynData *, DCThread *, SynapseAssignment &, inChannel *pre, inChannel *post, outChannel *out);
    void currentUpdate(double, double);
    void STlearn(double);
    double STDPFunc(double);
    void ODElearn(double);

    typedef DestexheSynData param_type;
};

#endif
