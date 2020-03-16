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

#ifndef AECCHANNEL_H
#define AECCHANNEL_H

#include <QVector>
#include "ChannelIndex.h"

class AECChannel
{
public:
    ChannelIndex inChnNum;
    ChannelIndex outChnNum;

    bool active;

    double kerSampPer;
    double kernelTime;
    double v_e;
    int buffInd;

    QVector<double> kernel;
    QVector<double> iBuffer;
    QVector<double> tBuffer;    

    AECChannel();

    void Activate();
    void Inactivate();
    bool IsActive();

    void Initialize(ChannelIndex inChnNum, ChannelIndex outChnNum, double kerSampPer, QVector<double> kernel);
    void ResetChannel();
    double CalculateVe(double current, double dt);

};

#endif // AECCHANNEL_H
