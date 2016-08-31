#ifndef CHANNELINDEX_H
#define CHANNELINDEX_H

#define CHAN_NONE -1
#define CHAN_SG 1000
#define OUTCHN_OFFSET 20000
#define INCHN_OFFSET  10000

class ChannelIndex
{
public:
    ChannelIndex();
    ChannelIndex(int rawIndex, bool inChn = true);

    void fromInt(int rawIndex, bool inChn = true);
    int toInt();
    int toInstance(int instID);

    bool isPrototype;
    bool isVirtual;
    bool isSG;
    bool isNone;
    bool inChn;

    enum ModelClass {
        HH = 100000000
    } modelClass;

    int modelID;
    int instID;

    int index;
};

#endif // CHANNELINDEX_H
