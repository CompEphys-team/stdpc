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

#ifndef CHANNELSINUSOID_H
#define CHANNELSINUSOID_H

#include "channeltransform.h"

struct ChannelSinusoidData : public ChannelTransformData
{
    double slope;
    double offset;
};

class ChannelSinusoidProxy : public ChannelTransformProxy
{
private:
    ChannelSinusoidProxy();
public:
    ChannelSinusoidProxy(const ChannelSinusoidProxy &) = delete;
    void operator=(const ChannelSinusoidProxy &) = delete;
    static ChannelSinusoidProxy *get() { static ChannelSinusoidProxy proxy; return &proxy; }

    inline ChannelSinusoidData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "ChannelSinusoid"; }
    inline QString prettyName() { return "Sinusoidal transform"; }

    ChannelTransform *createAssigned(size_t conductanceID, size_t assignID, inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<ChannelSinusoidData> p;
};

class ChannelSinusoid : public ChannelTransform
{
private:
  const ChannelSinusoidData *p;
  const ChannelTransformAssignment *a;

public:
  ChannelSinusoid(size_t condID, size_t assignID, inChannel *in, outChannel *out);
  void step(double t, double dt, bool settling);

  inline const ChannelSinusoidData &params() const { return ChannelSinusoidProxy::p[condID]; }
  ChannelSinusoidProxy *proxy() const;
};

#endif // CHANNELSINUSOID_H
