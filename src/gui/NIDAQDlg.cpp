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

#include "NIDAQDlg.h"
#include "Util.h"
#include "Nidaq.h"

NIDAQDlg::NIDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
    setupUi(this);
    connect(inChannels, SIGNAL(clicked(bool)), this, SLOT(openInChnDlg()));
    connect(outChannels, SIGNAL(clicked(bool)), this, SLOT(openOutChnDlg()));
    setIndex(idx);
}

void NIDAQDlg::setIndex(size_t no)
{
    DAQLabel->setText(QString("%1 %2").arg(NIDAQProxy::get()->prettyName()).arg(no));
    DAQDlg::setIndex(no);
}

void NIDAQDlg::exportData(bool forceInit)
{
    bool change= false;
    getEntry<QString>(NIDAQProxy::p[idx].label, leLabel->text(), change);
    getEntry(NIDAQProxy::p[idx].deviceName, DeviceNameE->text(), change);
    DAQDlg::exportData(change || forceInit);
}

void NIDAQDlg::importData()
{
    leLabel->setText(NIDAQProxy::p[idx].label);
    DeviceNameE->setText(NIDAQProxy::p[idx].deviceName);
    DAQDlg::importData();
}

void NIDAQDlg::backup()
{
    bak = NIDAQProxy::p[idx];
}

void NIDAQDlg::restoreBackup()
{
    NIDAQProxy::p[idx] = bak;
}
