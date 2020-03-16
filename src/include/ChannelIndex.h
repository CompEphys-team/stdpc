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

#ifndef CHANNELINDEX_H
#define CHANNELINDEX_H

#include <iostream>
#include <QString>
#include <QMetaType>
#include <QJsonObject>

// Forward
struct inChnData;
struct outChnData;
class DAQProxy;
class ModelProxy;
class ConductanceProxy;

class ChannelIndex
{
public:
    ChannelIndex();
    ChannelIndex(DAQProxy *proxy, size_t devID=0, size_t chanID=0, bool isInChn=true);
    ChannelIndex(ModelProxy *proxy, size_t modelID=0);
    ChannelIndex(ModelProxy *proxy, size_t modelID, size_t instID, bool isInChn=true);
    ChannelIndex(ConductanceProxy *proxy, size_t conductanceID, size_t assignID, size_t multiplexID=0);
    static ChannelIndex None() { ChannelIndex i; i.isValid = true; i.isNone = true; return i; }

    ChannelIndex toInstance(size_t instID) const;

    inChnData *getInChnData() const;
    outChnData *getOutChnData() const;

    QString toString(QChar sep = '/', bool withDetails = false) const;
    QString prettyName() const;
    QJsonObject toJson() const;

    bool isValid;

    bool isNone;

    bool isPrototype;
    bool isVirtual;
    QString modelClass;
    size_t modelID;
    size_t instID;

    bool isAnalog;
    bool isDigital;
    QString daqClass;
    size_t devID;
    bool isInChn;
    size_t chanID;

    bool isConductance;
    QString conductanceClass;
    size_t conductanceID;
    size_t assignID;
    size_t multiplexID;

    bool isLegacy;

    friend std::ostream &operator<<(std::ostream &os, ChannelIndex const&);
    friend std::istream &operator>>(std::istream &is, ChannelIndex &);

    friend bool operator==(ChannelIndex const& a, ChannelIndex const& b);
};

Q_DECLARE_METATYPE(ChannelIndex)

#endif // CHANNELINDEX_H
