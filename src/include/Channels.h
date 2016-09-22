#ifndef CHANNELS_H
#define CHANNELS_H

#include "Global.h"
#include "AECChannel.h"

class inChannel {
 public:
    inChnData *p;
     bool active;
     bool save;
     double V;
     bool startSpiking;
     bool spiking;
     int lastSpikeN;
     double lastSpike[MAX_ST_HISTORY];
     AECChannel aec;
     
     inChannel();
     void init(inChnData *);
     void spike_detect(double);
};


class outChannel {
 public:
    outChnData *p;
     bool active;
     bool save;
     double I;
     outChannel();
     void init(outChnData *);
     void reset();
};

#endif
