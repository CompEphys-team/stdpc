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

#include "DeviceManager.h"
#include "Clock.h"
#include "Global.h"

void DeviceManager::clear()
{
    activeDAQ.clear();
    allDAQ.clear();
    for ( DAQProxy *proxy : Register() )
        while ( proxy->size() )
            proxy->remove(proxy->size()-1);
}

DeviceStatus DeviceManager::initSingle(QString &name, DAQProxy *proxy, size_t idx)
{
    QString const& dc = proxy->daqClass();

    // Clear existing active device
    for ( int i = 0; i < activeDAQ.size(); i++ )
        if ( activeDAQ[i]->proxy->daqClass() == dc && activeDAQ[i]->devID == idx )
            activeDAQ.remove(i);

    // Ensure enough space in allDAQ
    if ( allDAQ[dc].size() <= (int)idx )
        allDAQ[dc].resize(idx+1);

    // Assign new to allDAQ
    allDAQ[dc][idx].reset(proxy->createDAQ(idx));

    // Assign to activeDAQ if appropriate
    if ( proxy->param(idx).active ) {
        if ( allDAQ[dc][idx]->initialize_board(name) ) {
            activeDAQ.push_back(allDAQ[dc][idx]);
            return DeviceStatus::Active;
        } else {
            return DeviceStatus::Failed;
        }
    } else {
        return DeviceStatus::Inactive;
    }
}

void DeviceManager::remove(DAQProxy *proxy, size_t idx)
{
    QString const& dc = proxy->daqClass();
    if ( !allDAQ.contains(dc) )
        return;

    for ( int i = 0; i < activeDAQ.size(); i++ )
        if ( activeDAQ[i]->proxy->daqClass() == dc && activeDAQ[i]->devID == idx )
            activeDAQ.remove(i);

    QVector<std::shared_ptr<DAQ>> &vec = allDAQ[dc];
    if ( vec.size() > (int)idx ) {
        vec.remove(idx);
        proxy->remove(idx);
        for ( int i = idx; i < vec.size(); i++ )
            vec[i]->devID = i;
        emit removedDevice(ChannelIndex(proxy, idx));
    }
}

DAQ *DeviceManager::getDevice(const ChannelIndex &dex)
{
    if ( !dex.isValid || !(dex.isAnalog || dex.isDigital) )
        return nullptr;
    QVector<std::shared_ptr<DAQ>> &vec = allDAQ[dex.daqClass];
    if ( vec.size() > (int)dex.devID )
        return vec[dex.devID].get();
    return nullptr;
}

inChannel *DeviceManager::getInChan(const ChannelIndex &dex)
{
    if ( dex.isValid && dex.isAnalog && dex.isInChn ) {
        QVector<std::shared_ptr<DAQ>> &vec = allDAQ[dex.daqClass];
        if ( vec.size() > (int)dex.devID && vec[dex.devID]->in.size() > (int)dex.chanID )
            return &(vec[dex.devID]->in[dex.chanID]);
    }
    return nullptr;
}

outChannel *DeviceManager::getOutChan(const ChannelIndex &dex)
{
    if ( dex.isValid && dex.isAnalog && !dex.isInChn ) {
        QVector<std::shared_ptr<DAQ>> &vec = allDAQ[dex.daqClass];
        if ( vec.size() > (int)dex.devID && vec[dex.devID]->out.size() > (int)dex.chanID )
            return &(vec[dex.devID]->out[dex.chanID]);
    }
    return nullptr;
}
