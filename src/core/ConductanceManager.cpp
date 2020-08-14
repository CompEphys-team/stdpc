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

#include "ConductanceManager.h"
#include "DCThread.h"

ConductanceManager::~ConductanceManager()
{
    for ( Conductance *c : preD )
        delete c;
    for ( Conductance *c : inD )
        delete c;
    for ( Conductance *c : postD )
        delete c;
}

void ConductanceManager::init(DCThread *DCT)
{
    for ( Conductance *c : preD )
        delete c;
    for ( Conductance *c : inD )
        delete c;
    for ( Conductance *c : postD )
        delete c;
    preD.clear();
    inD.clear();
    postD.clear();

    for ( ConductanceProxy *proxy : Register() ) {
        for ( size_t j = 0; j < proxy->size(); j++ ) {
            if ( proxy->param(j).active ) {
                for ( size_t i = 0; i < proxy->param(j).numAssignments(); i++ ) {
                    if ( proxy->param(j).assignment(i).active ) {
                        proxy->instantiate(j, i, DCT, this);
                    }
                }
            }
        }
    }
}

void ConductanceManager::clear()
{
    for ( Conductance *c : preD )
        delete c;
    for ( Conductance *c : inD )
        delete c;
    for ( Conductance *c : postD )
        delete c;
    preD.clear();
    inD.clear();
    postD.clear();
    for ( ConductanceProxy *proxy : Register() )
        while ( proxy->size() )
            proxy->remove(proxy->size()-1);
}

const double *match(const std::vector<Conductance*> ref, const ChannelIndex &dex)
{
    for ( Conductance *c : ref )
        if ( c->proxy()->conductanceClass() == dex.conductanceClass &&
             c->conductanceID() == dex.conductanceID &&
             c->assignmentID() == dex.assignID )
            return &c->conductance();
    return nullptr;
}

const double *ConductanceManager::conductance(const ChannelIndex &dex)
{
    const double *ret = nullptr;
    if ( !dex.isConductance )
        return ret;
    if ( (ret = match(preD, dex)) || (ret = match(inD, dex)) || (ret = match(postD, dex)) )
        return ret;
    return ret;
}

QStringList ConductanceManager::getStatus() const
{
    QStringList statuses;
    for ( ConductanceProxy *proxy : Register() ) {
        int euler = 0, rk4 = 0;
        for ( Conductance *c : preD )
            if ( c->proxy() == proxy )
                ++euler;
        for ( Conductance *c : inD )
            if ( c->proxy() == proxy )
                ++rk4;
        for ( Conductance *c : postD )
            if ( c->proxy() == proxy )
                ++euler;
        if ( euler+rk4 > 0 )
            statuses << QString("DC: %1 %2 conductances (%3 Euler, %4 Runge-Kutta)")
                        .arg(euler+rk4)
                        .arg(proxy->prettyName())
                        .arg(euler)
                        .arg(rk4);
    }
    return statuses;
}

QPair<QVector<ChannelIndex>, QVector<const double *>> ConductanceManager::toSave() const
{
    QVector<ChannelIndex> indices;
    QVector<const double *> values;
    for ( auto&& any : {preD, inD, postD} ) {
        for ( Conductance *c : any ) {
            if ( c->assignment().save ) {
                indices.push_back(ChannelIndex(c->proxy(), c->conductanceID(), c->assignmentID(), c->multiplexID()));
                values.push_back(&c->conductance());
            }
        }
    }
    return qMakePair(indices, values);
}
