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

#include "ChannelIndex.h"
#include <QStringList>
#include <QChar>
#include "Global.h"
#include "ModelManager.h"
#include "DeviceManager.h"
#include "ConductanceManager.h"

ChannelIndex::ChannelIndex() :
    isValid(false),
    isNone(false),
    isPrototype(false),
    isVirtual(false),
    modelClass(""),
    modelID(0),
    instID(0),
    isAnalog(false),
    isDigital(false),
    daqClass(""),
    devID(0),
    isInChn(true),
    chanID(0),
    isConductance(false),
    conductanceID(0),
    assignID(0),
    multiplexID(0),
    isLegacy(false)
{

}

ChannelIndex::ChannelIndex(DAQProxy *proxy, size_t devID, size_t chanID, bool isInChn) :
    isValid(proxy != nullptr),
    isNone(false),
    isPrototype(false),
    isVirtual(false),
    isAnalog(proxy != nullptr),
    isDigital(false),
    daqClass(proxy->daqClass()),
    devID(devID),
    isInChn(isInChn),
    chanID(chanID),
    isConductance(false),
    isLegacy(false)
{}

ChannelIndex::ChannelIndex(ModelProxy *proxy, size_t modelID) :
    isValid(proxy != nullptr),
    isNone(false),
    isPrototype(true),
    isVirtual(false),
    modelClass(proxy->modelClass()),
    modelID(modelID),
    instID(0),
    isAnalog(false),
    isDigital(false),
    isConductance(false),
    isLegacy(false)
{}

ChannelIndex::ChannelIndex(ModelProxy *proxy, size_t modelID, size_t instID, bool isInChn) :
    isValid(proxy != nullptr),
    isNone(false),
    isPrototype(false),
    isVirtual(true),
    modelClass(proxy->modelClass()),
    modelID(modelID),
    instID(instID),
    isAnalog(false),
    isDigital(false),
    isInChn(isInChn),
    isConductance(false),
    isLegacy(false)
{}

ChannelIndex::ChannelIndex(ConductanceProxy *proxy, size_t conductanceID, size_t assignID, size_t multiplexID) :
    isValid(proxy != nullptr),
    isNone(false),
    isPrototype(false),
    isVirtual(false),
    isAnalog(false),
    isDigital(false),
    isConductance(true),
    conductanceClass(proxy->conductanceClass()),
    conductanceID(conductanceID),
    assignID(assignID),
    multiplexID(multiplexID),
    isLegacy(false)
{}

ChannelIndex ChannelIndex::toInstance(size_t instID) const
{
    if ( !isPrototype || !isValid )
        return *this;
    ChannelIndex dex(*this);
    dex.isPrototype = false;
    dex.isVirtual = true;
    dex.instID = instID;
    return dex;
}

inChnData *ChannelIndex::getInChnData() const
{
    if ( !isValid || !isAnalog || !isInChn )
        return nullptr;
    DAQProxy *proxy = DeviceManager::Register().value(daqClass);
    if ( proxy && proxy->size() > devID && proxy->param(devID).inChn.size() > chanID )
        return &(proxy->param(devID).inChn[chanID]);
    return nullptr;
}

outChnData *ChannelIndex::getOutChnData() const
{
    if ( !isValid || !isAnalog || isInChn )
        return nullptr;
    DAQProxy *proxy = DeviceManager::Register().value(daqClass);
    if ( proxy && proxy->size() > devID && proxy->param(devID).outChn.size() > chanID )
        return &(proxy->param(devID).outChn[chanID]);
    return nullptr;
}

QString ChannelIndex::prettyName() const
{
    QString ret;
    if ( !isValid ) {
        return ret;
    } else if ( isNone ) {
        ret = "None";
    } else if ( isAnalog || isDigital ) {
        QString label;
        DAQProxy *proxy = Devices.Register().value(daqClass);
        if ( proxy->size() > devID )
            label = proxy->param(devID).label;
        if ( label.isEmpty() )
            label = QString("%1 %2").arg(proxy->prettyName()).arg(devID);
        if ( isAnalog )
            ret = QString("%1 %2 on %3").arg(isInChn ? "AI" : "AO").arg(chanID).arg(label);
        else if ( isDigital )
            ret = QString("%1 %2 on %3").arg(isInChn ? "DI" : "DO").arg(chanID).arg(label);
    } else if ( isPrototype || isVirtual ) {
        QString label;
        ModelProxy *proxy = Models.Register().value(modelClass);
        if ( proxy->size() > modelID )
            label = proxy->param(modelID).label;
        if ( label.isEmpty() )
            label = QString("%1 %2").arg(proxy->prettyName()).arg(modelID);
        if ( isPrototype )
            ret = QString("%1 %2:all (%4, all instances)").arg(modelClass).arg(modelID).arg(label);
        else
            ret = QString("%1 %2:%3 (%4, instance %3)").arg(modelClass).arg(modelID).arg(instID).arg(label);
        if ( isDirectional )
            ret.append(isInChn ? " VChan" : " IChan");
    } else if ( isConductance ) {
        QString label;
        ConductanceProxy *proxy = Conductances.Register().value(conductanceClass);
        if ( proxy->size() > conductanceID )
            label = proxy->param(conductanceID).label;
        if ( label.isEmpty() )
            label = QString("%1 %2").arg(proxy->prettyName()).arg(conductanceID);
        ret = QString("%1, assignment %2").arg(label).arg(assignID);
    } else ret = "Oops: Invalid channel index";
    return ret;
}

