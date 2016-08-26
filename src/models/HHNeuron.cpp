#include "HHNeuron.h"

HHNeuron::HHNeuron(HHNeuronData *inp, vInstData *inst) :
    V(-0.06),
    p(inp),
    instp(inst)
{
    in.init(&instp->inChn);
    out.init(&instp->outChn);
    in.V = V;
}

void HHNeuron::update(double t, double dt)
{
    if ( instp->active ) {
        double I = out.I + p->gLeak * (p->ELeak - V);
        V += I * dt / p->C;
    }
    in.V = V;
}

HHNeuronModel::HHNeuronModel(HHNeuronData *p) :
    p(p)
{
    for ( vInstData &a : p->inst )
        inst.push_back(HHNeuron(p, &a));
}

void HHNeuronModel::updateNeurons(double t, double dt)
{
    if ( p->active )
        for ( HHNeuron &n : inst )
            n.update(t, dt);
}

void HHNeuronModel::updateChannels(double t)
{
    if ( !p->active )
        return;
    for ( HHNeuron &neuron : inst ) {
        neuron.in.V = neuron.instp->inChn.bias;
        neuron.out.I = neuron.instp->outChn.bias;
        neuron.in.spike_detect(t);
    }
}

std::pair<int, int> HHNeuronModel::numActiveInst()
{
    int nInst = 0;
    for ( HHNeuron &neuron : inst )
        if ( neuron.instp->active )
            nInst++;
    return std::pair<int, int>(p->active, nInst);
}

QVector<QString> HHNeuronModel::labels(int modelNo)
{
    QVector<QString> ret;
    QString in = QString("HH%1-V%2").arg(modelNo);
    QString out = QString("HH%1-I%2").arg(modelNo);
    int i = 0;
    for ( HHNeuron &neuron : inst ) {
        if ( neuron.in.save )
            ret.push_back(in.arg(i));
        if ( neuron.out.save )
            ret.push_back(out.arg(i));
        ++i;
    }
    return ret;
}

void HHNeuronModel::putData(QVector<double>::iterator &iter)
{
    for ( HHNeuron &neuron : inst ) {
        if ( neuron.in.save )
            *(iter++) = neuron.in.V;
        if ( neuron.out.save )
            *(iter++) = neuron.out.I;
    }
}
