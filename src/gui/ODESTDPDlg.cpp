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


#include "ODESTDPDlg.h"

ODESTDPDlg::ODESTDPDlg(QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);
}

void ODESTDPDlg::setLabel(const QString &lb)
{
    synSTDPLabel->setText(lb);
}

void ODESTDPDlg::exportData(ODEPlast &d)
{
  d.InitialP= InitialPE->text().toDouble()*1e-9;
  d.InitialD= InitialDE->text().toDouble()*1e-9;
  d.betaP= betaPE->text().toDouble();
  d.betaD= betaDE->text().toDouble();
  d.gamma= gammaE->text().toDouble();
  d.eta= etaE->text().toInt();
  d.highP= highPE->text().toDouble()*1e-9;
  d.lowP= lowPE->text().toDouble()*1e-9;
  d.highD= highDE->text().toDouble()*1e-9;
  d.lowD= lowDE->text().toDouble()*1e-9;
  d.gMax= gMaxE->text().toDouble()*1e-9;
  d.gMid= gMidE->text().toDouble()*1e-9;
  d.gSlope= gSlopeE->text().toDouble()*1e-9;
  d.sigmoidTable= sigmoidTableCombo->currentIndex();
  d.sigmoidTableDg= sigmoidTableDgE->text().toDouble()*1e-9;
  d.sigmoidTableMaxEntry= sigmoidTableMaxEntryE->text().toDouble()*1e-9;
}

void ODESTDPDlg::importData(ODEPlast d)
{
  QString num;
  
  num.setNum(d.InitialP*1e9);
  InitialPE->setText(num);
  num.setNum(d.InitialD*1e9);
  InitialDE->setText(num);
  num.setNum(d.betaP);
  betaPE->setText(num);
  num.setNum(d.betaD);
  betaDE->setText(num);
  num.setNum(d.gamma);
  gammaE->setText(num);
  num.setNum(d.eta);
  etaE->setText(num);
  num.setNum(d.highP*1e9);
  highPE->setText(num);
  num.setNum(d.lowP*1e9);
  lowPE->setText(num);
  num.setNum(d.highD*1e9);
  highDE->setText(num);
  num.setNum(d.lowD*1e9);
  lowDE->setText(num);
  num.setNum(d.gMax*1e9);
  gMaxE->setText(num);
  num.setNum(d.gMid*1e9);
  gMidE->setText(num);
  num.setNum(d.gSlope*1e9);
  gSlopeE->setText(num);
  sigmoidTableCombo->setCurrentIndex(d.sigmoidTable);
  num.setNum(d.sigmoidTableDg*1e9);
  sigmoidTableDgE->setText(num);
  num.setNum(d.sigmoidTableMaxEntry*1e9);
  sigmoidTableMaxEntryE->setText(num);
}
