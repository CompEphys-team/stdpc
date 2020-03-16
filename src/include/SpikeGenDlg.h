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

#ifndef SPIKEGENDLG_H
#define SPIKEGENDLG_H

#include "ModelDlg.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QStyledItemDelegate>
#include <QStandardItemModel>
#include "WideComboBox.h"
#include "ChannelListModel.h"

namespace Ui {
class SpikeGenDlg;
}

class STDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    STDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    ~STDelegate() {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
                          const QModelIndex &/*index*/) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const override;
};

class SpikeGenDlg : public ModelDlg
{
    Q_OBJECT

public:
    explicit SpikeGenDlg(size_t idx, QWidget *parent = nullptr);
    ~SpikeGenDlg();

    void importData();
    void exportData();
    void setIndex(size_t);

private:
    Ui::SpikeGenDlg *ui;

    ChannelListModel *clm;

    QVector<QCheckBox*> actives, vSaves;
    QVector<QDoubleSpinBox*> vBiases, bdThresholds;
    QVector<WideComboBox*> bdChannels;
    QMetaObject::Connection activec, vSavec, vBiasc, bdChannelc, bdThresholdc;

    QStandardItemModel model;
    STDelegate delegate;

    void addInstRow(int row, QCheckBox *active, QCheckBox *vSave, QDoubleSpinBox *vBias,
                    WideComboBox *bdChannel, QDoubleSpinBox *bdThresh);
    void setCellCheckBox(int row, int column, QCheckBox *box);

    void importST(std::vector<std::vector<double>> &);
    void exportST(std::vector<std::vector<double>> &);

private slots:
    void growInstTable(bool = true);
};

#endif // SPIKEGENDLG_H
