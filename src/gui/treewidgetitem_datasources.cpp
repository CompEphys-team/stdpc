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

#include "treewidgetitem_datasources.h"
#include "ui_treewidgetitem_datasources.h"
#include "DaqOpts.h"
#include "ModelOpts.h"

TreeWidgetItem_DataSources::TreeWidgetItem_DataSources(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeWidgetItem_DataSources)
{
    ui->setupUi(this);

    QVector<DaqOptsPrototypeBase*> prototypes;
    for ( DAQProxy *proxy : DeviceManager::Register() )
        prototypes.push_back(new DaqOptsPrototype(proxy));
    for ( ModelProxy *proxy : ModelManager::Register() )
        prototypes.push_back(new ModelOptsPrototype(proxy));
    ui->DAQTable->init(prototypes, parent);

    connect(ui->DAQClear, &QPushButton::clicked, [=](){ui->DAQTable->importData(true);});
}

TreeWidgetItem_DataSources::~TreeWidgetItem_DataSources()
{
    delete ui;
}

void TreeWidgetItem_DataSources::importData(bool activeOnly)
{
    ui->DAQTable->importData(activeOnly);
}

void TreeWidgetItem_DataSources::exportData(bool ignoreDAQ)
{
    ui->DAQTable->exportData(ignoreDAQ);
}
