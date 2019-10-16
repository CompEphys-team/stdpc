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

#include "HHModelDlg.h"
#include "ui_HHModelDlg.h"
#include "HHNeuron.h"

/// Developer's note: In the interest of channel index stability, do not allow removing/reordering rows

HHModelDlg::HHModelDlg(size_t idx, QWidget *parent) :
    ModelDlg(idx, parent),
    ui(new Ui::HHModelDlg)
{
    ui->setupUi(this);
    setIndex(idx);
    ui->table->setHorizontalHeaderLabels({"Active",
                                          "",
                                          "VChan\nSave",
                                          "VChan\nBias (mV)",
                                          "VChan\nDetect spikes",
                                          "VChan\nThreshold (mV)",
                                          "",
                                          "IChan\nSave",
                                          "IChan\nBias (nA)"});
    ui->table->setColumnWidth(0, 40);
    ui->table->setColumnWidth(1, 10);
    ui->table->setColumnWidth(2, 80);
    ui->table->setColumnWidth(3, 80);
    ui->table->setColumnWidth(4, 80);
    ui->table->setColumnWidth(5, 100);
    ui->table->setColumnWidth(6, 10);
    ui->table->setColumnWidth(7, 80);
    ui->table->setColumnWidth(8, 80);
    growTable(false);

    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addMultiple()));

    connect(ui->Vlimit, &QCheckBox::toggled, [=](bool y){
        ui->Vmax->setEnabled(y);
        ui->Vmin->setEnabled(y);
    });
}

void HHModelDlg::setIndex(size_t no)
{
    ui->titleLabel->setText(QString("%1 %2").arg(HHNeuronProxy::get()->prettyName()).arg(no));
    ModelDlg::setIndex(no);
}

HHModelDlg::~HHModelDlg()
{
    delete ui;
}

void HHModelDlg::importData()
{
    int row = 0;
    QCheckBox *active, *vSave, *spkD, *iSave;
    QDoubleSpinBox *vBias, *spkThr, *iBias;

    actives.clear();
    vSaves.clear();
    vBiases.clear();
    spkDs.clear();
    spkThrs.clear();
    iSaves.clear();
    iBiases.clear();
    ui->table->setRowCount(0);

    for ( vInstData const& inst : HHNeuronProxy::p[idx].inst ) {
        active = new QCheckBox();
        vSave = new QCheckBox();
        vBias = new QDoubleSpinBox();
        spkD = new QCheckBox();
        spkThr = new QDoubleSpinBox();
        iSave = new QCheckBox();
        iBias = new QDoubleSpinBox();
        addRow(row++, active, vSave, vBias, spkD, spkThr, iSave, iBias);
        active->setChecked(inst.active);
        vSave->setChecked(inst.inChn.chnlSaving);
        vBias->setValue(inst.inChn.bias * 1e3);
        spkD->setChecked(inst.inChn.spkDetect);
        spkThr->setValue(inst.inChn.spkDetectThresh * 1e3);
        iSave->setChecked(inst.outChn.chnlSaving);
        iBias->setValue(inst.outChn.bias * 1e9);
    }
    growTable(false);

    ui->leLabel->setText(HHNeuronProxy::p[idx].label);
    ui->C->setValue(HHNeuronProxy::p[idx].C * 1e9);
    ui->gLeak->setValue(HHNeuronProxy::p[idx].gLeak * 1e9);
    ui->ELeak->setValue(HHNeuronProxy::p[idx].ELeak * 1e3);
    ui->Vlimit->setChecked(HHNeuronProxy::p[idx].Vlimit);
    ui->Vmin->setValue(HHNeuronProxy::p[idx].Vmin * 1e3);
    ui->Vmax->setValue(HHNeuronProxy::p[idx].Vmax * 1e3);
}

void HHModelDlg::exportData()
{
    vInstData inst;
    HHNeuronProxy::p[idx].inst.clear();
    HHNeuronProxy::p[idx].inst.reserve(ui->table->rowCount() - 1);
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        inst.active = actives[i]->isChecked();
        inst.inChn.chnlSaving = vSaves[i]->isChecked();
        inst.inChn.bias = vBiases[i]->value() * 1e-3;
        inst.inChn.spkDetect = spkDs[i]->isChecked();
        inst.inChn.spkDetectThresh = spkThrs[i]->value() * 1e-3;
        inst.outChn.chnlSaving = iSaves[i]->isChecked();
        inst.outChn.bias = iBiases[i]->value() * 1e-9;
        HHNeuronProxy::p[idx].inst.push_back(inst);
    }

    HHNeuronProxy::p[idx].label = ui->leLabel->text();
    HHNeuronProxy::p[idx].C = ui->C->value() * 1e-9;
    HHNeuronProxy::p[idx].gLeak = ui->gLeak->value() * 1e-9;
    HHNeuronProxy::p[idx].ELeak = ui->ELeak->value() * 1e-3;
    HHNeuronProxy::p[idx].Vlimit = ui->Vlimit->isChecked();
    HHNeuronProxy::p[idx].Vmin = ui->Vmin->value() * 1e-3;
    HHNeuronProxy::p[idx].Vmax = ui->Vmax->value() * 1e-3;
    emit channelsChanged();
    emit modelStatusChanged();
}

