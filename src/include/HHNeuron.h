#ifndef HHNEURON_H
#define HHNEURON_H

#include "Model.h"
#include "Global.h"

class HHNeuron : public Model
{
public:
    HHNeuron(ModelPrototype *parent, int instID, DCThread *DCT);

    void RK4(double t, double dt, size_t n);

protected:
    double V;
    double kV[4], Vi;
};


class HHNeuronModel : public ModelPrototype
{
public:
    HHNeuronModel(int modelID) : ModelPrototype(modelID) {}
    ~HHNeuronModel() {}

    void init(DCThread *);

    inline ModelData &params() const { return HHNeuronp[modelID]; }
    inline ModelClass modelClass() const { return HHNeuronData::modelClass; }
    inline QString prefix() const { return "HH"; }
};

#endif // HHNEURON_H
