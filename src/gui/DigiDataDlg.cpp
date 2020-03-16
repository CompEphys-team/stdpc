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

#include "DigiDataDlg.h"
#include "Util.h"
#include "DigiData.h"

DigiDataDlg::DigiDataDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
    setupUi(this);
    connect(inChannels, SIGNAL(clicked(bool)), this, SLOT(openInChnDlg()));
    connect(outChannels, SIGNAL(clicked(bool)), this, SLOT(openOutChnDlg()));
    setIndex(idx);
}

void DigiDataDlg::setIndex(size_t no)
{
    DAQLabel->setText(QString("%1 %2").arg(DigiDataProxy::get()->prettyName()).arg(no));
    DAQDlg::setIndex(no);
}

void DigiDataDlg::exportData(bool forceInit)
{
    bool change= false;
    bool success;
    short int tmp= (short int) BaseAddressE->text().toInt(&success,16);
    if (success) {
        getEntry(DigiDataProxy::p[idx].baseAddress, tmp, change);
    }
    getEntry<QString>(DigiDataProxy::p[idx].label, leLabel->text(), change);
    DAQDlg::exportData(forceInit || change);
}

void DigiDataDlg::importData()
{
    QString num;
    num.setNum(DigiDataProxy::p[idx].baseAddress,16);
    BaseAddressE->setText(num);
    leLabel->setText(DigiDataProxy::p[idx].label);
    DAQDlg::importData();
}

void DigiDataDlg::backup()
{
    bak = DigiDataProxy::p[idx];
}

void DigiDataDlg::restoreBackup()
{
    DigiDataProxy::p[idx] = bak;
}
