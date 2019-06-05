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

#ifndef DAQTABLE_H
#define DAQTABLE_H

#include <QTableWidget>
#include "DaqOptsBase.h"

class DaqTable : public QTableWidget
{
    Q_OBJECT

public:
    DaqTable(QWidget *parent = 0);
    ~DaqTable();

    void init(QVector<DaqOptsPrototypeBase *> prototypes, QWidget *parent);

public slots:
    void importData(bool activeOnly = false);
    void exportData(bool ignoreDAQ = false);

private slots:
    void addDaqOpts();

private:
    QVector<DaqOptsPrototypeBase *> proto;
    QVector<DaqOptsBase *> comp;
    QVector<int> idx;
    DaqFactoryWidget *factory;
    QWidget *parent;

    void makeFactory();
};

#endif // DAQTABLE_H
