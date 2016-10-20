#ifndef CHANNELS_H
#define CHANNELS_H

#include "Global.h"
#include "AECChannel.h"
#include "ChannelBufferHelper.h"
#include <memory>

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
     ~inChannel();

     void init(inChnData *);
     void process(double);

     size_t getBufferHandle(double duration, std::shared_ptr<ChannelBufferHelper> &);
     double getBufferedV(size_t handle);

private:
     double *Vbuf;
     size_t Vbufsize;
     std::shared_ptr<ChannelBufferHelper> bufferHelper;

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
