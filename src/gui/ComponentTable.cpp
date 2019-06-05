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

#include "ComponentTable.h"

ComponentTable::ComponentTable(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(1);
}

ComponentTable::~ComponentTable()
{
    for ( ComponentPrototype *p : proto )
        delete p;
}

void ComponentTable::makeFactory()
{
    factory = new ComponentFactoryWidget(this);
    connect(factory->button, SIGNAL(clicked(bool)), this, SLOT(addComponent()));
    for ( ComponentPrototype *p : proto )
        factory->combo->addItem(p->label());
}

void ComponentTable::init(QVector<ComponentPrototype *> prototypes)
{
    proto = prototypes;
    idx = QVector<int>(proto.size(), 0);
    makeFactory();
    setCellWidget(0, 0, factory);
}

void ComponentTable::importData(bool activeOnly)
{
    clear();
    comp.clear();
    int i = 0;
    for ( ComponentPrototype *p : proto ) {
        if ( activeOnly )
            p->clearInactive();
        else
            p->createAll();
        comp += p->inst;
        idx[i++] = p->inst.size();
    }
    i = 0;
    setColumnCount(comp.size() + 1);
    for ( Component *c : comp ) {
        setCellWidget(0, i++, c->widget());
    }

    makeFactory();
    setCellWidget(0, i, factory);
}

void ComponentTable::exportData()
{
    for ( ComponentPrototype *p : proto )
        p->exportData();
}

void ComponentTable::activateAll()
{
    for ( Component *c : comp )
        c->widget()->active->setChecked(true);
}

void ComponentTable::deactivateAll()
{
    for ( Component *c : comp )
        c->widget()->active->setChecked(false);
}

void ComponentTable::addComponent()
{
    int i = factory->combo->currentIndex();
    Component *c = proto[i]->create(idx[i]++);
    insertColumn(comp.size());
    setCellWidget(0, comp.size(), c->widget());
    comp.append(c);
    scrollTo(model()->index(0, comp.size()));
}
