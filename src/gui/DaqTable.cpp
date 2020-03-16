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

#include "DaqTable.h"
#include "MainWin.h"

DaqTable::DaqTable(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(1);
}

DaqTable::~DaqTable()
{
    for ( DaqOptsPrototypeBase *p : proto )
        delete p;
}

void DaqTable::makeFactory()
{
    factory = new DaqFactoryWidget(this);
    connect(factory->button, SIGNAL(clicked(bool)), this, SLOT(addDaqOpts()));
    for ( DaqOptsPrototypeBase *p : proto )
        factory->combo->addItem(p->label());
}

void DaqTable::init(QVector<DaqOptsPrototypeBase *> prototypes, QWidget *parent)
{
    proto = prototypes;
    idx = QVector<int>(proto.size(), 0);
    this->parent = parent;
    makeFactory();
    setCellWidget(0, 0, factory);
}

void DaqTable::importData(bool activeOnly)
{
    clear();
    comp.clear();
    int i = 0;
    for ( DaqOptsPrototypeBase *p : proto ) {
        if ( activeOnly )
            p->clearInactive();
        else
            p->createAll(parent);
        comp += p->inst;
        idx[i++] = p->inst.size();
    }
    i = 0;
    setColumnCount(comp.size() + 1);
    for ( DaqOptsBase *c : comp ) {
        setCellWidget(0, i++, c->widget());
    }
    makeFactory();
    setCellWidget(0, i, factory);
    if ( activeOnly )
        static_cast<MyMainWindow*>(parent)->updateStartButton();
}

void DaqTable::exportData(bool ignoreDAQ)
{
    for ( DaqOptsPrototypeBase *p : proto )
        p->exportData(ignoreDAQ);
}

void DaqTable::addDaqOpts()
{
    int i = factory->combo->currentIndex();
    DaqOptsBase *c = proto[i]->create(idx[i]++, parent);
    c->exportData();
    insertColumn(comp.size());
    setCellWidget(0, comp.size(), c->widget());
    comp.append(c);
    scrollTo(model()->index(0, comp.size()));
}
