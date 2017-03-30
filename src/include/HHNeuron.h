#ifndef HHNEURON_H
#define HHNEURON_H

#include "ModelManager.h"

struct HHNeuronData : public ModelData
{
    double C;
    double gLeak;
    double ELeak;
    double Vmin;
    double Vmax;
    std::vector<vInstData> inst;

    inline vInstData &instance(size_t i) { return inst[i]; }
    inline size_t numInst() const { return inst.size(); }
    HHNeuronData() : C(3.5e-9), gLeak(20e-9), ELeak(-20e-3), Vmin(-0.2), Vmax(0.2) {}
};

class HHNeuronProxy : public ModelProxy {
public:
    HHNeuronProxy();
    inline ModelData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString modelClass() { return "HH"; }
    inline QString prettyName() { return "HH Model"; }

    inline ModelPrototype *createPrototype(size_t modelID);
    inline ModelDlg *createDialog(size_t modelID, QWidget *parent=nullptr);

    static std::vector<HHNeuronData> p;
};


class HHNeuron : public Model
{
public:
    HHNeuron(ModelPrototype *parent, size_t instID, DCThread *DCT);

    void RK4(double t, double dt, size_t n);

protected:
    double V;
    double kV[4], Vi;
};


class HHNeuronModel : public ModelPrototype
{
public:
    HHNeuronModel(size_t modelID) : ModelPrototype(modelID) {}
    ~HHNeuronModel() {}

    void init(DCThread *);

    inline ModelData &params() const { return HHNeuronProxy::p[modelID]; }
    ModelProxy *proxy() const;
    inline QString prefix() const { return "HH"; }
};

#endif // HHNEURON_H
