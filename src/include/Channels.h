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

#ifndef CHANNELS_H
#define CHANNELS_H

#include "Global.h"
#include "AECChannel.h"
#include "ChannelBufferHelper.h"
#include <memory>

// Forward declaration for synapse/current ctors
class DCThread;

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
