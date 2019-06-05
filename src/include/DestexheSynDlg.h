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

#ifndef DESTEXHESYNDLG_H
#define DESTEXHESYNDLG_H


#include <QAbstractButton>
#include "ui_DestexheSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class DestexheSynDlg : public QDialog, private Ui::DestexheSynDlg
{
     Q_OBJECT

  public:
     DestexheSynDlg(int, ChannelListModel *in, ChannelListModel *out, QWidget *parent= 0);
     void exportData(DestexheSynData &);
     void importData(DestexheSynData);
     void setIndex(int);

     QString label;
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;

     typedef DestexheSynData param_type;

  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
};

#endif