void HHModelDlg::addRow(int row, QCheckBox *active,
                          QCheckBox *vSave, QDoubleSpinBox *vBias, QCheckBox *spkD, QDoubleSpinBox *spkThr,
                          QCheckBox *iSave, QDoubleSpinBox *iBias)
{
    ui->table->insertRow(row);
    ui->table->setRowHeight(row, 25);
    QStringList labels;
    for ( int i = 0; i < ui->table->rowCount(); i++ )
        labels << QString::number(i);
    ui->table->setVerticalHeaderLabels(labels);

    setCellCheckBox(row, 0, active);

    setCellCheckBox(row, 2, vSave);
    ui->table->setCellWidget(row, 3, vBias);
    setCellCheckBox(row, 4, spkD);
    ui->table->setCellWidget(row, 5, spkThr);

    setCellCheckBox(row, 7, iSave);
    ui->table->setCellWidget(row, 8, iBias);

    vBias->setRange(-1000, 1000);
    vBias->setDecimals(3);
    spkThr->setRange(-1000, 1000);
    spkThr->setDecimals(3);
    iBias->setRange(-1e6, 1e6);
    iBias->setDecimals(6);

    actives.insert(row, active);
    vSaves.insert(row, vSave);
    vBiases.insert(row, vBias);
    spkDs.insert(row, spkD);
    spkThrs.insert(row, spkThr);
    iSaves.insert(row, iSave);
    iBiases.insert(row, iBias);
}

void HHModelDlg::setCellCheckBox(int row, int column, QCheckBox *box)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(box);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    widget->setLayout(layout);
    ui->table->setCellWidget(row, column, widget);
}

void HHModelDlg::growTable(bool reactive)
{
    disconnect(activec);
    disconnect(vSavec);
    disconnect(vBiasc);
    disconnect(spkDc);
    disconnect(spkThrc);
    disconnect(iSavec);
    disconnect(iBiasc);

    if ( reactive && !actives.empty() )
        actives.last()->setChecked(true);

    QCheckBox *active = new QCheckBox();
    QCheckBox *vSave = new QCheckBox();
    QDoubleSpinBox *vBias = new QDoubleSpinBox();
    QCheckBox *spkD = new QCheckBox();
    QDoubleSpinBox *spkThr = new QDoubleSpinBox();
    QCheckBox *iSave = new QCheckBox();
    QDoubleSpinBox *iBias = new QDoubleSpinBox();

    addRow(ui->table->rowCount(), active, vSave, vBias, spkD, spkThr, iSave, iBias);

    activec = connect(active, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    vSavec = connect(vSave, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    vBiasc = connect(vBias, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
    spkDc = connect(spkD, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    spkThrc = connect(spkThr, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
    iSavec = connect(iSave, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    iBiasc = connect(iBias, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
}

void HHModelDlg::addMultiple()
{
    QCheckBox *active, *vSave, *spkD, *iSave;
    QDoubleSpinBox *vBias, *spkThr, *iBias;
    int insRow = ui->table->rowCount() - 1;
    int copyRow = insRow ? insRow - 1 : 0;
    for ( int i = 0; i < ui->addNumber->value(); i++ ) {
        active = new QCheckBox();
        vSave = new QCheckBox();
        vBias = new QDoubleSpinBox();
        spkD = new QCheckBox();
        spkThr = new QDoubleSpinBox();
        iSave = new QCheckBox();
        iBias = new QDoubleSpinBox();

        addRow(insRow, active, vSave, vBias, spkD, spkThr, iSave, iBias);

        if ( insRow > 0 ) {
            active->setChecked(actives[copyRow]->isChecked());
            vSave->setChecked(vSaves[copyRow]->isChecked());
            vBias->setValue(vBiases[copyRow]->value());
            spkD->setChecked(spkDs[copyRow]->isChecked());
            spkThr->setValue(spkThrs[copyRow]->value());
            iSave->setChecked(iSaves[copyRow]->isChecked());
            iBias->setValue(iBiases[copyRow]->value());
        } else {
            active->setChecked(true);
        }
    }
}
