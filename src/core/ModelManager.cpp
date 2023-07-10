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

#include "ModelManager.h"

void ModelManager::init(DCThread *DCT)
{
    activeModels.clear();
    for ( ModelProxy *proxy : Register() ) {
        for ( size_t j = 0; j < proxy->size(); j++ ) {
            if ( proxy->param(j).active ) {
                for ( size_t i = 0; i < proxy->param(j).numInst(); i++ ) {
                    if ( proxy->param(j).instance(i).active ) {
                        activeModels.emplace_back(proxy->instantiate(j, i, DCT));
                    }
                }
            }
        }
    }
    for ( auto &m : activeModels )
        m->post_init();
}

void ModelManager::clear()
{
    activeModels.clear();
    for ( ModelProxy *proxy : Register() )
        while ( proxy->size() )
            proxy->remove(proxy->size()-1);
}

bool ModelManager::empty() const
{
    for ( ModelProxy *proxy : Register() )
        for ( size_t i = 0; i < proxy->size(); i++ )
            if ( proxy->param(i).active )
                for ( size_t j = 0; j < proxy->param(i).numInst(); j++ )
                    if ( proxy->param(i).instance(j).active )
                        return true;
    return false;
}

inChannel *ModelManager::getInChan(const ChannelIndex &dex)
{
    if ( !dex.isValid || !dex.isVirtual )
        return nullptr;
    for ( auto const& m : activeModels )
        if ( m->proxy()->modelClass() == dex.modelClass && m->modelID() == dex.modelID && m->instanceID() == dex.instID )
            return &m->in;
    return nullptr;
}

outChannel *ModelManager::getOutChan(const ChannelIndex &dex)
{
    if ( !dex.isValid || !dex.isVirtual )
        return nullptr;
    for ( auto const& m : activeModels )
        if ( m->proxy()->modelClass() == dex.modelClass && m->modelID() == dex.modelID && m->instanceID() == dex.instID )
            return &m->out;
    return nullptr;
}

QStringList ModelManager::getStatus() const
{
    QStringList statuses;
    for ( ModelProxy *proxy : Register() ) {
        for ( size_t i = 0; i < proxy->size(); i++ ) {
            if ( proxy->param(i).active ) {
                int nInst = 0;
                for ( size_t j = 0; j < proxy->param(i).numInst(); j++ ) {
                    if ( proxy->param(i).instance(j).active )
                        ++nInst;
                }
                statuses.push_back(QString("Model %1 %2: %3 instance%4 active")
                                   .arg(proxy->modelClass()).arg(i).arg(nInst).arg(nInst>1 ? "s":""));
            }
        }
    }
    return statuses;
}

QPair<QVector<ChannelIndex>, QVector<const double *>> ModelManager::toSave() const
{
    QVector<ChannelIndex> indices;
    QVector<const double *> values;
    for ( std::shared_ptr<Model> const& m : activeModels ) {
        if ( m->in.save ) {
            indices.push_back(ChannelIndex(m->proxy(), m->modelID(), m->instanceID(), true));
            indices.last().isDirectional = true;
            values.push_back(&(m->in.V));
        }
        if ( m->out.save ) {
            indices.push_back(ChannelIndex(m->proxy(), m->modelID(), m->instanceID(), false));
            indices.last().isDirectional = true;
            values.push_back(&(m->out.I));
        }
    }
    return qMakePair(indices, values);
}
