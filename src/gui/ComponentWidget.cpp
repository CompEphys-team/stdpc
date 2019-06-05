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

#include "ComponentWidget.h"
#include "ui_ComponentFactoryWidget.h"
#include "ui_ComponentWidget.h"

ComponentFactoryWidget::ComponentFactoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentFactoryWidget),
    combo(ui->combo),
    button(ui->button)
{
    ui->setupUi(this);
}

ComponentFactoryWidget::~ComponentFactoryWidget()
{
    delete ui;
}


ComponentWidget::ComponentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentWidget),
    label(ui->label),
    params(ui->params),
    active(ui->active),
    activeSettling(ui->activeSettling)
{
    ui->setupUi(this);
    connect(active, &QCheckBox::stateChanged, [=](int){
        activeSettling->setEnabled(active->isChecked());
    });
}

ComponentWidget::~ComponentWidget()
{
    delete ui;
}
