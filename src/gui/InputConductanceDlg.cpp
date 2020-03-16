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

#include "InputConductanceDlg.h"
#include "ui_InputConductanceDlg.h"
#include "InputConductance.h"

InputConductanceDlg::InputConductanceDlg(size_t idx, QWidget *parent) :
    ConductanceDlg(idx, parent),
    ui(new Ui::InputConductanceDlg)
{
    ui->setupUi(this);
    setIndex(idx);
    
    QVector<AssignmentCellBase<SynapseAssignment>*> vec;
    vec.push_back(new AssignmentCellBool<SynapseAssignment>(&SynapseAssignment::active, "Active", 47));
    vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PreSynChannel, "G in", 165,
       ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank)));
    vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PostSynChannel, "V in", 165,
       ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank)));
    vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::OutSynChannel, "I out", 165,
       ChannelListModel::getModel(ChannelListModel::Out | ChannelListModel::Blank)));
    ui->assignments->init(vec);
}

InputConductanceDlg::~InputConductanceDlg()
{
    delete ui;
}

void InputConductanceDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    ui->DlgLabel->setText(QString("%1 %2").arg(InputConductanceProxy::get()->prettyName()).arg(no));
}

void InputConductanceDlg::importData()
{
    InputConductanceData &p = InputConductanceProxy::p[idx];
    ui->leLabel->setText(p.label);
    ui->assignments->importData(p.assign);
    ui->Vrev->setValue(p.Vrev * 1e3);
    ui->gStatic->setValue(p.gStatic * 1e9);
    ui->rectify->setChecked(p.rectify);
}

void InputConductanceDlg::exportData()
{
    InputConductanceData &p = InputConductanceProxy::p[idx];
    p.label = ui->leLabel->text();
    ui->assignments->exportData(p.assign);
    p.Vrev = ui->Vrev->value() * 1e-3;
    p.gStatic = ui->gStatic->value() * 1e-9;
    p.rectify = ui->rectify->isChecked();
}

void InputConductanceDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( button->text() == "Close" )
        hide();
    emit labelChanged(ui->leLabel->text());
}
