#ifndef CHEMSYN_H
#define CHEMSYN_H

#include "Synapse.h"
#include "Global_func.h"

struct CSynData : public SynapseData {
  bool LUTables;
  bool MgBlock;
  double gSyn;
  double VSyn;
  double tauSyn;
  double VThresh;
  double VSlope;
  int STD;
  double STDAmpl;
  double STDVThresh;
  double STDVSlope;
  double STDtauAmpl;
  double STDtau0;
  double STDtauVThresh;
  double STDtauVSlope;
  int fixVpost;
  double Vpost;
  double Mgfac;
  double Mgexpo;
  int Plasticity;
  STPlast ST;
  ODEPlast ODE;

  bool stochastic = false;
  int stoch_nRel = 5;
  double stoch_pRel = 0.6;
  double stoch_variance = 0.05;
};

class ChemSynProxy : public SynapseProxy {
private:
    ChemSynProxy();
public:
    ChemSynProxy(const ChemSynProxy &) = delete;
    void operator=(const ChemSynProxy &) = delete;
    static ChemSynProxy *get() { static ChemSynProxy proxy; return &proxy; }

    inline CSynData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "CSyn"; }
    inline QString prettyName() { return "ChemSyn"; }

    Synapse *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *,
                            inChannel *pre, inChannel *post, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<CSynData> p;
};

class ChemSyn : public Synapse
{
  private:
    const CSynData *p;
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

    bool spike = false;
    std::vector<double> stoch_S, stoch_amplitude;

  public:
    ChemSyn(size_t condID, size_t assignID, size_t multiID, DCThread *, inChannel *pre, inChannel *post, outChannel *out);
    inline const CSynData &params() const { return ChemSynProxy::p[condID]; }
    void step(double t, double dt, bool settling);

    ChemSynProxy *proxy() const;
};

#endif
