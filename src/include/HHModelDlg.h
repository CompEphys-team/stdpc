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

#ifndef HHMODELDLG_H
#define HHMODELDLG_H

#include "ModelDlg.h"
#include <QCheckBox>
#include <QDoubleSpinBox>

namespace Ui {
class HHModelDlg;
}

class HHModelDlg : public ModelDlg
{
    Q_OBJECT

public:
    explicit HHModelDlg(size_t idx, QWidget *parent = 0);
    ~HHModelDlg();

    void importData();
    void exportData();
    void setIndex(size_t);

private:
    Ui::HHModelDlg *ui;
    QVector<QCheckBox*> actives, vSaves, spkDs, iSaves;
    QVector<QDoubleSpinBox*> vBiases, spkThrs, iBiases;
    QMetaObject::Connection activec, vSavec, vBiasc, spkDc, spkThrc, iSavec, iBiasc;

    QString label;

    void addRow(int row, QCheckBox *active,
                QCheckBox *vSave, QDoubleSpinBox *vBias, QCheckBox *spkD, QDoubleSpinBox *spkThr,
                QCheckBox *iSave, QDoubleSpinBox *iBias);
    void setCellCheckBox(int row, int column, QCheckBox *box);

private slots:
    void growTable(bool reactive = true);
    void addMultiple();
};

#endif // HHMODELDLG_H
