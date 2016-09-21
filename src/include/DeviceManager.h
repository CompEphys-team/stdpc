#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QVector>
#include "SimulDAQ.h"
#include "DigiData.h"
#ifdef NATIONAL_INSTRUMENTS
#include "Nidaq.h"
#endif

enum class DeviceStatus { Active, Inactive, Failed };

class DeviceManager
{
public:
    DeviceManager() {}
    ~DeviceManager();

    QVector<QPair<DeviceStatus, QString>> init();

    template <typename param_type>
    DeviceStatus initSingle(QString &name, int idx);

    DAQ *getDevice(ChannelIndex const&);
    inChannel *getInChan(ChannelIndex const&);
    outChannel *getOutChan(ChannelIndex const&);

    QVector<DAQ *> actdev;

private:
    DeviceStatus _initSingle(DAQ *dev, bool active, QString &name);
    void clear();

    QVector<SimulDAQ*> sdaq;
    QVector<DigiData*> dd1200;
#ifdef NATIONAL_INSTRUMENTS
    QVector<NIDAQ*> nidaq;
#endif
};

#endif // DEVICEMANAGER_H
