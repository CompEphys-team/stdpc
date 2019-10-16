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

#include "treewidgetitem_models.h"
#include "ui_treewidgetitem_models.h"

TreeWidgetItem_Models::TreeWidgetItem_Models(const QMap<QString, ConductanceProxy *> &proxies, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeWidgetItem_Models)
{
    ui->setupUi(this);

    QVector<ComponentPrototype *> prototypes;
    for ( ConductanceProxy *proxy : proxies )
        prototypes.push_back(new ComponentPrototype(proxy));
    ui->toolsTable->init(prototypes);

    connect(ui->ToolsActivate, SIGNAL(clicked(bool)), ui->toolsTable, SLOT(activateAll()));
    connect(ui->ToolsDeactivate, SIGNAL(clicked(bool)), ui->toolsTable, SLOT(deactivateAll()));
    connect(ui->ToolsClear, &QPushButton::clicked, [=](){ui->toolsTable->importData(true);});
    connect(ui->ToolsReset, SIGNAL(clicked(bool)), ui->toolsTable, SLOT(importData()));
}

TreeWidgetItem_Models::~TreeWidgetItem_Models()
{
    delete ui;
}

void TreeWidgetItem_Models::importData(bool activeOnly)
{
    ui->toolsTable->importData(activeOnly);
}

void TreeWidgetItem_Models::exportData()
{
    ui->toolsTable->exportData();
}
