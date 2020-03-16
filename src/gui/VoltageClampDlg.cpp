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

#include <QDoubleSpinBox>
#include "VoltageClampDlg.h"
#include <QMessageBox>
#include "VoltageClamp.h"

VoltageClampDlg::VoltageClampDlg(int no, QWidget *parent)
     : ConductanceDlg(no, parent)
 {
     setupUi(this);

     setIndex(no);

     connect(CloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(CloseClicked(QAbstractButton *)));

     ChannelListModel *in = ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank);
     ChannelListModel *out = ChannelListModel::getModel(ChannelListModel::Out | ChannelListModel::Blank);

     QVector<AssignmentCellBase<SynapseAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<SynapseAssignment>(&SynapseAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PreSynChannel, "Command V", 95, in));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PostSynChannel, "Cell V", 95, in));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::OutSynChannel, "Clamp I", 95, out));
     assignments->init(vec);
}

void VoltageClampDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    QString lb = QString("%1 %2").arg(VoltageClampProxy::get()->prettyName()).arg(no);
    VoltageClampDlgLabel->setText(lb);
}

void VoltageClampDlg::CloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  emit labelChanged(leLabel->text());
}

void VoltageClampDlg::exportData()
{
  VoltageClampData &p = VoltageClampProxy::p[idx];
  p.label = leLabel->text();
  p.gP= gPE->text().toDouble()*1e-6;
  p.gI= gIE->text().toDouble()*1e-6;
  p.gD= gDE->text().toDouble()*1e-6;
  p.decayI= decayE->text().toDouble();
  p.tstepD= tDE->text().toInt();

  p.easeIn = easeIn->text().toDouble();
  p.easeInAmpLimit = easeInAmpLimit->text().toDouble() * 1e-9;

  assignments->exportData(p.assign);
}

void VoltageClampDlg::importData()
{
  VoltageClampData &p = VoltageClampProxy::p[idx];
  leLabel->setText(p.label);
  QString num;   
  num.setNum(p.gP*1e6);
  gPE->setText(num);
  num.setNum(p.gI*1e6);
  gIE->setText(num);
  num.setNum(p.gD*1e6);
  gDE->setText(num);
  num.setNum(p.decayI);
  decayE->setText(num);
  num.setNum(p.tstepD);
  tDE->setText(num);

  num.setNum(p.easeIn);
  easeIn->setText(num);
  num.setNum(p.easeInAmpLimit*1e9);
  easeInAmpLimit->setText(num);

  assignments->importData(p.assign);
}
