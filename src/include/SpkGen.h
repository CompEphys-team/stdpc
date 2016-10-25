#ifndef SPKGEN_H
#define SPKGEN_H

#include "Global_func.h"
#include "ObjectDataTypes.h"
#include "Channels.h"
#include <QObject>
#include <QPair>
#include <memory>

class SpkGen : public QObject
{
    Q_OBJECT
    friend class SpkGenPrototype;
public:
    SpkGen(SGData *, SgInstData *, DCThread *);
    ~SpkGen() {}

    void update(double t, double dt);

    inChannel in;

signals:
    void message(QString message);

protected:
    SGData *p;
    SgInstData *instp;
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

class SpkGenPrototype
{
public:
    SpkGenPrototype(SGData *, DCThread *);
    ~SpkGenPrototype() {}

    void updateInstances(double t, double dt);
    void updateChannels(double t);

    std::pair<int, int> numActiveInst();
    QPair<QVector<QString>, QVector<inChannel*>> inChans_to_save(int modelNo);
    QPair<QVector<QString>, QVector<outChannel*>> outChans_to_save(int modelNo);

    std::vector<std::shared_ptr<SpkGen>> inst;

private:
    SGData *p;
};

#endif
