#ifndef CHANNELS_H
#define CHANNELS_H

#include "global.h"

class inChannel {
 private:
     inChnData *p;
      
 public:
     bool active;
     double V;
     bool startSpiking;
     bool spiking;
     int lastSpikeN;
     double lastSpike[MAX_ST_HISTORY];
     
     inChannel();
     void init(inChnData *);
     void spike_detect(double);
};


class outChannel {
 private:
     outChnData *p;
     
 public:
     bool active;
     double I;
     outChannel();
     void init(outChnData *);
};

#endif
