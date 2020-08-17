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

#include "sinusoidaltransformdlg.h"
#include "ui_sinusoidaltransformdlg.h"
#include "sinusoidaltransform.h"

SinusoidalTransformDlg::SinusoidalTransformDlg(size_t no, QWidget *parent) :
    ConductanceDlg(no, parent),
    ui(new Ui::SinusoidalTransformDlg)
{
    ui->setupUi(this);
    setIndex(no);

    QVector<AssignmentCellBase<ChannelTransformAssignment>*> vec;
    vec.push_back(new AssignmentCellBool<ChannelTransformAssignment>(&ChannelTransformAssignment::active, "Active", 47));
    vec.push_back(new AssignmentCellChannel<ChannelTransformAssignment>(&ChannelTransformAssignment::target, "Target", 165,
       ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Out | ChannelListModel::Directional | ChannelListModel::Blank)));
    vec.push_back(new AssignmentCellBool<ChannelTransformAssignment>(&ChannelTransformAssignment::save, "Save untransformed values", 30));
    ui->assignments->init(vec);
}

SinusoidalTransformDlg::~SinusoidalTransformDlg()
{
    delete ui;
}

void SinusoidalTransformDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    ui->DlgLabel->setText(QString("%1 %2").arg(SinusoidalTransformProxy::get()->prettyName()).arg(no));
}

void SinusoidalTransformDlg::exportData()
{
    SinusoidalTransformData &p = SinusoidalTransformProxy::p[idx];
    p.label = ui->leLabel->text();
    p.seq = ui->order->value();
    p.slope = ui->slope->text().toDouble();
    p.offset = ui->offset->text().toDouble();

    ui->assignments->exportData(p.assign);
}

void SinusoidalTransformDlg::importData()
{
    SinusoidalTransformData &p = SinusoidalTransformProxy::p[idx];
    ui->leLabel->setText(p.label);
    ui->order->setValue(p.seq);
    ui->slope->setText(QString::number(p.slope));
    ui->offset->setText(QString::number(p.offset));

    ui->assignments->importData(p.assign);
}

void SinusoidalTransformDlg::on_buttonBox_clicked(QAbstractButton *)
{
    hide();
    emit labelChanged(ui->leLabel->text());
}