QString ChannelIndex::toString(QChar sep, bool withDetails) const
{
    QString ret;
    if ( !isValid || isNone ) {
        ret = "None";
    } else if ( isAnalog ) {
        ret = QString("Analog/%1/%2/%3%4").arg(daqClass).arg(devID).arg(isInChn ? "ai" : "ao").arg(chanID);
    } else if ( isDigital ) {
        ret = QString("Digital/%1/%2/%3%4").arg(daqClass).arg(devID).arg(isInChn ? "di" : "do").arg(chanID);
    } else if ( isPrototype ) {
        ret = QString("Prototype/%1/%2").arg(modelClass).arg(modelID);
        if ( isDirectional )
            ret.append(isInChn ? "/in" : "/out");
    } else if ( isVirtual ) {
        QString direction = "";
        if ( withDetails || isDirectional )
            direction = (isInChn ? "/in" : "/out");
        ret = QString("Virtual/%1/%2/%3%4").arg(modelClass).arg(modelID).arg(instID).arg(direction);
    } else if ( isConductance ) {
        QString multi = "";
        if ( withDetails )
            multi = QString("/multi%1").arg(multiplexID);
        ret = QString("Conductance/%1/%2/%3%4").arg(conductanceClass).arg(conductanceID).arg(assignID).arg(multi);
    }
    if ( sep != QChar('/') )
        ret.replace('/', sep);
    return ret;
}

QJsonObject ChannelIndex::toJson() const
{
    QJsonObject obj;
    if ( !isValid || isNone ) {
        return obj;
    } else if ( isAnalog || isDigital ) {
        obj = QJsonObject {
            {"type", "DAQ"},
            {"class", daqClass},
            {"class_id", int(devID)},
            {"channel_id", int(chanID)},
            {"is_input_channel", isInChn}
        };
        if ( isAnalog )
            obj.insert("units", isInChn ? "V" : "A");
        else
            obj.insert("units", "bit");
        DAQProxy *proxy = Devices.Register().value(daqClass);
        if ( proxy->size() > devID && !proxy->param(devID).label.isEmpty() )
            obj.insert("label", proxy->param(devID).label);
        return obj;
    } else if ( isPrototype ) {
        obj = QJsonObject {
            {"type", "Prototype"},
            {"class", modelClass},
            {"class_id", int(modelID)}
        };
        if ( isDirectional )
            obj.insert("is_input_channel", isInChn);
        ModelProxy *proxy = Models.Register().value(modelClass);
        if ( proxy->size() > modelID && !proxy->param(modelID).label.isEmpty() )
            obj.insert("label", proxy->param(modelID).label);
    } else if ( isVirtual ) {
        obj = QJsonObject {
            {"type", "Virtual"},
            {"class", modelClass},
            {"class_id", int(modelID)},
            {"instance_id", int(instID)},
            {"is_input_channel", isInChn},
            {"units", isInChn ? "V" : "A"}
        };
        ModelProxy *proxy = Models.Register().value(modelClass);
        if ( proxy->size() > modelID && !proxy->param(modelID).label.isEmpty() )
            obj.insert("label", proxy->param(modelID).label);
    } else if ( isConductance ) {
        obj = QJsonObject {
            {"type", "Conductance"},
            {"class", conductanceClass},
            {"class_id", int(conductanceID)},
            {"assignment_id", int(assignID)},
            {"multiplex_id", int(multiplexID)},
            {"units", "S"}
        };
        ConductanceProxy *proxy = Conductances.Register().value(conductanceClass);
        if ( proxy->size() > conductanceID && !proxy->param(conductanceID).label.isEmpty() )
            obj.insert("label", proxy->param(conductanceID).label);
    }
    return obj;
}

std::ostream &operator<<(std::ostream &os, const ChannelIndex &dex)
{
    os << dex.toString().toStdString();
    return os;
}

