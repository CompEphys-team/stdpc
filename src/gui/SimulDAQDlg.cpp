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

#include "SimulDAQDlg.h"
#include "Util.h"
#include "SimulDAQ.h"
#include <QFileDialog>

SimulDAQDlg::SimulDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
  setupUi(this);
  connect(inChannels, SIGNAL(clicked(bool)), this, SLOT(openInChnDlg()));
  connect(outChannels, SIGNAL(clicked(bool)), this, SLOT(openOutChnDlg()));
  setIndex(idx);
  change= false;
}

void SimulDAQDlg::setIndex(size_t no)
{
    DAQLabel->setText(QString("%1 %2").arg(SimulDAQProxy::get()->prettyName()).arg(no));
    DAQDlg::setIndex(no);
}

void SimulDAQDlg::exportData(bool forceInit)
{
  uint inChnNo = SimulDAQProxy::p[idx].inChn.size(), outChnNo = SimulDAQProxy::p[idx].outChn.size();
  getEntry(inChnNo, inChannelE->text().toUInt(), change);
  getEntry(outChnNo, outChannelE->text().toUInt(), change);
  if ( change ) {
      SimulDAQProxy::p[idx].inChn.resize(inChnNo);
      SimulDAQProxy::p[idx].outChn.resize(outChnNo);
      for ( inChnData &i : SimulDAQProxy::p[idx].inChn )
          i.active = true;
      for ( outChnData &i : SimulDAQProxy::p[idx].outChn )
          i.active = true;
  }
  getEntry<QString>(SimulDAQProxy::p[idx].label, leLabel->text(), change);
  getEntry<QString>(SimulDAQProxy::p[idx].inFileName, InputFileE->text(), change);
  getEntry<QString>(SimulDAQProxy::p[idx].outFileName, OutputFileE->text(), change);
  getEntry(SimulDAQProxy::p[idx].inTFac, inTFacE->text().toDouble(), change);
  getEntry(SimulDAQProxy::p[idx].outDt, outDtE->text().toDouble()*1e-3, change);
  getEntry(SimulDAQProxy::p[idx].rewindAfterSettling, cbRewind->isChecked(), change);

  DAQDlg::exportData(change || forceInit);
  change= false;
}

void SimulDAQDlg::importData()
{
  QString num;
  leLabel->setText(SimulDAQProxy::p[idx].label);
  InputFileE->setText(SimulDAQProxy::p[idx].inFileName);
  OutputFileE->setText(SimulDAQProxy::p[idx].outFileName);
  inChannelE->setText(QString::number(SimulDAQProxy::p[idx].inChn.size()));
  outChannelE->setText(QString::number(SimulDAQProxy::p[idx].outChn.size()));
  num.setNum(SimulDAQProxy::p[idx].inTFac);
  inTFacE->setText(num);
  num.setNum(SimulDAQProxy::p[idx].outDt*1e3);
  outDtE->setText(num);
  cbRewind->setChecked(SimulDAQProxy::p[idx].rewindAfterSettling);

  DAQDlg::importData();
}

void SimulDAQDlg::backup()
{
    bak = SimulDAQProxy::p[idx];
}

void SimulDAQDlg::restoreBackup()
{
  SimulDAQProxy::p[idx] = bak;
}

void SimulDAQDlg::on_InputFileB_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select input file...", InputFileE->text());
    if ( !file.isEmpty() ) {
        InputFileE->setText(file);
        change= true;
    }
}

void SimulDAQDlg::on_OutputFileB_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Select input file...", OutputFileE->text());
    if ( !file.isEmpty() ) {
        OutputFileE->setText(file);
        change= true;
    }
}
