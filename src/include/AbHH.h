#ifndef ABHH_H
#define ABHH_H

#include "IonicCurrent.h"
#include "Global_func.h"

struct abHHData : public CurrentData {
  bool LUTables;
  double gMax;
  double Vrev;
  int mExpo;
  int hExpo;
  int maFunc;
  double mka;
  double mVa;
  double msa;
  int mbFunc;
  double mkb;
  double mVb;
  double msb;
  int haFunc;
  double hka;
  double hVa;
  double hsa;
  int hbFunc;
  double hkb;
  double hVb;
  double hsb;
};

class abHHProxy : public IonicCurrentProxy
{
public:
    abHHProxy();
    inline abHHData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "abHH"; }
    inline QString prettyName() { return "a/b HH"; }

    IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                 inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<abHHData> p;
};

class abHH : public IonicCurrent
{
  private:
    const abHHData *p;
    const CurrentAssignment *a;

    stdpc::function *theExp;
    stdpc::function *theExpSigmoid;
    stdpc::function *theTanh;

    double mhFunc(double, int);
    
  protected:
    double m, h;
    double ma, mb, ha, hb;
    double km[4], kh[4], mi, hi;
    
  public:
    abHH(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
    void step(double t, double dt);
    void RK4(double t, double dt, size_t n);

    inline const abHHData &params() const { return abHHProxy::p[condID]; }
    abHHProxy *proxy() const;
};

#endif
