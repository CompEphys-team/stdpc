#ifndef HHNEURON_H
#define HHNEURON_H

#include "ObjectDataTypes.h"
#include "Channels.h"

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
    QVector<QString> labels(int modelNo);
    void putData(QVector<double>::iterator &iter);

    std::vector<HHNeuron> inst;

private:
    HHNeuronData *p;
};

#endif // HHNEURON_H
