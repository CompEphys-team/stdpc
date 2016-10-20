#include "Channels.h"

inChannel::inChannel() :
    active(true),
    Vbuf(nullptr),
    Vbufsize(0)
{
}

inChannel::~inChannel()
{
    delete Vbuf;
}

void inChannel::init(inChnData *inp)
{
  p= inp;
  save = inp->chnlSaving;
  V= 0.0;
  startSpiking= 0;
  spiking= 0;
  lastSpikeN= 0;
  for (int i= 0; i < MAX_ST_HISTORY; i++) {
    lastSpike[i]= -1.0;    // initialize with negative numbers to indicate no spikes
  }

  delete Vbuf;
  Vbuf = nullptr;
  Vbufsize = 0;
}     

void inChannel::process(double t)
{
  V += p->bias;

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

  if ( Vbufsize ) {
      Vbuf[bufferHelper->absOffset() % Vbufsize] = V;
  }
}

size_t inChannel::getBufferHandle(double duration, std::shared_ptr<ChannelBufferHelper> &helper)
{
    bufferHelper = helper;
    size_t reqSize = bufferHelper->requiredSize(duration);
    if ( reqSize > Vbufsize ) {
        Vbufsize = reqSize;
        delete Vbuf;
        Vbuf = new double[Vbufsize]();
    }
    return bufferHelper->getHandle(duration);
}

double inChannel::getBufferedV(size_t handle)
{
    size_t offset = bufferHelper->offset(handle);
    if ( offset > Vbufsize ) // Overflow
        offset = Vbufsize;
    return Vbuf[(bufferHelper->absOffset() - offset) % Vbufsize];
}

outChannel::outChannel() : active(true)
{
  I= 0.0;
}

void outChannel::init(outChnData *outp)
{
  p= outp;
  save = outp->chnlSaving;
}

void outChannel::reset()
{
  I= p->bias;
}
