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

#ifndef DESTEXHESYN_H
#define DESTEXHESYN_H

#include "Global_func.h"
#include "Channels.h"

class DestexheSyn {
  private:
    DestexheSynData *p;
    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;
    void learn();
    inChannel *pre;
    inChannel *post;
    outChannel *out;
    SynapseAssignment *a;

    size_t bufferHandle;
    bool buffered;

    double P_f(double);
    double D_f(double);
    double invGFilter(double);
    double gFilter(double);
    
  protected:
    double I;
    double S;
    double tlast;
    double g;
    double graw;
    double P;  // "potentiation var" in ODE STDP
    double D;  // "depression var" in ODE STDP
    double Pslope; // slope of the sigmoid for P
    double Dslope; // slope of the sigmoid for D

  public:
    DestexheSyn(DestexheSynData *, DCThread *, SynapseAssignment *, inChannel *pre, inChannel *post, outChannel *out);
    void currentUpdate(double, double);
    void STlearn(double);
    double STDPFunc(double);
    void ODElearn(double);

    typedef DestexheSynData param_type;
};

#endif
