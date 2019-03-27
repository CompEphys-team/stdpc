#pragma once

#include "Global_func.h"
#include "IonicCurrent.h"

struct WireData : public CurrentData {
    double factor = 1e-6;
};

class WireProxy : public IonicCurrentProxy {
private:
    WireProxy();
public:
    WireProxy(const WireProxy &) = delete;
    void operator=(const WireProxy &) = delete;
    static WireProxy *get() { static WireProxy proxy; return &proxy; }

    inline WireData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "Wire"; }
    inline QString prettyName() { return "Pass Wire"; }

    IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                 inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<WireData> p;
};

class Wire : public IonicCurrent {
  private:
    const WireData *p;
    const CurrentAssignment *a;
        
  public:
    Wire(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
    inline const WireData &params() const { return WireProxy::p[condID]; }

    virtual void RK4(double, double, size_t, bool) {}
    void step(double t, double dt, bool settling);

    WireProxy *proxy() const;
};

