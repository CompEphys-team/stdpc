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

#include "DaqWidget.h"
#include "ui_DaqFactoryWidget.h"
#include "ui_DaqWidget.h"

DaqFactoryWidget::DaqFactoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DaqFactoryWidget),
    combo(ui->combo),
    button(ui->button)
{
    ui->setupUi(this);
}

DaqFactoryWidget::~DaqFactoryWidget()
{
    delete ui;
}


DaqWidget::DaqWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DaqWidget),
    params(ui->params),
    active(ui->active)
{
    ui->setupUi(this);
}

DaqWidget::~DaqWidget()
{
    delete ui;
}

void DaqWidget::setLabel(const QString &newLabel) {
    label = newLabel;
    ui->label->setText(label);
}

void DaqWidget::statusChanged(DeviceStatus s)
{
    if ( s == DeviceStatus::Failed )
        ui->label->setText(QString("<html><head/><body><p style='font-weight:600; color:#ff0000;'>%1</p></body></html>").arg(label));
    else
        ui->label->setText(label);
}
