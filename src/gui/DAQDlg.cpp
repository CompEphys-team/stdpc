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

#include "DAQDlg.h"

DAQDlg::DAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    QDialog(parent),
    idx(idx),
    proxy(proxy),
    inDlg(new InputChannelDlg(idx, proxy, this)),
    outDlg(new OutputChannelDlg(idx, proxy, this))
{
    inDlg->init();
    outDlg->init();
}

void DAQDlg::importData()
{
    inDlg->importData();
    outDlg->importData();
}

void DAQDlg::exportData(bool forceInit)
{
    inDlg->exportData();
    outDlg->exportData();
    if ( forceInit ) {
        initDAQ();
        inDlg->importData();
        outDlg->importData();
    }
}

void DAQDlg::setIndex(size_t i) {
    inDlg->setIndex(i);
    outDlg->setIndex(i);
    idx = i;
}

void DAQDlg::initDAQ()
{
    QString name;
    DeviceStatus status = Devices.initSingle(name, proxy, idx);
    emit deviceStatusChanged(status, name);
    inDlg->init();
    outDlg->init();
    didInit = true;
}
