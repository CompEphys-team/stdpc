/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
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

#ifndef HH_H
#define HH_H

#include <cmath>
#include "ObjectDataTypes.h"
#include "Channels.h"
#include "LUtables.h"
#include "Global_func.h"

class HH
{
  private:
    mhHHData *p;
    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;
    
  protected:
    double m, h;
    double km[4], kh[4], mi, hi;
    double minf, taum, hinf, tauh;
    double I;
    inChannel *pre;
    outChannel *out;
    CurrentAssignment *a;
    
  public:
    HH(mhHHData *p, CurrentAssignment *a, inChannel *pre, outChannel *out);
    void currentUpdate(double, double);
    void RK4(double, double, size_t);

    typedef mhHHData param_type;
};

#endif
