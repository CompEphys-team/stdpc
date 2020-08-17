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

#include "Model.h"
#include "DCThread.h"

Model::Model(size_t modelID, size_t instID, DCThread *DCT) :
    modID(modelID),
    instID(instID)
{
    connect(this, SIGNAL(message(QString)), DCT, SIGNAL(message(QString)));
}

void Model::post_init(DCThread *)
{
    in.init(&(instance().inChn));
    out.init(&(instance().outChn));
}

void Model::updateChannels(double t)
{
    if ( (in.active = out.active = (params().active && instance().active)) ) {
        updateIn(t);
        updateOut(t);
    }
}
