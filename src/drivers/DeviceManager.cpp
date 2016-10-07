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
    for ( DAQ *d : sdaq )
        delete d;
    sdaq.clear();
    for ( DAQ *d : dd1200 )
        delete d;
    dd1200.clear();
#ifdef NATIONAL_INSTRUMENTS
    for ( DAQ *d : nidaq )
        delete d;
    nidaq.clear();
#endif
}


template <>
DeviceStatus DeviceManager::initSingle<SDAQData>(QString &name, int idx)
{
    if ( idx < sdaq.size() ) {
        actdev.removeAll(sdaq[idx]);
        sdaq[idx]->~DAQ();
        sdaq[idx] = new(sdaq[idx]) SimulDAQ(idx);
    } else {
        sdaq.push_back(new SimulDAQ(idx));
    }
    return _initSingle(sdaq[idx], SDAQp[idx].active, name);
}

template <>
void DeviceManager::remove<SDAQData>(int idx)
{
    actdev.removeAll(sdaq[idx]);
    delete sdaq[idx];
    sdaq.remove(idx);
    emit removedDevice(getDex(DAQClass::Simul, idx));
}

template <>
QVector<DAQ *> DeviceManager::get<SDAQData>()
{
    return sdaq;
}


template <>
DeviceStatus DeviceManager::initSingle<DigiDataData>(QString &name, int idx)
{
    if ( idx < dd1200.size() ) {
        actdev.removeAll(dd1200[idx]);
        dd1200[idx]->~DAQ();
        dd1200[idx] = new(dd1200[idx]) DigiData(idx);
    } else {
        dd1200.push_back(new DigiData(idx));
    }
    return _initSingle(dd1200[idx], DigiDatap[idx].active, name);
}

template <>
void DeviceManager::remove<DigiDataData>(int idx)
{
    actdev.removeAll(dd1200[idx]);
    delete dd1200[idx];
    dd1200.remove(idx);
    emit removedDevice(getDex(DAQClass::DD1200, idx));
}

template <>
QVector<DAQ *> DeviceManager::get<DigiDataData>()
{
    return dd1200;
}


#ifdef NATIONAL_INSTRUMENTS
template <>
DeviceStatus DeviceManager::initSingle<NIDAQData>(QString &name, int idx)
{
    if ( idx < nidaq.size() ) {
        actdev.removeAll(nidaq[idx]);
        nidaq[idx]->~DAQ();
        nidaq[idx] = new(nidaq[idx]) NIDAQ(idx);
    } else {
        nidaq.push_back(new NIDAQ(idx));
    }
    return _initSingle(nidaq[idx], NIDAQp[idx].active, name);
}

template <>
void DeviceManager::remove<NIDAQData>(int idx)
{
    actdev.removeAll(nidaq[idx]);
    delete nidaq[idx];
    nidaq.remove(idx);
    emit removedDevice(getDex(DAQClass::NI, idx));
}

template <>
QVector<DAQ *> DeviceManager::get<NIDAQData>()
{
    return nidaq;
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
    QVector<QPair<DeviceStatus, QString>> ret(
                SDAQp.size()
                + DigiDatap.size()
#ifdef NATIONAL_INSTRUMENTS
                + NIDAQp.size()
#endif
                );
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
        default:
            return nullptr;
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
        default:
            return nullptr;
        }
    }
    return nullptr;
}

ChannelIndex DeviceManager::getDex(DAQClass type, int idx)
{
    ChannelIndex dex;
    dex.isValid = true;
    dex.isAnalog = true;
    dex.daqClass = type;
    dex.devID = idx;
    return dex;
}
