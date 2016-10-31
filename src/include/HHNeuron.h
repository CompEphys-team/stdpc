#ifndef HHNEURON_H
#define HHNEURON_H

#include "ModelManager.h"

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
