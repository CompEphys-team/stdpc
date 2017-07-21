#ifndef SYN_H
#define SYN_H

#include "ConductanceManager.h"
#include "Channels.h"

class Synapse : public Conductance
{
public:
    Synapse(size_t condID, size_t assignID, size_t multiID, inChannel *pre, inChannel *post, outChannel *out);
    ~Synapse() {}

    virtual const SynapseAssignment &assignment() const { return params().assignment(assignID); }
    virtual const SynapseData &params() const = 0;

    virtual void RK4(double, double, size_t, bool) {}
    virtual void step(double t, double dt, bool settling) = 0;

protected:
    inChannel *pre;
    inChannel *post;
    outChannel *out;

    void setupBuffer(DCThread *DCT);
    size_t bufferHandle;
    bool buffered;
};

class SynapseProxy : public ConductanceProxy
{
public:
    virtual SynapseData &param(size_t i) const = 0;
    void instantiate(size_t conductanceID, size_t assignID, DCThread *,
                    std::vector<Conductance*> &preD,
                    std::vector<Conductance*> &inD,
                    std::vector<Conductance*> &postD);

    virtual Synapse *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *,
                                    inChannel *pre, inChannel *post, outChannel *out) = 0;
};

#endif // SYN_H
