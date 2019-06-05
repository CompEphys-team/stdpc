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

#include "HH.h"
#include "DCThread.h"

HH::HH(mhHHData *inp, CurrentAssignment *a, inChannel *pre, outChannel *out) :
    p(inp),
    m(0.0),
    h(0.9),
    minf(0.0),
    taum(p->taum),
    hinf(0.9),
    tauh(p->tauh),
    pre(pre),
    out(out),
    a(a)
{
    if (p->LUTables) {
      theExp= &expLU;
      expLU.require(-1.0, 0.0, 0.001);
      theTanh= &tanhLU;
      tanhLU.require(-50.0, 50.0, 0.02);
      theExpSigmoid= &expSigmoidLU;
      expSigmoidLU.require(-50.0, 50.0, 0.02);
    }
    else {
      theExp= &expFunc;
      theTanh= &tanhFunc;
      theExpSigmoid= &expSigmoidFunc;
    }

    mi = m;
    hi = h;
}

void HH::currentUpdate(double, double dt)
{
  static double powm, powh;
  static double V, tmp;
  static int i;
    
  if (p->active && a->active && pre->active && out->active) {
    V= pre->V;
    if (p->mExpo > 0) {
      // linear Euler:
      // m+= (minf-m)*dt/taum;   // calculate m with previous values of minf, m, taum
      // exponential Euler:
      m= minf + (m - minf)*(*theExp)(-dt/taum);
      minf= (1.0-p->Cm)*(*theExpSigmoid)((V-p->Vm)/p->sm)+p->Cm;
      if (p->taumType == 0) {
          taum= p->taum - p->taumAmpl*(*theExpSigmoid)((V-p->Vtaum)/p->staum);
      } else if (p->taumType == 1) {
          tmp= (*theTanh)((V-p->Vtaum)/p->staum);
          taum= p->taum+p->taumAmpl*(1.0-tmp*tmp);
      } else if (p->taumType == 2) {
          tmp= (*theExp)((V-p->Vtaum)/p->staum);
          taum= p->taum + p->taumAmpl*tmp*minf;
      } else if (p->taumType == 3) {
          taum = p->taum + p->taumAmpl*V*1e3;
      }

      powm = m;
      for(i= 0; i < p->mExpo-1; i++) powm*= m;
    }
    else powm= 1.0;
    
    if (p->hExpo > 0){
      // linear Euler: 
      // h+= (hinf-h)*dt/tauh;   // calculate h with previous values of hinf, h, tauh
      // exponential Euler:
      h= hinf + (h - hinf)*(*theExp)(-dt/tauh);
      hinf= (1.0-p->Ch)*(*theExpSigmoid)((V-p->Vh)/p->sh)+p->Ch;
      if (p->tauhType == 0) {
          tauh= p->tauh - p->tauhAmpl*(*theExpSigmoid)((V-p->Vtauh)/p->stauh);
      } else if (p->tauhType == 1) {
          tmp= (*theTanh)((V-p->Vtauh)/p->stauh);
          tauh= p->tauh+p->tauhAmpl*(1.0-tmp*tmp);
      } else if (p->tauhType == 2) {
          tmp= (*theExp)((V-p->Vtauh)/p->stauh);
          tauh= p->tauh + p->tauhAmpl*tmp*hinf;
      } else if (p->tauhType == 3) {
          tauh = p->tauh + p->tauhAmpl*V*1e3;
      }
      powh= h;
      for(i= 0; i < p->hExpo-1; i++) powh*= h;   
    }
    else powh = 1.0;

    I= p->gMax*powm*powh*(p->Vrev - V);
    out->I+= I; 
  }
}

void HH::RK4(double, double dt, size_t n)
{
    static double powm, powh;
    static double V, tmp;
    static int i;

    if (p->active && a->active && pre->active && out->active) {
      V= pre->V;
      if (p->mExpo > 0) {
          minf = (1.0-p->Cm)*(*theExpSigmoid)((V-p->Vm)/p->sm)+p->Cm;
          switch ( p->taumType ) {
          case 0:
              taum = p->taum - p->taumAmpl*(*theExpSigmoid)((V-p->Vtaum)/p->staum);
              break;
          case 1:
              tmp= (*theTanh)((V-p->Vtaum)/p->staum);
              taum= p->taum+p->taumAmpl*(1.0-tmp*tmp);
              break;
          case 2:
              tmp= (*theExp)((V-p->Vtaum)/p->staum);
              taum= p->taum + p->taumAmpl*tmp*minf;
              break;
          case 3:
              taum = p->taum + p->taumAmpl*V*1e3;
              break;
          }
          km[n] = (minf-mi)/taum;

          powm = mi;
          for(i= 0; i < p->mExpo-1; i++) powm*= mi;

          if ( n < 3 ) {
              mi = m + km[n]*dt;
          } else {
              mi = m + (km[0] + 2*km[1] + 2*km[2] + km[3]) * dt / 6;
          }
          if ( mi < 0.0 ) mi = 0.0;
          if ( mi > 1.0 ) mi = 1.0;
          if ( n == 3 ) {
              m = mi;
          }
      }
      else powm= 1.0;

      if (p->hExpo > 0){
          hinf = (1.0-p->Ch)*(*theExpSigmoid)((V-p->Vh)/p->sh)+p->Ch;
          switch ( p->tauhType ) {
          case 0:
              tauh = p->tauh - p->tauhAmpl*(*theExpSigmoid)((V-p->Vtauh)/p->stauh);
              break;
          case 1:
              tmp= (*theTanh)((V-p->Vtauh)/p->stauh);
              tauh= p->tauh+p->tauhAmpl*(1.0-tmp*tmp);
              break;
          case 2:
              tmp= (*theExp)((V-p->Vtauh)/p->stauh);
              tauh= p->tauh + p->tauhAmpl*tmp*hinf;
              break;
          case 3:
              tauh = p->tauh + p->tauhAmpl*V*1e3;
              break;
          }
          kh[n] = (hinf-hi)/tauh;

          powh = hi;
          for(i= 0; i < p->hExpo-1; i++) powh*= hi;

          if ( n < 3 ) {
              hi = h + kh[n]*dt;
          } else {
              hi = h + (kh[0] + 2*kh[1] + 2*kh[2] + kh[3]) * dt / 6;
          }
          if ( hi < 0.0 ) hi = 0.0;
          if ( hi > 1.0 ) hi = 1.0;
          if ( n == 3 ) {
              h = hi;
          }
      }
      else powh = 1.0;

      I= p->gMax*powm*powh*(p->Vrev - V);
      out->I+= I;
    }
}
