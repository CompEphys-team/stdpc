#include "HHNeuron.h"

HHNeuron::HHNeuron(ModelPrototype *parent, int instID, DCThread *DCT) :
    Model(parent, instID, DCT),
    V(-0.06)
{
    in.V = V;
}

void HHNeuron::updateIn(double t)
{
    in.V = V;
    in.process(t);
}

void HHNeuron::updateOut(double)
{
    out.I = params().outChn.bias;
}

void HHNeuron::update(double, double dt)
{
    const HHNeuronData &p = static_cast<HHNeuronData const&>(parent->params());
    double I = out.I + p.gLeak * (p.ELeak - V);
    V += I * dt / p.C;
    in.V = V;
}



void HHNeuronModel::init(DCThread *DCT)
{
    inst.reserve(params().numInst());
    for ( size_t i = 0; i < params().numInst(); i++ )
        if ( params().instance(i).active )
            inst.emplace_back(new HHNeuron(this, i, DCT));
}
