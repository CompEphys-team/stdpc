#ifndef HHNEURON_H
#define HHNEURON_H

#include "ModelManager.h"

class HHNeuronProxy : public ModelProxy {
public:
    HHNeuronProxy();
    inline ModelData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline QString modelClass() { return "HH"; }
    inline ModelPrototype *createPrototype(int modelID);

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
