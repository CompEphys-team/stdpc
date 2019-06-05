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

#ifndef AlphaBetaHHDLG_H
#define AlphaBetaHHDLG_H


#include "ui_AlphaBetaHHDlg.h"
#include "ObjectDataTypes.h"

class AlphaBetaHHDlg : public QDialog, private Ui::AlphaBetaHHDlg
{
     Q_OBJECT

  public:
     AlphaBetaHHDlg(int, ChannelListModel *in, ChannelListModel *out, QWidget *parent= 0);
     void exportData(abHHData &);
     void importData(abHHData);
     void setIndex(int);
     typedef abHHData param_type;

     QString label;
}; 

#endif
