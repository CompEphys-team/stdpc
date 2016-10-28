#ifndef SPKGEN_H
#define SPKGEN_H

#include "Model.h"
#include "Global_func.h"
#include "Global.h"

class SpkGen : public Model
{
public:
    SpkGen(ModelPrototype *, int, DCThread *);
    ~SpkGen() {}

    // Override processing on unused out
    inline void updateOut(double) {}

    void update(double t, double dt);
    void RK4(double,double,size_t);

protected:
    const SGData * const p;
    const SgInstData * const instp;
    double V;

    stdpc::function *theExp;
    stdpc::function *theTanh;
    inChannel *bdChn;
    bool SGactive;
    bool burstDetected;
    double tOverThresh;
    double tUnderThresh;
    bool onThreshold;
    int burstNo;
    double ISI_time;
    double period;
    bool initial;
    bool active;

    double VSpike(double t);
};

class SpkGenPrototype : public ModelPrototype
{
public:
    SpkGenPrototype(int modelID) : ModelPrototype(modelID) {}
    ~SpkGenPrototype() {}

    void init(DCThread *);

    // Override processing on unused Model::out
    inline void retainCurrent(double) {}
    inline void restoreCurrent(double) {}

    inline ModelData &params() const { return SGp[modelID]; }
    inline ModelClass modelClass() const { return SGData::modelClass; }
    inline QString prefix() const { return "SG"; }
};

#endif
