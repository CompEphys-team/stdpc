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

#ifndef ABSYNDLG_H
#define ABSYNDLG_H

#include "ConductanceDlg.h"
#include <QAbstractButton>
#include "ui_AbSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class abSynDlg : public ConductanceDlg, private Ui::abSynDlg
{
     Q_OBJECT

  public:
     abSynDlg(size_t, QWidget *parent= nullptr);
     void exportData();
     void importData();
     void setIndex(size_t);

     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;
     
  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
}; 

#endif
