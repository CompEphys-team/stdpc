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

#ifndef TREEWIDGETITEM_MODELS_H
#define TREEWIDGETITEM_MODELS_H

#include <QWidget>
#include "ConductanceManager.h"

namespace Ui {
class TreeWidgetItem_Models;
}

class TreeWidgetItem_Models : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidgetItem_Models(const QMap<QString, ConductanceProxy *> &proxies, QWidget *parent = nullptr);
    ~TreeWidgetItem_Models();

public slots:
    void importData(bool activeOnly = false);
    void exportData();

private:
    Ui::TreeWidgetItem_Models *ui;
};

#endif // TREEWIDGETITEM_MODELS_H
