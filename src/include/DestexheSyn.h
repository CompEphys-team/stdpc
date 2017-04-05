#ifndef DESTEXHESYN_H
#define DESTEXHESYN_H

#include "Global_func.h"
#include "Synapse.h"

struct DestexheSynData : public SynapseData {
  bool LUTables;
  double gSyn;
  double Vpre;
  double Vrev;
  double trelease;
  double alpha;
  double beta;
  int fixVpost;
  double Vpost;
  int Plasticity;
  STPlast ST;
  ODEPlast ODE;
};

class DestexheSynProxy : public SynapseProxy {
public:
    DestexheSynProxy();
    inline DestexheSynData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "DxheSyn"; }
    inline QString prettyName() { return "DestexheSyn"; }

    Synapse *createAssigned(size_t conductanceID, size_t assignID, DCThread *,
                            inChannel *pre, inChannel *post, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<DestexheSynData> p;
};

class DestexheSyn : public Synapse {
  private:
    const DestexheSynData *p;
    const SynapseAssignment *a;

    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;

    double P_f(double);
    double D_f(double);
    double invGFilter(double);
    double gFilter(double);

    void STlearn(double);
    double STDPFunc(double);
    void ODElearn(double);
    
  protected:
    double S;
    double tlast;
    double g;
    double graw;
    double P;  // "potentiation var" in ODE STDP
    double D;  // "depression var" in ODE STDP
    double Pslope; // slope of the sigmoid for P
    double Dslope; // slope of the sigmoid for D

  public:
    DestexheSyn(size_t condID, size_t assignID, DCThread *, inChannel *pre, inChannel *post, outChannel *out);
    inline const DestexheSynData &params() const { return DestexheSynProxy::p[condID]; }
    void step(double t, double dt);

    DestexheSynProxy *proxy() const;
};

#endif
