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


#include "GapJunction.h"
#include "DCThread.h"

GapJunction::GapJunction(GJunctData *inp, GapJunctionAssignment *a, inChannel *pre, outChannel *outpre, inChannel *post, outChannel *outpost) :
    p(inp),
    pre(pre),
    post(post),
    outpre(outpre),
    outpost(outpost),
    a(a)
{
}

void GapJunction::currentUpdate(double, double)
{
  if ( p->active && a->active && pre->active && post->active && outpre->active && outpost->active ) {
      // calculate synaptic current
      I= p->gSyn * (pre->V - post->V);
      if ((p->type == 1) && (I < 0.0)) {
           I= 0.0;
           return;
      }
      outpost->I+= I;
      outpre->I-= I;
  }
}

