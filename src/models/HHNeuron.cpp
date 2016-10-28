#include "HHNeuron.h"

HHNeuron::HHNeuron(ModelPrototype *parent, int instID, DCThread *DCT) :
    Model(parent, instID, DCT),
    V(-0.06)
{
    Vi = V;
    in.V = V + params().inChn.bias;
}

void HHNeuron::RK4(double, double dt, size_t n)
{
    // instance update
    const HHNeuronData &p = static_cast<HHNeuronData const&>(parent->params());
    kV[n] = (out.I + p.gLeak * (p.ELeak - Vi)) / p.C;
    if ( n < 3 ) {
        Vi = V + kV[n]*dt;
    } else {
        Vi = V = V + (kV[0] + 2*kV[1] + 2*kV[2] + kV[3]) * dt / 6;
    }
    in.V = Vi + params().inChn.bias;

    // restore current (except on last step, where it's better to keep the final value for output/graphing)
    if ( n < 3 ) {
        out.I = retainedI;
    }
}



void HHNeuronModel::init(DCThread *DCT)
{
    inst.reserve(params().numInst());
    for ( size_t i = 0; i < params().numInst(); i++ )
        if ( params().instance(i).active )
            inst.emplace_back(new HHNeuron(this, i, DCT));
}
