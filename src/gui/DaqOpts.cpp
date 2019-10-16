/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
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

#include "DaqOpts.h"
#include "MainWin.h"
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <functional>

DaqOpts::DaqOpts(QWidget *parent, DAQProxy *proxy, size_t idx) :
    DaqOptsBase(parent, idx),
    proxy(proxy)
{
    if ( idx >= proxy->size() ) {
        proxy->resize(idx+1);
        proxy->param(idx).active = false;
    }

    QString name;
    QFuture<DeviceStatus> future;
    QMessageBox loadMsg(QMessageBox::Information, "Loading...",
                        "Initializing hardware ... this may take a while ...");
    loadMsg.setStandardButtons(QMessageBox::NoButton);
    QFutureWatcher<DeviceStatus> watcher;
    connect(&watcher, SIGNAL(finished()), &loadMsg, SLOT(reject()));
    future = QtConcurrent::run(
                &Devices, &DeviceManager::initSingle, std::ref(name), proxy, idx);
    watcher.setFuture(future);
    loadMsg.exec();
    future.waitForFinished();

    dlg = proxy->createDialog(idx, parent);

    regenerateWidget();
    setIndex(idx);

    connect(dlg, SIGNAL(accepted()), this, SLOT(setLabel()));
    connect(dlg, SIGNAL(channelsChanged()), parent, SIGNAL(channelsChanged()));
    connect(dlg, SIGNAL(message(QString)), parent, SLOT(DisplayMessage(QString)));
    connect(dlg, SIGNAL(deviceStatusChanged(DeviceStatus, const QString&)), parent, SLOT(updateDeviceStatus(DeviceStatus, const QString&)));
    connect(dlg, SIGNAL(CloseToLimit(QString,QString,double,double,double)),
            parent, SLOT(CloseToLimitWarning(QString, QString, double, double, double)));

    static_cast<MyMainWindow*>(parent)->updateDeviceStatus(future.result(), name);
    _widget->statusChanged(future.result());
}

DaqOpts::~DaqOpts()
{
    delete dlg;
}

void DaqOpts::removeDevice()
{
    Devices.remove(proxy, idx);
}

void DaqOpts::importData()
{
    dlg->importData();
    _widget->active->setChecked(proxy->param(idx).active);
}

void DaqOpts::exportData()
{
    if ( idx >= proxy->size() )
        proxy->resize(idx+1);
    proxy->param(idx).active = _widget->active->isChecked();
    dlg->exportData();
}

void DaqOpts::setIndex(size_t i)
{
    idx = i;
    setLabel();
    dlg->setIndex(idx);
}

void DaqOpts::setLabel()
{
    QString label = proxy->param(idx).label;
    if ( label.isEmpty() )
        label = QString("%1 %2").arg(proxy->prettyName()).arg(idx);
    _widget->setLabel(label);
}

void DaqOpts::regenerateWidget()
{
    _widget = new DaqWidget();
    connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(open()));
    connect(_widget->active, SIGNAL(clicked(bool)), this, SLOT(activeChanged()));
    connect(dlg, SIGNAL(deviceStatusChanged(DeviceStatus, const QString&)),
            _widget, SLOT(statusChanged(DeviceStatus)));
}

void DaqOpts::activeChanged()
{
    proxy->param(idx).active = _widget->active->isChecked();
    dlg->exportData(true);
}



DaqOptsBase *DaqOptsPrototype::create(size_t idx, QWidget *parent)
{
    DaqOptsBase *c = new DaqOpts(parent, proxy, idx);
    inst.push_back(c);
    return c;
}

void DaqOptsPrototype::createAll(QWidget *parent)
{
    for ( DaqOptsBase *c : inst )
        delete c;
    inst.clear();
    inst.reserve(proxy->size());
    for ( size_t i = 0; i < proxy->size(); i++ ) {
        create(i, parent)->importData();
    }
}

void DaqOptsPrototype::exportData(bool ignoreDAQ)
{
    if ( !ignoreDAQ ) {
        proxy->resize(inst.size());
        for ( DaqOptsBase *c : inst )
            c->exportData();
    }
}
