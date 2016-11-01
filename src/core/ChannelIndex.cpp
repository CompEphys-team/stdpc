#include "ChannelIndex.h"
#include <QStringList>
#include <QChar>
#include "Global.h"
#include "ModelManager.h"
#include "DeviceManager.h"

ChannelIndex::ChannelIndex(bool validNone) :
    isValid(validNone),
    isNone(validNone),
    isPrototype(false),
    isVirtual(false),
    modelClass(""),
    modelID(0),
    instID(0),
    isAnalog(false),
    daqClass(""),
    devID(0),
    isInChn(false),
    chanID(0)
{
}

ChannelIndex::ChannelIndex(DAQClass dClass, int dID, bool isIn, int cID) :
    isValid(true),
    isNone(false),
    isPrototype(false),
    isVirtual(false),
    isAnalog(true),
    devID(dID),
    isInChn(isIn),
    chanID(cID)
{
    switch ( dClass ) {
    case DAQClass::DD1200 : daqClass = "DigiData1200"; break;
    case DAQClass::NI :     daqClass = "NIDAQ";        break;
    case DAQClass::Simul :  daqClass = "SimulDAQ";     break;
    }
}

ChannelIndex::ChannelIndex(ModelClass mClass, int mID, int iID) :
    isValid(true),
    isNone(false),
    isPrototype(iID == -1),
    isVirtual(iID >= 0),
    modelID(mID),
    instID(iID == -1 ? 0 : iID),
    isAnalog(false)
{
    switch ( mClass ) {
    case ModelClass::HH : modelClass = "HH"; break;
    case ModelClass::SG : modelClass = "SG"; break;
    }
}

ChannelIndex::ChannelIndex(ChannelIndex::ctorType type, QString typeClass, size_t first, size_t second, bool isInChn) :
    isValid(true),
    isNone(type==None),
    isPrototype(type==Prototype),
    isVirtual(type==Virtual),
    modelClass(typeClass),
    modelID(first),
    instID(second),
    isAnalog(type==Analog),
    daqClass(typeClass),
    devID(first),
    isInChn(isInChn),
    chanID(second)
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
    } else if ( isAnalog ) {
        ret = QString("%1 %2 on %3, device %4").arg(isInChn ? "AI" : "AO").arg(chanID).arg(daqClass).arg(devID);
    } else if ( isPrototype || isVirtual ) {
        if ( isPrototype )
            ret = QString("%1 %2:all (model %2, all instances)").arg(modelClass).arg(modelID);
        else
            ret = QString("%1 %2:%3 (model %2, instance %3)").arg(modelClass).arg(modelID).arg(instID);
    } else ret = "Oops: Invalid channel index";
    return ret;
}

QString ChannelIndex::toString(QChar sep) const
{
    QString ret;
    if ( !isValid || isNone ) {
        ret = "None";
    } else if ( isAnalog ) {
        ret = QString("Analog/%1/%2/%3%4").arg(daqClass).arg(devID).arg(isInChn ? "ai" : "ao").arg(chanID);
    } else if ( isPrototype ) {
        ret = QString("Prototype/%1/%2").arg(modelClass).arg(modelID);
    } else if ( isVirtual ) {
        ret = QString("Virtual/%1/%2/%3").arg(modelClass).arg(modelID).arg(instID);
    }
    if ( sep != QChar('/') )
        ret.replace('/', sep);
    return ret;
}

std::ostream &operator<<(std::ostream &os, const ChannelIndex &dex)
{
    os << dex.toString().toStdString();
    return os;
}

static void legacyRescue_0(QString str, ChannelIndex &dex)
{
    // Attempt legacy rescue
    // Caveats: - inChn/outChn is unknowable; uses heuristics
    //          - Graph channels are likely to fail
    //          - Explicit NONE on pre-2016 collapses onto SG
    bool ok = false;
    int ch = str.toInt(&ok);
    if ( !ok )
        return;
    DAQData *d =& DeviceManager::Register()[LEGACY_DAQ_CLASS]->param(0);
    dex.daqClass = LEGACY_DAQ_CLASS;
    dex.devID = 0;
    if ( ch >= 0 && ch <= (int)(d->inChn.size()+d->outChn.size()+1) ) { // Pre-2016
        dex.chanID = ch;
        if ( ch < (int) d->inChn.size() && ch < (int)d->outChn.size() ) {
            dex.isAnalog = true;
            dex.isInChn = d->inChn.size() > d->outChn.size(); // Statistically...
        } else if ( d->inChn.size() >= d->outChn.size() && ch < (int)d->inChn.size() ) {
            dex.isAnalog = true;
            dex.isInChn = true;
        } else if ( d->inChn.size() < d->outChn.size() && ch < (int)d->outChn.size() ) {
            dex.isAnalog = true;
            dex.isInChn = false;
        } else {
            dex.isNone = true;
        }
        dex.isValid = true; // One hopes.
    } else if ( ch == -1 ) { // 2016-RC1 : None
        dex.isNone = true;
        dex.isValid = true;
    } else if ( ch == 1000 ) { // 2016-RC1 : SG
        dex.isVirtual = true;
        dex.modelClass = "SG";
        dex.modelID = 0;
        dex.instID = 0;
        dex.isValid = true;
    } else if ( ch >= 1100000000 ) { // 2016-RC1 : HH Proto
        dex.isPrototype = true;
        dex.modelClass = "HH";
        dex.modelID = ((ch - 1100000000) / 100000) - 1;
        dex.isValid = true;
    } else if ( ch >= 100000000 ) { // 2016-RC1 : HH Inst
        dex.isVirtual = true;
        dex.modelClass = "HH";
        dex.modelID = ((ch - 100000000) / 100000) - 1;
        dex.instID = ch % 100000;
        dex.isValid = true;
    }
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
            dex.daqClass = parts.at(2);
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
    } else if ( !parts.at(0).compare("Prototype", Qt::CaseInsensitive) ) {
        if ( parts.size() != 3 )
            return is;

        if ( ModelManager::Register.contains(parts.at(1)) )
            dex.modelClass = parts.at(1);
        else
            return is;

        dex.isPrototype = true;
        dex.modelID = parts.at(2).toUInt();
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Virtual", Qt::CaseInsensitive) ) {
        if ( parts.size() != 4 )
            return is;

        if ( ModelManager::Register.contains(parts.at(1)) )
            dex.modelClass = parts.at(1);
        else
            return is;

        dex.isVirtual = true;
        dex.modelID = parts.at(2).toUInt();
        dex.instID = parts.at(3).toUInt();
        dex.isValid = true;
    } else if ( parts.size() == 1 && LOADED_PROTOCOL_VERSION == 0 ) {
        legacyRescue_0(str, dex);
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
            (!a.isPrototype || (a.modelClass==b.modelClass && a.modelID==b.modelID)) &&
            (!a.isVirtual || (a.modelClass==b.modelClass && a.modelID==b.modelID && a.instID==b.instID)) &&
            a.isAnalog==b.isAnalog &&
            (!a.isAnalog || (a.daqClass==b.daqClass && a.devID==b.devID && a.isInChn==b.isInChn && a.chanID==b.chanID));
}
