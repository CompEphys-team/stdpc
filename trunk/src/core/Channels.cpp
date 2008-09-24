#include "Channels.h"

inChannel::inChannel() { 
}

void inChannel::init(inChnData *inp)
{
  p= inp;
  V= 0.0;
  startSpiking= 0;
  spiking= 0;
  lastSpikeN= 0;
  for (int i= 0; i < MAX_ST_HISTORY; i++) {
    lastSpike[i]= -1.0;    // initialize with negative numbers to indicate no spikes
  }
}     

void inChannel::spike_detect(double t)
{
  if (p->spkDetect) {
    if (spiking) {
      startSpiking= false;
      if (V < p->spkDetectThresh) spiking= false; 
    }
    else {
      if (V > p->spkDetectThresh) {
        startSpiking= true;
        lastSpike[(lastSpikeN++)%MAX_ST_HISTORY]= t;
      }
    }
  }
}

outChannel::outChannel() 
{
  I= 0.0;
}

void outChannel::init(outChnData *inp) 
{
  p= inp;
}
