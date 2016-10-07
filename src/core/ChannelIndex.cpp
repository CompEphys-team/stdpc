#include "ChannelIndex.h"
#include <QStringList>
#include <QChar>
#include "Global.h"

ChannelIndex::ChannelIndex() :
    isValid(false),
    isNone(false),
    isSG(false),
    isPrototype(false),
    isVirtual(false),
    modelClass(ModelClass::HH),
    modelID(0),
    instID(0),
    isAnalog(false),
    daqClass(DAQClass::Simul),
    devID(0),
    isInChn(false),
    chanID(0)
{
}

ChannelIndex ChannelIndex::toInstance(int instID) const
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
    switch ( daqClass ) {
    case DAQClass::Simul :
        if ( SDAQp.empty() || (int)SDAQp.size() <= devID || (int)SDAQp[devID].inChn.size() <= chanID )
            return nullptr;
        return &(SDAQp[devID].inChn[chanID]);
    case DAQClass::DD1200 :
        if ( DigiDatap.empty() || (int)DigiDatap.size() <= devID || (int)DigiDatap[devID].inChn.size() <= chanID )
            return nullptr;
        return &(DigiDatap[devID].inChn[chanID]);
    case DAQClass::NI :
#ifdef NATIONAL_INSTRUMENTS
        if ( NIDAQp.empty() || (int)NIDAQp.size() <= devID || (int)NIDAQp[devID].inChn.size() <= chanID )
            return nullptr;
        return &(NIDAQp[devID].inChn[chanID]);
#endif
    default:
        return nullptr;
    }
    return nullptr;
}

outChnData *ChannelIndex::getOutChnData() const
{
    if ( !isValid || !isAnalog || isInChn )
        return nullptr;
    switch ( daqClass ) {
    case DAQClass::Simul :
        if ( SDAQp.empty() || (int)SDAQp.size() <= devID || (int)SDAQp[devID].outChn.size() <= chanID )
            return nullptr;
        return &(SDAQp[devID].outChn[chanID]);
    case DAQClass::DD1200 :
        if ( DigiDatap.empty() || (int)DigiDatap.size() <= devID || (int)DigiDatap[devID].outChn.size() <= chanID )
            return nullptr;
        return &(DigiDatap[devID].outChn[chanID]);
    case DAQClass::NI :
#ifdef NATIONAL_INSTRUMENTS
        if ( NIDAQp.empty() || (int)NIDAQp.size() <= devID || (int)NIDAQp[devID].outChn.size() <= chanID )
            return nullptr;
        return &(NIDAQp[devID].outChn[chanID]);
#endif
    default:
        return nullptr;
    }
    return nullptr;
}

QString ChannelIndex::prettyName() const
{
    QString ret;
    if ( !isValid ) {
        return ret;
    } else if ( isNone ) {
        ret = "None";
    } else if ( isSG ) {
        ret = "Spike generator";
    } else if ( isAnalog ) {
        if ( isInChn )
            ret = QString("AI %1 on ").arg(chanID);
        else
            ret = QString("AO %1 on ").arg(chanID);
        switch ( daqClass ) {
        case DAQClass::Simul :  ret += "SimulDAQ";         break;
        case DAQClass::DD1200 : ret += "DigiData 1200(A)"; break;
        case DAQClass::NI :     ret += "NI";               break;
        }
        ret += QString(", device %1").arg(devID);
    } else if ( isPrototype || isVirtual ) {
        if ( isPrototype )
            ret = QString("%2 %1:all (model %1, all instances)").arg(modelID);
        else
            ret = QString("%3 %1:%2 (model %1, instance %2)").arg(modelID).arg(instID);
        switch ( modelClass ) {
        case ModelClass::HH : ret = ret.arg("HH"); break;
        }
    } else ret = "Oops: Invalid channel index";
    return ret;
}

