#include "DeviceManager.h"
#include "Clock.h"
#include "Global.h"

DeviceManager::~DeviceManager()
{
    clear();
}

void DeviceManager::clear()
{
    actdev.clear();
    for ( SimulDAQ *d : sdaq )
        delete d;
    sdaq.clear();
    for ( DigiData *d : dd1200 )
        delete d;
    dd1200.clear();
#ifdef NATIONAL_INSTRUMENTS
    for ( NIDAQ *d : nidaq )
        delete d;
    nidaq.clear();
#endif
}

template <>
DeviceStatus DeviceManager::initSingle<SDAQData>(QString &name, int idx)
{
    if ( idx < sdaq.size() ) {
        actdev.removeAll(sdaq[idx]);
        sdaq[idx]->~SimulDAQ();
        sdaq[idx] = new(sdaq[idx]) SimulDAQ(&SDAQp[idx], idx);
    } else {
        sdaq.push_back(new SimulDAQ(&SDAQp[idx], idx));
    }
    return _initSingle(sdaq[idx], SDAQp[idx].active, name);
}

template <>
DeviceStatus DeviceManager::initSingle<DigiDataData>(QString &name, int idx)
{
    if ( idx < dd1200.size() ) {
        actdev.removeAll(dd1200[idx]);
        dd1200[idx]->~DigiData();
        dd1200[idx] = new(dd1200[idx]) DigiData(&DigiDatap[idx], idx);
    } else {
        dd1200.push_back(new DigiData(&DigiDatap[idx], idx));
    }
    return _initSingle(dd1200[idx], DigiDatap[idx].active, name);
}

#ifdef NATIONAL_INSTRUMENTS
template <>
DeviceStatus DeviceManager::initSingle<NIDAQData>(QString &name, int idx)
{
    if ( idx < nidaq.size() ) {
        actdev.removeAll(nidaq[idx]);
        nidaq[idx]->~NIDAQ();
        nidaq[idx] = new(nidaq[idx]) NIDAQ(&NIDAQp[idx], idx);
    } else {
        nidaq.push_back(new NIDAQ(&NIDAQp[idx], idx));
    }
    return _initSingle(nidaq[idx], NIDAQp[idx].active, name);
}
#endif

DeviceStatus DeviceManager::_initSingle(DAQ *dev, bool active, QString &name)
{
    if ( active ) {
        if ( dev->initialize_board(name) ) {
            actdev.push_back(dev);
            return DeviceStatus::Active;
        } else {
            return DeviceStatus::Failed;
        }
    } else {
        return DeviceStatus::Inactive;
    }
}

QVector<QPair<DeviceStatus, QString>> DeviceManager::init()
{
    QVector<QPair<DeviceStatus, QString>> ret(SDAQp.size() + DigiDatap.size() + NIDAQp.size());
    int r = 0;
    clear();
    for ( size_t i = 0; i < SDAQp.size(); i++ ) {
        ret[r].first = initSingle<SDAQData>(ret[r].second, i);
        ++r;
    }
    for ( size_t i = 0; i < DigiDatap.size(); i++ ) {
        ret[r].first = initSingle<DigiDataData>(ret[r].second, i);
        ++r;
    }
#ifdef NATIONAL_INSTRUMENTS
    for ( size_t i = 0; i < NIDAQp.size(); i++ ) {
        ret[r].first = initSingle<NIDAQData>(ret[r].second, i);
        ++r;
    }
#endif
    return ret;
}

DAQ *DeviceManager::getDevice(const ChannelIndex &dex)
{
    if ( !dex.isValid || !dex.isAnalog )
        return nullptr;
    switch ( dex.daqClass ) {
    case DAQClass::Simul :
        if ( sdaq.empty() || (int)sdaq.size() <= dex.devID )
            return nullptr;
        return sdaq[dex.devID];
    case DAQClass::DD1200 :
        if ( dd1200.empty() || (int)dd1200.size() <= dex.devID )
            return nullptr;
        return dd1200[dex.devID];
    case DAQClass::NI :
#ifdef NATIONAL_INSTRUMENTS
        if ( nidaq.empty() || (int)nidaq.size() <= dex.devID )
            return nullptr;
        return nidaq[dex.devID];
#endif
    default:
        return nullptr;
    }
}

inChannel *DeviceManager::getInChan(const ChannelIndex &dex)
{
    if ( dex.isAnalog && dex.isInChn ) {
        switch ( dex.daqClass ) {
        case DAQClass::Simul :
            if ( sdaq.empty() || (int)sdaq.size() <= dex.devID || sdaq[dex.devID]->in.size() <= dex.chanID )
                return nullptr;
            return &(sdaq[dex.devID]->in[dex.chanID]);
        case DAQClass::DD1200 :
            if ( dd1200.empty() || (int)dd1200.size() <= dex.devID || dd1200[dex.devID]->in.size() <= dex.chanID )
                return nullptr;
            return &(dd1200[dex.devID]->in[dex.chanID]);
        case DAQClass::NI :
#ifdef NATIONAL_INSTRUMENTS
            if ( nidaq.empty() || (int)nidaq.size() <= dex.devID || nidaq[dex.devID]->in.size() <= dex.chanID )
                return nullptr;
            return &(nidaq[dex.devID]->in[dex.chanID]);
#endif
        }
    }
    return nullptr;
}

outChannel *DeviceManager::getOutChan(const ChannelIndex &dex)
{
    if ( dex.isAnalog && !dex.isInChn ) {
        switch ( dex.daqClass ) {
        case DAQClass::Simul :
            if ( sdaq.empty() || (int)sdaq.size() <= dex.devID || sdaq[dex.devID]->out.size() <= dex.chanID )
                return nullptr;
            return &(sdaq[dex.devID]->out[dex.chanID]);
        case DAQClass::DD1200 :
            if ( dd1200.empty() || (int)dd1200.size() <= dex.devID || dd1200[dex.devID]->out.size() <= dex.chanID )
                return nullptr;
            return &(dd1200[dex.devID]->out[dex.chanID]);
        case DAQClass::NI :
#ifdef NATIONAL_INSTRUMENTS
            if ( nidaq.empty() || (int)nidaq.size() <= dex.devID || nidaq[dex.devID]->out.size() <= dex.chanID )
                return nullptr;
            return &(nidaq[dex.devID]->out[dex.chanID]);
#endif
        }
    }
    return nullptr;
}
