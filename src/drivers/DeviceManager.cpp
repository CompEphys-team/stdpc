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

template <typename param_type>
DeviceStatus DeviceManager::initSingle(QString &name, int idx, std::vector<param_type> *params)
{
    QVector<DAQ*> &vec = get<param_type>();
    if ( idx < vec.size() ) {
        actdev.removeAll(vec[idx]);
        vec[idx]->~DAQ();
        vec[idx] = new(vec[idx]) typename param_type::DaqType(idx);
    } else {
        vec.push_back(new typename param_type::DaqType(idx));
    }
    return _initSingle(vec[idx], (*params)[idx].active, name);
}

template <typename param_type>
void DeviceManager::remove(int idx, std::vector<param_type> *params)
{
    QVector<DAQ*> &vec = get<param_type>();
    actdev.removeAll(vec[idx]);
    delete vec[idx];
    vec.remove(idx);
    using std::swap;
    for ( int i = idx+1; i < (int)params->size(); i++ )
        swap((*params)[i-1], (*params)[i]);
    for ( int i = idx; i < vec.size(); i++ )
        vec[idx]->devID = i;
    emit removedDevice(getDex(param_type::DaqClass, idx));
}

template <>
QVector<DAQ *> &DeviceManager::get<SDAQData>() { return sdaq; }
template DeviceStatus DeviceManager::initSingle(QString&,int,std::vector<SDAQData>*);
template void DeviceManager::remove(int,std::vector<SDAQData>*);

template <>
QVector<DAQ *> &DeviceManager::get<DigiDataData>() { return dd1200; }
template DeviceStatus DeviceManager::initSingle(QString&,int,std::vector<DigiDataData>*);
template void DeviceManager::remove(int,std::vector<DigiDataData>*);

#ifdef NATIONAL_INSTRUMENTS
template <>
QVector<DAQ *> &DeviceManager::get<NIDAQData>() { return nidaq; }
template DeviceStatus DeviceManager::initSingle(QString&,int,std::vector<NIDAQData>*);
template void DeviceManager::remove(int,std::vector<NIDAQData>*);
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
        ret[r].first = initSingle(ret[r].second, i, &SDAQp);
        ++r;
    }
    for ( size_t i = 0; i < DigiDatap.size(); i++ ) {
        ret[r].first = initSingle(ret[r].second, i, &DigiDatap);
        ++r;
    }
#ifdef NATIONAL_INSTRUMENTS
    for ( size_t i = 0; i < NIDAQp.size(); i++ ) {
        ret[r].first = initSingle(ret[r].second, i, &NIDAQp);
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
