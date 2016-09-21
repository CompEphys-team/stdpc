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
        if ( NIDAQp.empty() || (int)NIDAQp.size() <= devID || (int)NIDAQp[devID].inChn.size() <= chanID )
            return nullptr;
        return &(NIDAQp[devID].inChn[chanID]);
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
        if ( NIDAQp.empty() || (int)NIDAQp.size() <= devID || (int)NIDAQp[devID].outChn.size() <= chanID )
            return nullptr;
        return &(NIDAQp[devID].outChn[chanID]);
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
    } else ret = "NYI";
    return ret;
}

QString ChannelIndex::toString(QChar sep) const
{
    QString ret;
    if ( !isValid ) {
        return ret;
    } else if ( isNone ) {
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

std::istream &operator>>(std::istream &is, ChannelIndex &dex)
{
    ChannelIndex blank;
    dex = blank;
    std::string stdstr;
    is >> stdstr;
    QString str = QString::fromStdString(stdstr);
    QStringList parts = str.split("/");
    if ( parts.size() < 3 ) { // Minimum is Prototype/modelClass/modelID
        // TODO: Attempt legacy rescue?
        return is;
    }
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
