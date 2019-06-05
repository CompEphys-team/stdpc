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


#include "STDPDlg.h"

STDPDlg::STDPDlg(QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);
}

void STDPDlg::setLabel(const QString & lb)
{
    synSTDPLabel->setText(lb);
}

void STDPDlg::exportData(STPlast &d)
{
  d.AMinus= AMinusE->text().toDouble()*1e-9;
  d.tauMinus= tauMinusE->text().toDouble()*1e-3;
  d.APlus= APlusE->text().toDouble()*1e-9;
  d.tauPlus= tauPlusE->text().toDouble()*1e-3;
  d.Exponent= ExponentE->text().toInt();
  d.Shift= ShiftE->text().toDouble()*1e-3;
  d.History= HistoryE->text().toInt();
  d.Table= TableCombo->currentIndex();
  d.tableDt= tableDtE->text().toDouble()*1e-3;
  d.tableDgMin= tableDgMinE->text().toDouble()*1e-9;
  d.gMax= gMaxE->text().toDouble()*1e-9;
  d.gMid= gMidE->text().toDouble()*1e-9;
  d.gSlope= gSlopeE->text().toDouble()*1e-9;
  d.sigmoidTable= sigmoidTableCombo->currentIndex();
  d.sigmoidTableDg= sigmoidTableDgE->text().toDouble()*1e-9;
  d.sigmoidTableMaxEntry= sigmoidTableMaxEntryE->text().toDouble()*1e-9;
}

void STDPDlg::importData(STPlast d)
{
  QString num;
  
  num.setNum(d.AMinus*1e9);
  AMinusE->setText(num);
  num.setNum(d.tauMinus*1e3);
  tauMinusE->setText(num);
  num.setNum(d.APlus*1e9);
  APlusE->setText(num);
  num.setNum(d.tauPlus*1e3);
  tauPlusE->setText(num);
  num.setNum(d.Exponent);
  ExponentE->setText(num);
  num.setNum(d.Shift*1e3);
  ShiftE->setText(num);
  num.setNum(d.History);
  HistoryE->setText(num);
  TableCombo->setCurrentIndex(d.Table);
  num.setNum(d.tableDt*1e3);
  tableDtE->setText(num);
  num.setNum(d.tableDgMin*1e9);
  tableDgMinE->setText(num);
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