std::istream &operator>>(std::istream &is, ChannelIndex &dex)
{
    ChannelIndex blank;
    dex = blank;
    std::string stdstr;
    is >> stdstr;
    QString str = QString::fromStdString(stdstr);
    QStringList parts = str.split("/");
    if ( !parts.at(0).compare("None", Qt::CaseInsensitive) ) {
        dex.isNone = true;
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Analog", Qt::CaseInsensitive) ) {
        if ( parts.size() != 4 )
            return is;

        if ( DeviceManager::Register().contains(parts.at(1)) )
            dex.daqClass = parts.at(1);
        else
            return is;

        if ( parts.at(3).startsWith("ai", Qt::CaseInsensitive) )
            dex.isInChn = true;
        else if ( parts.at(3).startsWith("ao", Qt::CaseInsensitive) )
            dex.isInChn = false;
        else
            return is;

        dex.isAnalog = true;
        dex.devID = parts.at(2).toUInt();
        dex.chanID = parts.at(3).mid(2).toUInt();
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Digital", Qt::CaseInsensitive) ) {
        if ( parts.size() != 4 )
            return is;

        if ( DeviceManager::Register().contains(parts.at(1)) )
            dex.daqClass = parts.at(1);
        else
            return is;

        if ( parts.at(3).startsWith("di", Qt::CaseInsensitive) )
            dex.isInChn = true;
        else if ( parts.at(3).startsWith("do", Qt::CaseInsensitive) )
            dex.isInChn = false;
        else
            return is;

        dex.isDigital = true;
        dex.devID = parts.at(2).toUInt();
        dex.chanID = parts.at(3).mid(2).toUInt();
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Prototype", Qt::CaseInsensitive) ) {
        if ( parts.size() != 3 && parts.size() != 4 )
            return is;

        if ( ModelManager::Register().contains(parts.at(1)) )
            dex.modelClass = parts.at(1);
        else
            return is;

        dex.isPrototype = true;
        dex.modelID = parts.at(2).toUInt();

        if ( parts.size() == 4 ) {
            dex.isDirectional = true;
            if ( parts.at(3).startsWith("in", Qt::CaseInsensitive) )
                dex.isInChn = true;
            else if ( parts.at(3).startsWith("out", Qt::CaseInsensitive) )
                dex.isInChn = false;
            else
                return is;
        }

        dex.isValid = true;
    } else if ( !parts.at(0).compare("Virtual", Qt::CaseInsensitive) ) {
        if ( parts.size() != 4 && parts.size() != 5 )
            return is;

        if ( ModelManager::Register().contains(parts.at(1)) )
            dex.modelClass = parts.at(1);
        else
            return is;

        dex.isVirtual = true;
        dex.modelID = parts.at(2).toUInt();
        dex.instID = parts.at(3).toUInt();
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Conductance", Qt::CaseInsensitive) ) {
        if ( parts.size() != 4 )
            return is;

        if ( ConductanceManager::Register().contains(parts.at(1)) )
            dex.conductanceClass = parts.at(1);
        else
            return is;

        dex.isConductance = true;
        dex.conductanceID = parts.at(2).toUInt();
        dex.assignID = parts.at(3).toUInt();

        if ( parts.size() == 5 ) {
            dex.isDirectional = true;
            if ( parts.at(4).startsWith("in", Qt::CaseInsensitive) )
                dex.isInChn = true;
            else if ( parts.at(4).startsWith("out", Qt::CaseInsensitive) )
                dex.isInChn = false;
            else
                return is;
        }

        dex.isValid = true;
    } else if ( parts.size() == 1 && LOADED_PROTOCOL_VERSION == 0 ) {
        dex.isLegacy = true;
        dex.chanID = parts.at(0).toInt();
    } else if ( parts.size() == 1 && LOADED_PROTOCOL_VERSION < 3 ) {
        if ( !parts.at(0).compare("SpikeGen", Qt::CaseInsensitive) ) {
            dex.isVirtual = true;
            dex.modelClass = "SG";
            dex.modelID = 0;
            dex.instID = 0;
            dex.isValid = true;
        }
    }
    return is;
}

bool operator==(ChannelIndex const& a, ChannelIndex const& b)
{
    return a.isValid==b.isValid &&
            a.isNone==b.isNone &&
            a.isPrototype==b.isPrototype &&
            a.isVirtual==b.isVirtual &&
            (!a.isPrototype || (a.modelClass==b.modelClass && a.modelID==b.modelID && a.isInChn==b.isInChn && a.isDirectional==b.isDirectional)) &&
            (!a.isVirtual || (a.modelClass==b.modelClass && a.modelID==b.modelID && a.instID==b.instID && a.isInChn==b.isInChn && a.isDirectional==b.isDirectional)) &&
            a.isAnalog==b.isAnalog &&
            a.isDigital==b.isDigital &&
            (!(a.isAnalog || a.isDigital) || (a.daqClass==b.daqClass && a.devID==b.devID && a.isInChn==b.isInChn && a.chanID==b.chanID)) &&
            a.isConductance==b.isConductance &&
            (!a.isConductance || (a.conductanceClass==b.conductanceClass && a.conductanceID==b.conductanceID && a.assignID==b.assignID && a.multiplexID==b.multiplexID)) &&
            a.isLegacy == b.isLegacy;
}
