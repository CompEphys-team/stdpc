#ifndef HHNEURON_H
#define HHNEURON_H

#include "ObjectDataTypes.h"
#include "Channels.h"
#include <QPair>

class HHNeuron
{
friend class HHNeuronModel;
public:
    HHNeuron(HHNeuronData *, vInstData *);
    void update(double t, double dt);

    inChannel in;
    outChannel out;
    double V;

protected:
    HHNeuronData *p;
    vInstData *instp;
};


class HHNeuronModel
{
public:
    HHNeuronModel(HHNeuronData *p);

    void updateNeurons(double t, double dt);
    void updateChannels(double t);

    std::pair<int, int> numActiveInst();
    QPair<QVector<QString>, QVector<inChannel*>> inChans_to_save(int modelNo);
    QPair<QVector<QString>, QVector<outChannel*>> outChans_to_save(int modelNo);

    std::vector<HHNeuron> inst;

private:
    HHNeuronData *p;
};

#endif // HHNEURON_H
