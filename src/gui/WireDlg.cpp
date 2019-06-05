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

#include <QDoubleSpinBox>
#include "WireDlg.h"
#include <QMessageBox>
#include "Wire.h"

WireDlg::WireDlg(int no, QWidget *parent)
     : ConductanceDlg(no, parent)
 {
     setupUi(this);

     setIndex(no);

     connect(CloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(CloseClicked(QAbstractButton *)));

     ChannelListModel *in = ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank);
     ChannelListModel *out = ChannelListModel::getModel(ChannelListModel::Out | ChannelListModel::Blank);

     QVector<AssignmentCellBase<CurrentAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::VChannel, "Input channel", 200, in));
     vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::IChannel, "Output channel", 200, out));
     assignments->init(vec);
}

void WireDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    QString lb = QString("%1 %2").arg(WireProxy::get()->prettyName()).arg(no);
    WireDlgLabel->setText(lb);
}

void WireDlg::CloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  emit labelChanged(leLabel->text());
}

void WireDlg::exportData()
{
  WireData &p = WireProxy::p[idx];
  p.label = leLabel->text();
  p.factor = leFactor->text().toDouble() * 1e-6;

  assignments->exportData(p.assign);
}

void WireDlg::importData()
{
  WireData &p = WireProxy::p[idx];
  leLabel->setText(p.label);
  QString num;
  num.setNum(p.factor * 1e6);
  leFactor->setText(num);

  assignments->importData(p.assign);
}
