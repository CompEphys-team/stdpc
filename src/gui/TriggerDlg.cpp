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

#include "TriggerDlg.h"
#include "ui_TriggerDlg.h"
#include "DeviceManager.h"

TriggerDlg::TriggerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TriggerDlg)
{
    ui->setupUi(this);
    connect(&Devices, SIGNAL(removedDevice(ChannelIndex)), this, SLOT(updateChannels()));
}

TriggerDlg::~TriggerDlg()
{
    delete ui;
}

void TriggerDlg::exportData()
{
    Triggerp.channel = ui->channel->currentData().value<ChannelIndex>();
}

void TriggerDlg::importData()
{
    for ( int i = 0; i < ui->channel->count(); i++ )
        if ( ui->channel->itemData(i).value<ChannelIndex>() == Triggerp.channel )
            ui->channel->setCurrentIndex(i);
}

void TriggerDlg::updateChannels()
{
    ChannelIndex currentChannel = ui->channel->currentData().value<ChannelIndex>();
    ui->channel->clear();
    for ( auto vec : Devices.all() ) {
        for ( std::shared_ptr<DAQ> daqp : vec ) {
            for ( int i = 0; i < daqp->digInChnNo; i++ ) {
                ChannelIndex tmp(daqp->proxy, daqp->devID, i, true);
                tmp.isDigital = true;
                tmp.isAnalog = false;
                ui->channel->addItem(tmp.prettyName(), QVariant::fromValue(tmp));
            }
        }
    }
    for ( int i = 0; i < ui->channel->count(); i++ )
        if ( ui->channel->itemData(i).value<ChannelIndex>() == currentChannel )
            ui->channel->setCurrentIndex(i);
}
