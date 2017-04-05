#ifndef HH_H
#define HH_H

#include <cmath>
#include "IonicCurrent.h"
#include "Global_func.h"

struct mhHHData : public CurrentData {
  bool LUTables;
  double gMax;
  double Vrev;
  int mExpo;
  int hExpo;
  double Vm;
  double sm;
  double Cm;
  int taumType;
  double taum;
  double taumAmpl;
  double Vtaum;
  double staum;
  double Vh;
  double sh;
  double Ch;
  int tauhType;
  double tauh;
  double tauhAmpl;
  double Vtauh;
  double stauh;
};

class HHProxy : public IonicCurrentProxy
{
public:
    HHProxy();
    inline mhHHData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "mhHH"; }
    inline QString prettyName() { return "m/h/tau HH"; }

    IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, DCThread *,
                                 inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<mhHHData> p;
};

class HH : public IonicCurrent
{
  private:
    const mhHHData *p;
    const CurrentAssignment *a;

    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;
    
  protected:
    double m, h;
    double km[4], kh[4], mi, hi;
    double minf, taum, hinf, tauh;
    
  public:
    HH(size_t condID, size_t assignID, DCThread *, inChannel *in, outChannel *out);
    void step(double t, double dt);
    void RK4(double t, double dt, size_t n);

    inline const mhHHData &params() const { return HHProxy::p[condID]; }
    HHProxy *proxy() const;
};

#endif