QString ChannelIndex::toString(QChar sep) const
{
    QString ret;
    if ( !isValid || isNone ) {
        ret = "None";
    } else if ( isSG ) {
        ret = "SpikeGen";
    } else if ( isAnalog ) {
        ret = QString("Analog/");
        switch ( daqClass ) {
        case DAQClass::Simul :  ret += "SimulDAQ/";      break;
        case DAQClass::DD1200 : ret += "DigiData1200/";  break;
        case DAQClass::NI :     ret += "NIDAQ/";         break;
        }
        ret += QString::number(devID) + "/";
        if ( isInChn )
            ret += "ai" + QString::number(chanID);
        else
            ret += "ao" + QString::number(chanID);
    } else if ( isPrototype ) {
        ret = QString("Prototype/");
        switch ( modelClass ) {
        case ModelClass::HH : ret += "HH/"; break;
        }
        ret += QString::number(modelID);
    } else if ( isVirtual ) {
        ret += "Virtual/";
        switch ( modelClass ) {
        case ModelClass::HH : ret += "HH/"; break;
        }
        ret += QString::number(modelID) + "/" + QString::number(instID);
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
    DAQData *d;
    switch ( LEGACY_DAQ_CLASS ) {
    case DAQClass::Simul :  d =& SDAQp[0];     break;
    case DAQClass::DD1200 : d =& DigiDatap[0]; break;
#ifdef NATIONAL_INSTRUMENTS
    case DAQClass::NI :     d =& NIDAQp[0];    break;
#endif
    default: return;
    }
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
        } else if ( ch == (int)d->inChn.size() || ch == (int)d->outChn.size() ) {
            dex.isSG = true;
        } else if ( ch == (int)(d->inChn.size()+d->outChn.size()+1) ) { // Graph SG ?
            dex.isSG = true;
        } else { // Graph out ?
            dex.isAnalog = true;
            dex.isInChn = false;
            dex.chanID = ch - d->inChn.size() - 1;
        }
        dex.isValid = true; // One hopes.
    } else if ( ch == -1 ) { // 2016-RC1 : None
        dex.isNone = true;
        dex.isValid = true;
    } else if ( ch == 1000 ) { // 2016-RC1 : SG
        dex.isSG = true;
        dex.isValid = true;
    } else if ( ch >= 10000 && ch < 10000 + (int)d->inChn.size() ) { // 2016-RC1 : Graph inChn
        dex.isAnalog = true;
        dex.chanID = ch - 10000;
        dex.isInChn = true;
        dex.isValid = true;
    } else if ( ch >= 20000 && ch < 20000 + (int)d->outChn.size() ) { // 2016-RC1 : Graph outChn
        dex.isAnalog = true;
        dex.chanID = ch - 20000;
        dex.isInChn = false;
        dex.isValid = true;
    } else if ( ch >= 1100000000 ) { // 2016-RC1 : HH Proto
        dex.isPrototype = true;
        dex.modelClass = ModelClass::HH;
        dex.modelID = ((ch - 1100000000) / 100000) - 1;
        dex.isValid = true;
    } else if ( ch >= 100000000 ) { // 2016-RC1 : HH Inst
        dex.isVirtual = true;
        dex.modelClass = ModelClass::HH;
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
    } else if ( !parts.at(0).compare("SpikeGen", Qt::CaseInsensitive) ) {
        dex.isSG = true;
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Analog", Qt::CaseInsensitive) ) {
        if ( parts.size() != 4 )
            return is;

        if ( !parts.at(1).compare("SimulDAQ", Qt::CaseInsensitive) )
            dex.daqClass = DAQClass::Simul;
        else if ( !parts.at(1).compare("DigiData1200", Qt::CaseInsensitive) )
            dex.daqClass = DAQClass::DD1200;
        else if ( !parts.at(1).compare("NIDAQ", Qt::CaseInsensitive) )
            dex.daqClass = DAQClass::NI;
        else
            return is;

        if ( parts.at(3).startsWith("ai", Qt::CaseInsensitive) )
            dex.isInChn = true;
        else if ( parts.at(3).startsWith("ao", Qt::CaseInsensitive) )
            dex.isInChn = false;
        else
            return is;

        dex.isAnalog = true;
        dex.devID = parts.at(2).toInt();
        dex.chanID = parts.at(3).mid(2).toInt();
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Prototype", Qt::CaseInsensitive) ) {
        if ( parts.size() != 3 )
            return is;

        if ( !parts.at(1).compare("HH", Qt::CaseInsensitive) )
            dex.modelClass = ModelClass::HH;
        else
            return is;

        dex.isPrototype = true;
        dex.modelID = parts.at(2).toInt();
        dex.isValid = true;
    } else if ( !parts.at(0).compare("Virtual", Qt::CaseInsensitive) ) {
        if ( parts.size() != 4 )
            return is;

        if ( !parts.at(1).compare("HH", Qt::CaseInsensitive) )
            dex.modelClass = ModelClass::HH;
        else
            return is;

        dex.isVirtual = true;
        dex.modelID = parts.at(2).toInt();
        dex.instID = parts.at(3).toInt();
        dex.isValid = true;
    } else if ( parts.size() == 1 && LOADED_PROTOCOL_VERSION == 0 ) {
        legacyRescue_0(str, dex);
    }
    return is;
}

bool operator==(ChannelIndex const& a, ChannelIndex const& b)
{
    return a.isValid==b.isValid &&
            a.isNone==b.isNone &&
            a.isSG==b.isSG &&
            a.isPrototype==b.isPrototype &&
            a.isVirtual==b.isVirtual &&
            (!a.isPrototype || (a.modelClass==b.modelClass && a.modelID==b.modelID)) &&
            (!a.isVirtual || (a.modelClass==b.modelClass && a.modelID==b.modelID && a.instID==b.instID)) &&
            a.isAnalog==b.isAnalog &&
            (!a.isAnalog || (a.daqClass==b.daqClass && a.devID==b.devID && a.isInChn==b.isInChn && a.chanID==b.chanID));
}
