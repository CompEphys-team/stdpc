#ifndef SPKGEN_H
#define SPKGEN_H

#include "Global_func.h"
#include "Global.h"
#include "ModelManager.h"

class SpkGenProxy : public ModelProxy {
public:
    SpkGenProxy();
    inline ModelData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline QString modelClass() { return "SG"; }
    ModelPrototype *createPrototype(int modelID);

    static std::vector<SGData> p;
};

class SpkGen : public Model
{
public:
    SpkGen(ModelPrototype *, size_t, DCThread *);
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
    SpkGenPrototype(size_t modelID) : ModelPrototype(modelID) {}
    ~SpkGenPrototype() {}

    void init(DCThread *);

    // Override processing on unused Model::out
    inline void retainCurrent(double) {}
    inline void restoreCurrent(double) {}

    inline ModelData &params() const { return SGp[modelID]; }
    inline ModelProxy *proxy() const;
    inline QString prefix() const { return "SG"; }
};

#endif
