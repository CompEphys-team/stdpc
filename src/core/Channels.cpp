/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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

void inChannel::init(const inChnData *inp)
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

void outChannel::init(const outChnData *outp)
{
  p= outp;
  save = outp->chnlSaving;
}

void outChannel::reset()
{
  I= p->bias;
}
