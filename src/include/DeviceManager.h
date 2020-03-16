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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QHash>
#include <memory>
#include "Daq.h"
#include "AP.h"

class DAQDlg;

class DAQProxy {
public:
    /// Parameter access and manipulation functions:
    /// Return a reference to the indicated parameter set
    virtual DAQData& param(size_t i) = 0;
    /// Return the number of parameter sets present
    virtual size_t size() = 0;
    /// Change the number of parameter sets
    virtual void resize(size_t) = 0;
    /// Remove a parameter set. Subsequent sets are expected to move up into the freed space.
    virtual void remove(size_t) = 0;

    /// Return an identifier for this device class (e.g. "NIDAQ"). Must not contain any whitespace or "/" characters.
    virtual QString daqClass() = 0;

    /// Return a human-readable identifier for this device class (e.g. "National Instruments") for use in the GUI
    virtual inline QString prettyName() { return daqClass(); }

    /// Create a new device object
    virtual DAQ *createDAQ(size_t devID) = 0;

    /// Create a new device dialog
    virtual DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr) = 0;

    /// Return a collection of core AP-registered params that should be saved to StdpC.conf on exit
    virtual const std::vector<AP*> &coreAPs() const = 0;

    virtual ~DAQProxy() = default;
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

    static inline QMap<QString, DAQProxy*> &Register() { static QMap<QString, DAQProxy*> r; return r; }
    static inline void RegisterDAQ(QString daqClass, DAQProxy *proxy) { Register()[daqClass] = proxy; }

signals:
    void removedDevice(ChannelIndex dex);

private:
    QVector<std::shared_ptr<DAQ>> activeDAQ;
    QHash<QString, QVector<std::shared_ptr<DAQ>>> allDAQ;
};

#endif // DEVICEMANAGER_H
