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

#ifndef COMPONENTTABLE_H
#define COMPONENTTABLE_H

#include <QTableWidget>
#include "Component.h"

class ComponentTable : public QTableWidget
{
    Q_OBJECT

public:
    ComponentTable(QWidget *parent = nullptr);
    ~ComponentTable();

    void init(QVector<ComponentPrototype *> prototypes);

public slots:
    void importData(bool activeOnly = false);
    void exportData();

    void activateAll();
    void deactivateAll();

private slots:
    void addComponent();

private:
    QVector<ComponentPrototype *> proto;
    QVector<Component *> comp;
    QVector<int> idx;
    ComponentFactoryWidget *factory;

    void makeFactory();
};

#endif // COMPONENTTABLE_H
