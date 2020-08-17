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

#ifndef SINUSOIDALTRANSFORM_H
#define SINUSOIDALTRANSFORM_H

#include "channeltransform.h"

struct SinusoidalTransformData : public ChannelTransformData
{
    double slope;
    double offset;
};

class SinusoidalTransformProxy : public ChannelTransformProxy
{
private:
    SinusoidalTransformProxy();
public:
    SinusoidalTransformProxy(const SinusoidalTransformProxy &) = delete;
    void operator=(const SinusoidalTransformProxy &) = delete;
    static SinusoidalTransformProxy *get() { static SinusoidalTransformProxy proxy; return &proxy; }

    inline SinusoidalTransformData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "SinusoidalTransform"; }
    inline QString prettyName() { return "Sinusoidal transform"; }

    ChannelTransform *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<SinusoidalTransformData> p;
};

class SinusoidalTransform : public ChannelTransform
{
private:
  const SinusoidalTransformData *p;
  const ChannelTransformAssignment *a;

public:
  SinusoidalTransform(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
  void step(double t, double dt, bool settling);

  inline const SinusoidalTransformData &params() const { return SinusoidalTransformProxy::p[condID]; }
  SinusoidalTransformProxy *proxy() const;
};

#endif // SINUSOIDALTRANSFORM_H
