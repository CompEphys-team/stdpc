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

#include "Model.h"
#include "DCThread.h"

Model::Model(ModelPrototype *parent, size_t instID, DCThread *DCT) :
    parent(parent),
    instID(instID)
{
    in.init(&(params().inChn));
    out.init(&(params().outChn));
    connect(this, SIGNAL(message(QString)), DCT, SIGNAL(message(QString)));
}

vInstData &Model::params() const
{
     return parent->params().instance(instID);
}

void ModelPrototype::reset()
{
    inst.clear();
}

void ModelPrototype::updateChannels(double t)
{
    for ( std::shared_ptr<Model> &m : inst ) {
        if ( (m->in.active = m->out.active = params().active && m->params().active) ) {
            m->updateIn(t);
            m->updateOut(t);
        }
    }
}

void ModelPrototype::retainCurrent(double t)
{
    if ( params().active )
        for ( std::shared_ptr<Model> &m : inst )
            if ( m->params().active )
                m->retainCurrent(t);
}

void ModelPrototype::restoreCurrent(double t)
{
    if ( params().active )
        for ( std::shared_ptr<Model> &m : inst )
            if ( m->params().active )
                m->restoreCurrent(t);
}

void ModelPrototype::RK4(double t, double dt, size_t n, bool settling)
{
    if ( params().active )
        for ( std::shared_ptr<Model> &m : inst )
            if ( params().instance(m->id()).active )
                m->RK4(t, dt, n, settling);
}

QString ModelPrototype::getStatus() const
{
    int nInst = 0;
    for ( size_t i = 0; i < params().numInst(); i++ )
        if ( params().instance(i).active )
            nInst++;
    return QString("Model %1 %2: %3 instance%4 active").arg(prefix()).arg(modelID).arg(nInst).arg(nInst>1 ? "s":"");
}

QPair<QVector<ChannelIndex>, QVector<const double *>> ModelPrototype::valuesToSave() const
{
    QVector<ChannelIndex> indices;
    QVector<const double *> values;
    for ( std::shared_ptr<Model> const& m : inst ) {
        if ( m->in.save ) {
            indices.push_back(ChannelIndex(proxy(), modelID, m->id(), true));
            values.push_back(&(m->in.V));
        }
        if ( m->out.save ) {
            indices.push_back(ChannelIndex(proxy(), modelID, m->id(), false));
            values.push_back(&(m->out.I));
        }
    }
    return qMakePair(indices, values);
}
