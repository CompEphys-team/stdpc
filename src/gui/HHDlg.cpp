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

#include <QString>
#include "HHDlg.h"
#include <QMessageBox>
#include "HH.h"

HHDlg::HHDlg(size_t no, QWidget *parent)
     : ConductanceDlg(no, parent)
 {
     setupUi(this);
     setIndex(no);

     QVector<AssignmentCellBase<CurrentAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::VChannel, "V in", 165,
        ChannelListModel::getModel(ChannelListModel::AnalogIn | ChannelListModel::Prototype | ChannelListModel::Blank)));
     vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::IChannel, "I out", 165,
        ChannelListModel::getModel(ChannelListModel::AnalogOut | ChannelListModel::Prototype | ChannelListModel::Blank)));
     vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::save, "Save", 30));
     assignments->init(vec);
}

void HHDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    HHDlgLabel->setText(QString("%1 %2").arg(HHProxy::get()->prettyName()).arg(no));
}

void HHDlg::exportData()
{
  mhHHData &p = HHProxy::p[idx];
  p.label = leLabel->text();
  p.LUTables= (LUCombo->currentIndex() == 1);
  p.gMax= gMaxE->text().toDouble()*1e-9;
  p.Vrev= VrevE->text().toDouble()*1e-3;
  p.mExpo= mExpoE->text().toDouble();
  p.hExpo= hExpoE->text().toDouble();
  // mh formalism
  p.Vm= VmE->text().toDouble()*1e-3;
  p.sm= smE->text().toDouble()*1e-3;
  p.Cm= CmE->text().toDouble();
  p.taumType= taumTypeCombo->currentIndex();
  p.taum= taumE->text().toDouble()*1e-3;
  p.taumAmpl= taumAmplE->text().toDouble()*1e-3;
  p.Vtaum= VtaumE->text().toDouble()*1e-3;
  p.staum= staumE->text().toDouble()*1e-3;
  p.Vh= VhE->text().toDouble()*1e-3;
  p.sh= shE->text().toDouble()*1e-3;
  p.Ch= ChE->text().toDouble();
  p.tauhType= tauhTypeCombo->currentIndex();
  p.tauh= tauhE->text().toDouble()*1e-3;
  p.tauhAmpl= tauhAmplE->text().toDouble()*1e-3;
  p.Vtauh= VtauhE->text().toDouble()*1e-3;
  p.stauh= stauhE->text().toDouble()*1e-3;

  assignments->exportData(p.assign);
}

void HHDlg::importData()
{
  mhHHData &p = HHProxy::p[idx];
  QString num;
  leLabel->setText(p.label);
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);
  num.setNum(p.gMax*1e9);
  gMaxE->setText(num);
  num.setNum(p.Vrev*1e3);
  VrevE->setText(num);
  num.setNum(p.mExpo, 'f', 2);
  mExpoE->setText(num);
  num.setNum(p.hExpo, 'f', 2);
  hExpoE->setText(num);
  // mh formalism
  num.setNum(p.Vm*1e3);
  VmE->setText(num);
  num.setNum(p.sm*1e3);
  smE->setText(num);
  num.setNum(p.Cm);
  CmE->setText(num);
  taumTypeCombo->setCurrentIndex(p.taumType);
  num.setNum(p.taum*1e3);
  taumE->setText(num);
  num.setNum(p.taumAmpl*1e3);
  taumAmplE->setText(num);
  num.setNum(p.Vtaum*1e3);
  VtaumE->setText(num);
  num.setNum(p.staum*1e3);
  staumE->setText(num);
  num.setNum(p.Vh*1e3);
  VhE->setText(num);
  num.setNum(p.sh*1e3);
  shE->setText(num);
  num.setNum(p.Ch);
  ChE->setText(num);
  tauhTypeCombo->setCurrentIndex(p.tauhType);
  num.setNum(p.tauh*1e3);
  tauhE->setText(num);
  num.setNum(p.tauhAmpl*1e3);
  tauhAmplE->setText(num);
  num.setNum(p.Vtauh*1e3);
  VtauhE->setText(num);
  num.setNum(p.stauh*1e3);
  stauhE->setText(num);

  assignments->importData(p.assign);
}

void HHDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( button->text() == "Close" )
        hide();
    emit labelChanged(leLabel->text());
}
