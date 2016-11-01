#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <memory>
#include "Daq.h"

class DAQProxy {
public:
    virtual DAQData& param(size_t i) = 0;
    virtual size_t size() = 0;
    virtual void resize(size_t) = 0;
    virtual void remove(size_t) = 0;

    virtual QString daqClass() = 0;
    virtual inline QString prettyName() { return daqClass(); }

    virtual DAQ *createDAQ(size_t devID) = 0;
    /* NYI: virtual DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr) = 0; */
};

enum class DeviceStatus { Active, Inactive, Failed };

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    DeviceManager() {}
    ~DeviceManager() {}

    void clear();

    DeviceStatus initSingle(QString &name, DAQProxy *proxy, size_t idx);

    /// Remove a device from active use, freeing its space and moving up all subsequent devices of the same type.
    void remove(DAQProxy *proxy, size_t idx);

    DAQ *getDevice(ChannelIndex const&);
    inChannel *getInChan(ChannelIndex const&);
    outChannel *getOutChan(ChannelIndex const&);

    inline QVector<std::shared_ptr<DAQ>> &active() { return activeDAQ; }
    inline QHash<QString, QVector<std::shared_ptr<DAQ>>> const& all() const { return allDAQ; }

    static inline QHash<QString, DAQProxy*> &Register() { static QHash<QString, DAQProxy*> r; return r; }
    static inline void RegisterDAQ(QString daqClass, DAQProxy *proxy) { Register()[daqClass] = proxy; }

signals:
    void removedDevice(ChannelIndex dex);

private:
    QVector<std::shared_ptr<DAQ>> activeDAQ;
    QHash<QString, QVector<std::shared_ptr<DAQ>>> allDAQ;
};

#endif // DEVICEMANAGER_H
