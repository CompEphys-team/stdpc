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

#ifndef SYNAPTICNOISEDLG_H
#define SYNAPTICNOISEDLG_H

#include "ConductanceDlg.h"
#include <QAbstractButton>

namespace Ui {
class SynapticNoiseDlg;
}

class SynapticNoiseDlg : public ConductanceDlg
{
    Q_OBJECT

public:
    explicit SynapticNoiseDlg(size_t idx, QWidget *parent = nullptr);
    ~SynapticNoiseDlg();

    void importData();
    void exportData();
    void setIndex(size_t);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SynapticNoiseDlg *ui;
};

#endif // SYNAPTICNOISEDLG_H
