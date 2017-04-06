#ifndef IONICCURRENT_H
#define IONICCURRENT_H

#include "ConductanceManager.h"
#include "Channels.h"

class IonicCurrent : public Conductance
{
public:
    IonicCurrent(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
    ~IonicCurrent() {}

    inline virtual const CurrentAssignment &assignment() const { return params().assignment(assignID); }
    virtual const CurrentData &params() const = 0;

    virtual void RK4(double t, double dt, size_t n) = 0;
    virtual void step(double t, double dt) = 0;

protected:
    inChannel *in;
    outChannel *out;
};

class IonicCurrentProxy : public ConductanceProxy
{
public:
    virtual CurrentData &param(size_t i) const = 0;
    void instantiate(size_t conductanceID, size_t assignID, DCThread *,
                     std::vector<Conductance*> &preD,
                     std::vector<Conductance*> &inD,
                     std::vector<Conductance*> &postD);

    virtual IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                         inChannel *in, outChannel *out) = 0;
};

#endif // IONICCURRENT_H
