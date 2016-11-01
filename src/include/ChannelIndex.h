#ifndef CHANNELINDEX_H
#define CHANNELINDEX_H

#include <iostream>
#include <QString>
#include <QMetaType>

// Forward
struct inChnData;
struct outChnData;

enum class ModelClass { HH, SG };
enum class DAQClass { Simul, DD1200, NI };

class ChannelIndex
{
public:
    enum ctorType { Prototype, Virtual, Analog, None };

    ChannelIndex(bool validNone = false);
    ChannelIndex(DAQClass dClass, int dID = 0, bool isIn = true, int cID = 0);
    ChannelIndex(ModelClass mClass, int mID = 0, int iID = 0); // iID = -1 to get a Prototype

    ChannelIndex(ctorType type, QString typeClass, size_t modelID_or_deviceID=0, size_t instID_or_chanID=0, bool isInChn=true);

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

    friend std::ostream &operator<<(std::ostream &os, ChannelIndex const&);
    friend std::istream &operator>>(std::istream &is, ChannelIndex &);

    friend bool operator==(ChannelIndex const& a, ChannelIndex const& b);
};

Q_DECLARE_METATYPE(ChannelIndex)

#endif // CHANNELINDEX_H
