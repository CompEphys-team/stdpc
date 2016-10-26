#ifndef HHNEURON_H
#define HHNEURON_H

#include "Model.h"
#include "Global.h"

class HHNeuron : public Model
{
public:
    HHNeuron(ModelPrototype *parent, int instID, DCThread *DCT);
    void updateIn(double t);
    void updateOut(double t);
    void update(double t, double dt);

protected:
    double V;
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
