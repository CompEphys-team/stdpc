#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QVector>
#include "SimulDAQ.h"
#include "DigiData.h"
#ifdef NATIONAL_INSTRUMENTS
#include "Nidaq.h"
#endif

enum class DeviceStatus { Active, Inactive, Failed };

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    DeviceManager() {}
    ~DeviceManager();

    QVector<QPair<DeviceStatus, QString>> init();
    void clear();

    template <typename param_type>
    DeviceStatus initSingle(QString &name, int idx, std::vector<param_type> *params);

    /// Remove a device from active use, freeing its space and moving up all subsequent devices of the same type.
    /// In the interest of undoability, the corresponding parameters are moved to the end of the param vector.
    template <typename param_type>
    void remove(int idx, std::vector<param_type> *params);

    DAQ *getDevice(ChannelIndex const&);
    inChannel *getInChan(ChannelIndex const&);
    outChannel *getOutChan(ChannelIndex const&);

    template <typename param_type>
    QVector<DAQ *> &get();

    QVector<DAQ *> actdev;

signals:
    void removedDevice(ChannelIndex dex);

private:
    DeviceStatus _initSingle(DAQ *dev, bool active, QString &name);

    QVector<DAQ*> sdaq;
    QVector<DAQ*> dd1200;
#ifdef NATIONAL_INSTRUMENTS
    QVector<DAQ*> nidaq;
#endif
};

#endif // DEVICEMANAGER_H
