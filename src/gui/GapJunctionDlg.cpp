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


#include "GapJunctionDlg.h"

GapJunctionDlg::GapJunctionDlg(int no, ChannelListModel *in, ChannelListModel *out, QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);
     
     label = gapJunctDlgLabel->text();
     setIndex(no);

     QVector<AssignmentCellBase<GapJunctionAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<GapJunctionAssignment>(&GapJunctionAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::preInChannel, "Presyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::preOutChannel, "Presyn I", 95, out));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::postInChannel, "Postsyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::postOutChannel, "Postsyn I", 95, out));
     assignments->init(vec);
}

void GapJunctionDlg::exportData(GJunctData &p) 
{
  p.type= typeCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  assignments->exportData(p.assign);
}

void GapJunctionDlg::importData(GJunctData p) 
{
  QString num;
  typeCombo->setCurrentIndex(p.type);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  assignments->importData(p.assign);
}

void GapJunctionDlg::setIndex(int no)
{
    gapJunctDlgLabel->setText(label.arg(no));
}
