#pragma once

#include <vector>
#include "Global_func.h"
#include "ModelManager.h"


struct VStepsData : public ModelData {
  double holdV;
  double startV;
  double endV;
  double deltaV;
  double t0;
  double tStep;
  double tHold;
  vInstData inst;

  inline vInstData &instance(size_t) { return inst; }
  inline size_t numInst() const { return 1; }

  VStepsData();
};

class VStepsProxy : public ModelProxy {
private:
    VStepsProxy();
public:
    VStepsProxy(const VStepsProxy &) = delete;
    void operator=(const VStepsProxy &) = delete;
    static VStepsProxy *get() { static VStepsProxy proxy; return &proxy; }

    inline ModelData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString modelClass() { return "VStep"; }
    inline QString prettyName() { return "Voltage stepper"; }

    ModelPrototype *createPrototype(size_t modelID);
    ModelDlg *createDialog(size_t modelID, QWidget *parent);

    static std::vector<VStepsData> p;
};

class VSteps : public Model
{
public:
    VSteps(ModelPrototype *, size_t, DCThread *);
    ~VSteps() {}

    // Override processing on unused out
    inline void updateOut(double) {}

    void update(double t, double dt);
    virtual void RK4(double t, double dt, size_t n, bool settling);

protected:
    const VStepsData * const p;
    const vInstData * const instp;
    double V;
    std::vector<double> cmdT, cmdV;
    std::vector<double>::iterator cmdTI, cmdVI;
    bool active;
};

class VStepsPrototype : public ModelPrototype
{
public:
    VStepsPrototype(size_t modelID) : ModelPrototype (modelID) {}
    ~VStepsPrototype() {}

    void init(DCThread *);

    // Override processing on unused Model::out
    inline void retainCurrent(double) {}
    inline void restoreCurrent(double) {}

    inline ModelData &params() const { return VStepsProxy::p[modelID]; }
    inline ModelProxy *proxy() const;
    inline QString prefix() const { return "VStep"; }
};

