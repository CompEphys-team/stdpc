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

#ifndef DAQDLG_H
#define DAQDLG_H

#include <QDialog>
#include "InputChannelDlg.h"
#include "OutputChannelDlg.h"
#include "DeviceManager.h"

class DAQDlg : public QDialog
{
     Q_OBJECT

public:
     DAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent = nullptr);

     virtual void importData();
     virtual void exportData(bool forceInit = false);

     virtual void setIndex(size_t i);

public slots:
    inline void accept() { exportData(); QDialog::accept(); }
    inline void reject() { restoreBackup(); if ( didInit ) initDAQ(); importData(); QDialog::reject(); }
    inline void open() { backup(); didInit = false; QDialog::open(); }
    
signals:
    void message(QString);
    void deviceStatusChanged(DeviceStatus, const QString&);
    void channelsChanged();
    void CloseToLimit(QString,QString,double,double,double);

protected slots:
    inline void openInChnDlg() { exportData(); inDlg->open(); }
    inline void openOutChnDlg() { exportData(); outDlg->open(); }

protected:
    size_t idx;
    DAQProxy *proxy;
    InputChannelDlg *inDlg;
    OutputChannelDlg *outDlg;
    bool didInit;

    virtual void backup() = 0;
    virtual void restoreBackup() = 0;

    void initDAQ();
}; 

#endif




