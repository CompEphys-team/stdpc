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

#include "DataSavingDlg.h"
#include "ui_DataSavingDlg.h"
#include "Global.h"


DataSavingDlg::DataSavingDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    SaveFileNameDlg= new QFileDialog(this, QString("Save File Dialog"), QString("."),
           QString("*.dat"));
    SaveFileNameDlg->setAcceptMode(QFileDialog::AcceptSave);

    QObject::connect(browseButton, SIGNAL(released()), SaveFileNameDlg, SLOT(show()));
    QObject::connect(SaveFileNameDlg, SIGNAL(accepted()), SLOT(updateSaveFileName()));
    QObject::connect(enabledE, SIGNAL(clicked()), SLOT(toggleEnabled()));
    toggleEnabled();
}


DataSavingDlg::~DataSavingDlg()
{
}

void DataSavingDlg::updateSaveFileName()
{
    QStringList fnlist= SaveFileNameDlg->selectedFiles();
    leFileName->setText(*fnlist.begin());
}

void DataSavingDlg::exportData()
{
    dataSavingPs.enabled= enabledE->isChecked();
    dataSavingPs.fileName = leFileName->text();
    double sFreq = 1000 * leSavingFreq->text().toDouble();   // kHz -> Hz
    if ( sFreq <= 0.0 ) sFreq = 1000; // set to 1kHz by default
    dataSavingPs.savingFreq = sFreq;
    dataSavingPs.isBinary = (cbSavingFormat->currentIndex() > 0);
}

void DataSavingDlg::importData()
{
    QString sFreq;
    enabledE->setChecked(dataSavingPs.enabled);
    leFileName->setText(dataSavingPs.fileName);
    sFreq.setNum(dataSavingPs.savingFreq / 1000);   // Hz -> kHz
    leSavingFreq->setText(sFreq);
    cbSavingFormat->setCurrentIndex(dataSavingPs.isBinary);
    toggleEnabled();
}

void DataSavingDlg::toggleEnabled()
{
    if (enabledE->isChecked()) {
        leFileName->setEnabled(true);
        browseButton->setEnabled(true);
        leSavingFreq->setEnabled(true);
        cbSavingFormat->setEnabled(true);
    }
    else {
        leFileName->setEnabled(false);
        browseButton->setEnabled(false);
        leSavingFreq->setEnabled(false);
        cbSavingFormat->setEnabled(false);
    }
}
