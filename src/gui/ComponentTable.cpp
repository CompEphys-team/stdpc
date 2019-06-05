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
    for ( ComponentPrototypeBase *p : proto )
        delete p;
}

void ComponentTable::makeFactory()
{
    factory = new ComponentFactoryWidget(this);
    connect(factory->button, SIGNAL(clicked(bool)), this, SLOT(addComponent()));
    for ( ComponentPrototypeBase *p : proto )
        factory->combo->addItem(p->label);
}

void ComponentTable::init(QVector<ComponentPrototypeBase *> prototypes, ChannelListModel *in, ChannelListModel *out)
{
    proto = prototypes;
    idx = QVector<int>(proto.size(), 0);
    this->in = in;
    this->out = out;
    makeFactory();
    setCellWidget(0, 0, factory);
}

void ComponentTable::importData(bool activeOnly)
{
    clear();
    comp.clear();
    int i = 0;
    for ( ComponentPrototypeBase *p : proto ) {
        if ( activeOnly )
            p->clearInactive();
        else
            p->createAll(in, out);
        comp += p->inst;
        idx[i++] = p->inst.size();
    }
    i = 0;
    setColumnCount(comp.size() + 1);
    for ( GenericComponent *c : comp ) {
        setCellWidget(0, i++, c->widget());
    }

    makeFactory();
    setCellWidget(0, i, factory);
}

void ComponentTable::exportData()
{
    for ( ComponentPrototypeBase *p : proto )
        p->exportData();
}

void ComponentTable::activateAll()
{
    for ( GenericComponent *c : comp )
        c->widget()->active->setChecked(true);
}

void ComponentTable::deactivateAll()
{
    for ( GenericComponent *c : comp )
        c->widget()->active->setChecked(false);
}

void ComponentTable::addComponent()
{
    int i = factory->combo->currentIndex();
    GenericComponent *c = proto[i]->create(idx[i]++, in, out);
    insertColumn(comp.size());
    setCellWidget(0, comp.size(), c->widget());
    comp.append(c);
    scrollTo(model()->index(0, comp.size()));
}
