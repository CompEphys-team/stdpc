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

#include "ModelManager.h"

void ModelManager::initActive(DCThread *DCT)
{
    for ( std::shared_ptr<ModelPrototype> &m : activeModels )
        m->init(DCT);
}

void ModelManager::clear()
{
    activeModels.clear();
    allModels.clear();
    for ( ModelProxy *proxy : Register() )
        while ( proxy->size() )
            proxy->remove(proxy->size());
}

void ModelManager::initSingle(ModelProxy *proxy, size_t idx)
{
    QString const& mc = proxy->modelClass();

    // Clear existing active model
    for ( int i = 0; i < activeModels.size(); i++ )
        if ( activeModels[i]->proxy()->modelClass() == mc && activeModels[i]->modelID == idx )
            activeModels.remove(i);

    // Ensure enough space in allModels
    if ( allModels[mc].size() <= (int)idx )
        allModels[mc].resize(idx+1);

    // Assign new to allModels
    allModels[mc][idx].reset(proxy->createPrototype(idx));

    // Assign to activeModels if appropriate
    if ( proxy->param(idx).active ) {
        for ( size_t i = 0; i < proxy->param(idx).numInst(); i++ ) {
            if ( proxy->param(idx).instance(i).active ) {
                activeModels.push_back(allModels[mc][idx]);
                break;
            }
        }
    }
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
    if ( dex.isValid && dex.isVirtual
         && allModels[dex.modelClass].size() > (int)dex.modelID
         && allModels[dex.modelClass][dex.modelID]->instances().size() > dex.instID ) {
        return &(allModels[dex.modelClass][dex.modelID]->instances()[dex.instID]->in);
    }
    return nullptr;
}

outChannel *ModelManager::getOutChan(const ChannelIndex &dex)
{
    if ( dex.isValid && dex.isVirtual
         && allModels[dex.modelClass].size() > (int)dex.modelID
         && allModels[dex.modelClass][dex.modelID]->instances().size() > dex.instID ) {
        return &(allModels[dex.modelClass][dex.modelID]->instances()[dex.instID]->out);
    }
    return nullptr;
}
