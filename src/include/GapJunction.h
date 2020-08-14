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

#ifndef GAPJUNCTION_H
#define GAPJUNCTION_H

#include "Global_func.h"
#include "Channels.h"
#include "ConductanceManager.h"

class GapJunction;

struct GJunctData : public ConductanceData {
    int type;
    double gSyn;
    inline const GapJunctionAssignment &assignment(size_t i) const { return assign[i]; }
    inline size_t numAssignments() const { return assign.size(); }
    std::vector<GapJunctionAssignment> assign;
    int legacy_PreIn = -1;
    int legacy_PostIn= -1;
    int legacy_PreOut = -1;
    int legacy_PostOut = -1;
};

class GapJunctionProxy : public ConductanceProxy
{
private:
    GapJunctionProxy();
public:
    GapJunctionProxy(const GapJunctionProxy &) = delete;
    void operator=(const GapJunctionProxy &) = delete;
    static GapJunctionProxy *get() { static GapJunctionProxy proxy; return &proxy; }

    inline GJunctData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "ESyn"; }
    inline QString prettyName() { return "Gap Junction"; }

    void instantiate(size_t conductanceID, size_t assignID, DCThread *, ConductanceManager *manager);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<GJunctData> p;
};

class GapJunction : public Conductance {
  private:
    const GJunctData *p;
    const GapJunctionAssignment *a;
    
  protected:
    double I;
    inChannel *pre;
    inChannel *post;
    outChannel *outpre;
    outChannel *outpost;
        
  public:
    GapJunction(size_t condID, size_t assignID, size_t multiID, inChannel *pre, outChannel *outpre, inChannel *post, outChannel *outpost);
    void step(double t, double dt, bool settling);
    inline void RK4(double, double, size_t, bool) {}

    inline const GapJunctionAssignment &assignment() const { return params().assignment(assignID); }
    inline const GJunctData &params() const { return GapJunctionProxy::p[condID]; }
    GapJunctionProxy *proxy() const;
};

#endif
