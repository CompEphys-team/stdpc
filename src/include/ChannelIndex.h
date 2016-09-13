#ifndef CHANNELINDEX_H
#define CHANNELINDEX_H

#include <iostream>
#include <QString>
#include <QMetaType>

// Forward
struct inChnData;
struct outChnData;

enum class ModelClass { HH };
enum class DAQClass { Simul, DD1200, NI };

class ChannelIndex
{
public:
    ChannelIndex();

    ChannelIndex toInstance(int instID) const;

    inChnData *getInChnData() const;
    outChnData *getOutChnData() const;

    QString toString(QChar sep = '/') const;
    QString prettyName() const;

    bool isValid;

    bool isNone;

    bool isSG;

    bool isPrototype;
    bool isVirtual;
    ModelClass modelClass;
    int modelID;
    int instID;

    bool isAnalog;
    DAQClass daqClass;
    int devID;
    bool isInChn;
    int chanID;

    friend std::ostream &operator<<(std::ostream &os, ChannelIndex const&);
    friend std::istream &operator>>(std::istream &is, ChannelIndex &);

    friend bool operator==(ChannelIndex const& a, ChannelIndex const& b);
};

Q_DECLARE_METATYPE(ChannelIndex)

#endif // CHANNELINDEX_H
