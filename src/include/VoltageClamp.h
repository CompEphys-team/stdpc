#pragma once

#include "Global_func.h"
#include "Synapse.h"

struct VoltageClampData : public SynapseData {
    double gP;
    double gI;
    double gD;
    double decayI;
    int tstepD;
    double easeIn = 0;
    double easeInAmpLimit = 0;
};

class VoltageClampProxy : public SynapseProxy {
private:
    VoltageClampProxy();
public:
    VoltageClampProxy(const VoltageClampProxy &) = delete;
    void operator=(const VoltageClampProxy &) = delete;
    static VoltageClampProxy *get() { static VoltageClampProxy proxy; return &proxy; }

    inline VoltageClampData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "VClamp"; }
    inline QString prettyName() { return "Voltage clamp"; }

    Synapse *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *,
                            inChannel *pre, inChannel *post, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<VoltageClampData> p;
};

class VoltageClamp : public Synapse {
  private:
    const VoltageClampData *p;
    const SynapseAssignment *a;
    DCThread *DCT;
    
  protected:
    bool active;
    double I, Imin, Imax;
    double IP, II, ID;
    double cmdAvg, postAvg, denom;
    vector<double> VBuf, cmdBuf, tBuf;
    size_t Bufptr, BufSz, BufMax;
        
  public:
    VoltageClamp(size_t condID, size_t assignID, size_t multiID, DCThread *, inChannel *pre, inChannel *post, outChannel *out);
    inline const VoltageClampData &params() const { return VoltageClampProxy::p[condID]; }
    void step(double t, double dt, bool settling);

    VoltageClampProxy *proxy() const;
};

