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

#ifndef LINEARTRANSFORM_H
#define LINEARTRANSFORM_H

#include "channeltransform.h"

struct LinearTransformData : public ChannelTransformData
{
    double factor;
    double offset;
};

class LinearTransformProxy : public ChannelTransformProxy
{
private:
    LinearTransformProxy();
public:
    LinearTransformProxy(const LinearTransformProxy &) = delete;
    void operator=(const LinearTransformProxy &) = delete;
    static LinearTransformProxy *get() { static LinearTransformProxy proxy; return &proxy; }

    inline LinearTransformData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "LinearTransform"; }
    inline QString prettyName() { return "Linear transform"; }

    ChannelTransform *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<LinearTransformData> p;
};

class LinearTransform : public ChannelTransform
{
private:
  const LinearTransformData *p;
  const ChannelTransformAssignment *a;

public:
  LinearTransform(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
  void step(double t, double dt, bool settling);

  inline const LinearTransformData &params() const { return LinearTransformProxy::p[condID]; }
  LinearTransformProxy *proxy() const;
};

#endif // LINEARTRANSFORM_H
