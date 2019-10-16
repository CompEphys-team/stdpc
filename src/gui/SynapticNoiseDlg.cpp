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

#include "SynapticNoiseDlg.h"
#include "ui_SynapticNoiseDlg.h"
#include "SynapticNoise.h"

SynapticNoiseDlg::SynapticNoiseDlg(size_t idx, QWidget *parent) :
    ConductanceDlg(idx, parent),
    ui(new Ui::SynapticNoiseDlg)
{
    ui->setupUi(this);
    setIndex(idx);
    
    QVector<AssignmentCellBase<CurrentAssignment>*> vec;
    vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::active, "Active", 47));
    vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::VChannel, "V in", 165,
       ChannelListModel::getModel(ChannelListModel::AnalogIn | ChannelListModel::Prototype | ChannelListModel::Virtual | ChannelListModel::Blank)));
    vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::IChannel, "I out", 165,
       ChannelListModel::getModel(ChannelListModel::AnalogOut | ChannelListModel::Prototype | ChannelListModel::Virtual | ChannelListModel::Blank)));
    vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::save, "Save", 30));
    ui->assignments->init(vec);
}

SynapticNoiseDlg::~SynapticNoiseDlg()
{
    delete ui;
}

void SynapticNoiseDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    ui->DlgLabel->setText(QString("%1 %2").arg(SynapticNoiseProxy::get()->prettyName()).arg(no));
}

void SynapticNoiseDlg::importData()
{
    SynapticNoiseData &p = SynapticNoiseProxy::p[idx];
    ui->leLabel->setText(p.label);
    ui->assignments->importData(p.assign);
    ui->Vrev->setValue(p.Vrev * 1e3);
    ui->tau->setValue(p.tau * 1e3);
    ui->g0->setValue(p.g0 * 1e9);
    ui->stddev->setValue(p.std * 1e9);
    ui->ignoreNegative->setChecked(p.ignoreNegative);
}

void SynapticNoiseDlg::exportData()
{
    SynapticNoiseData &p = SynapticNoiseProxy::p[idx];
    p.label = ui->leLabel->text();
    ui->assignments->exportData(p.assign);
    p.Vrev = ui->Vrev->value() * 1e-3;
    p.tau = ui->tau->value() * 1e-3;
    p.g0 = ui->g0->value() * 1e-9;
    p.std = ui->stddev->value() * 1e-9;
    p.ignoreNegative = ui->ignoreNegative->isChecked();
}

void SynapticNoiseDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( button->text() == "Close" )
        hide();
    emit labelChanged(ui->leLabel->text());
}
