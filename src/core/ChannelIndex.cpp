#include "ChannelIndex.h"
#include "Global.h"

#define PROTOTYPE  1000000000
#define CLASSOFFSET 100000000
#define MODELOFFSET    100000

#define CLASS(x) ((x) - (x)%CLASSOFFSET)                  /* Just the class - compare to ModelClass */
#define MODEL(x) (((x)%CLASSOFFSET) / MODELOFFSET - 1)   /* Model number, use as index to *Neuronp */
#define INST(x) ((x) % MODELOFFSET)                       /* Instance number */

ChannelIndex::ChannelIndex() :
    isPrototype(false),
    isVirtual(false),
    isSG(false),
    isNone(false),
    inChn(false),
    modelClass(HH),
    modelID(0),
    instID(0),
    index(0)
{
}

ChannelIndex::ChannelIndex(int idx, bool inChn) :
    ChannelIndex()
{
    fromInt(idx, inChn);
}

void ChannelIndex::fromInt(int idx, bool in)
{
    index = idx;
    inChn = in;
    if ( idx == CHAN_SG || (idx == (int)inChnp.size() && inChn) ) {
        isPrototype = false;
        isVirtual = false;
        isSG = true;
        isNone = false;
    } else if ( idx == CHAN_NONE || (idx == (int)inChnp.size() && !inChn) ) {
        isPrototype = false;
        isVirtual = false;
        isSG = false;
        isNone = true;
    } else if ( idx < MODELOFFSET ) {
        isPrototype = false;
        isVirtual = false;
        isSG = false;
        isNone = false;
    } else {
        if ( (isPrototype = (idx >= PROTOTYPE)) ) {
            idx -= PROTOTYPE; // Remove prototype flag, leave: Class, Model, Inst
        }
        isVirtual = true;
        isSG = false;
        isNone = false;

        int c = CLASS(idx);
        modelClass = (ModelClass) c;
        idx -= c; // Remove class ID, leave: Model, Inst

        modelID = idx / MODELOFFSET - 1;
        instID = idx % MODELOFFSET;
    }
}

int ChannelIndex::toInt()
{
    if ( isSG ) {
        return index = CHAN_SG;
    } else if ( isNone ) {
        return index = CHAN_NONE;
    } else if ( isPrototype ) {
        return index = PROTOTYPE + (int)modelClass + (modelID+1)*MODELOFFSET;
    } else if ( isVirtual ) {
        return index = (int)modelClass + (modelID+1)*MODELOFFSET + instID;
    } else {
        return index;
    }
}

int ChannelIndex::toInstance(int instID)
{
    if ( isPrototype )
        return (int)modelClass + (modelID+1)*MODELOFFSET + instID;
    else
        return index;
}
