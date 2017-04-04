#ifndef CHANNELINDEX_H
#define CHANNELINDEX_H

#include <iostream>
#include <QString>
#include <QMetaType>

// Forward
struct inChnData;
struct outChnData;
class DAQProxy;
class ModelProxy;
class ConductanceProxy;

class ChannelIndex
{
public:
    ChannelIndex();
    ChannelIndex(DAQProxy *proxy, size_t devID=0, size_t chanID=0, bool isInChn=true);
    ChannelIndex(ModelProxy *proxy, size_t modelID=0);
    ChannelIndex(ModelProxy *proxy, size_t modelID, size_t instID);
    ChannelIndex(ConductanceProxy *proxy, size_t conductanceID, size_t assignID);
    static ChannelIndex None() { ChannelIndex i; i.isValid = true; i.isNone = true; return i; }

    ChannelIndex toInstance(size_t instID) const;

    inChnData *getInChnData() const;
    outChnData *getOutChnData() const;

    QString toString(QChar sep = '/') const;
    QString prettyName() const;

    bool isValid;

    bool isNone;

    bool isPrototype;
    bool isVirtual;
    QString modelClass;
    size_t modelID;
    size_t instID;

    bool isAnalog;
    QString daqClass;
    size_t devID;
    bool isInChn;
    size_t chanID;

    bool isConductance;
    QString conductanceClass;
    size_t conductanceID;
    size_t assignID;

    bool isLegacy;

    friend std::ostream &operator<<(std::ostream &os, ChannelIndex const&);
    friend std::istream &operator>>(std::istream &is, ChannelIndex &);

    friend bool operator==(ChannelIndex const& a, ChannelIndex const& b);
};

Q_DECLARE_METATYPE(ChannelIndex)

#endif // CHANNELINDEX_H
