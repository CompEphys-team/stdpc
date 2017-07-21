#ifndef ABSYN_H
#define ABSYN_H

#include "Global_func.h"
#include "Synapse.h"

struct abSynData : public SynapseData {
  bool LUTables;
  double gSyn;
  double Vrev;
  double aS;
  double bS;
  double aR;
  double VaR;
  double saR;
  double bR;
  int fixVpost;
  double Vpost;
  int Plasticity;
  STPlast ST;
  ODEPlast ODE;
};

class abSynProxy : public SynapseProxy {
public:
    abSynProxy();
    inline abSynData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "abSyn"; }
    inline QString prettyName() { return "a/b Syn"; }

    Synapse *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *,
                            inChannel *pre, inChannel *post, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<abSynData> p;
};

class abSyn : public Synapse {
  private:
    const abSynData *p;
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
    double R;
    double g;
    double graw;
    double P;  // "potentiation var" in ODE STDP
    double D;  // "depression var" in ODE STDP
    double Pslope; // slope of the sigmoid for P
    double Dslope; // slope of the sigmoid for D

  public:
    abSyn(size_t condID, size_t assignID, size_t multiID, DCThread *, inChannel *pre, inChannel *post, outChannel *out);
    inline const abSynData &params() const { return abSynProxy::p[condID]; }
    void step(double t, double dt, bool settling);

    abSynProxy *proxy() const;
};

#endif
