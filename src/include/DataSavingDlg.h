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

#ifndef DataSavingDlg_H
#define DataSavingDlg_H

#include <QDialog>
#include "ui_DataSavingDlg.h"


namespace Ui {
    class DataSavingDlg;
}

class DataSavingDlg : public QDialog, private Ui::DataSavingDlg
{
    Q_OBJECT

public:
    DataSavingDlg(QWidget *parent = nullptr);
    ~DataSavingDlg();
    void exportData();
    void importData();

private slots:
    void on_browse_clicked();
};

#endif // DataSavingDlg_H
