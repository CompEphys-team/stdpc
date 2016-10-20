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
    double I = out.I + p->gLeak * (p->ELeak - V);
    V += I * dt / p->C;
    in.V = V;
}

HHNeuronModel::HHNeuronModel(HHNeuronData *p) :
    p(p)
{
    inst.reserve(p->inst.size());
    for ( vInstData &a : p->inst )
        inst.push_back(HHNeuron(p, &a));
}

void HHNeuronModel::updateNeurons(double t, double dt)
{
    if ( p->active )
        for ( HHNeuron &n : inst )
            if ( n.instp->active)
                n.update(t, dt);
}

void HHNeuronModel::updateChannels(double t)
{
    for ( HHNeuron &neuron : inst ) {
        if ( (neuron.in.active = neuron.out.active = p->active && neuron.instp->active) ) {
            neuron.in.V = neuron.V;
            neuron.out.I = neuron.instp->outChn.bias;
            neuron.in.process(t);
        }
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

QPair<QVector<QString>, QVector<inChannel *>> HHNeuronModel::inChans_to_save(int modelNo)
{
    QVector<QString> labels;
    QVector<inChannel*> chans;
    int i = 0;
    for ( HHNeuron &neuron : inst ) {
        if ( neuron.in.save ) {
            labels.push_back(QString("HH_%1_V%2").arg(modelNo).arg(i));
            chans.push_back(&neuron.in);
        }
        ++i;
    }
    return qMakePair(labels, chans);
}

QPair<QVector<QString>, QVector<outChannel *>> HHNeuronModel::outChans_to_save(int modelNo)
{
    QVector<QString> labels;
    QVector<outChannel*> chans;
    int i = 0;
    for ( HHNeuron &neuron : inst ) {
        if ( neuron.out.save ) {
            labels.push_back(QString("HH_%1_I%2").arg(modelNo).arg(i));
            chans.push_back(&neuron.out);
        }
        ++i;
    }
    return qMakePair(labels, chans);
}
