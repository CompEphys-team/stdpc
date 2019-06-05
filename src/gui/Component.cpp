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

#include "Component.h"


Component::Component(ConductanceProxy *proxy, size_t idx) :
    proxy(proxy), idx(idx), dlg(proxy->createDialog(idx))
{
    regenerateWidget();
    setIndex(idx);
    connect(dlg, &ConductanceDlg::labelChanged, this, &Component::setExplicitLabel);
}

void Component::importData()
{
    dlg->importData();
    _widget->active->setChecked(proxy->param(idx).active);
    _widget->activeSettling->setChecked(proxy->param(idx).activeSettling);
}

void Component::exportData()
{
    if ( idx >= proxy->size() )
        proxy->resize(idx+1);
    proxy->param(idx).active = _widget->active->isChecked();
    proxy->param(idx).activeSettling = _widget->activeSettling->isChecked();
    dlg->exportData();
}

void Component::setIndex(int i)
{
    idx = i;
    setLabel();
    dlg->setIndex(idx);
}

void Component::setLabel()
{
    QString label;
    if ( proxy->size() > idx )
        label = proxy->param(idx).label;
    setExplicitLabel(label);
}

void Component::setExplicitLabel(QString label)
{
    if ( label.isEmpty() )
        label = QString("%1 %2").arg(proxy->prettyName()).arg(idx);
    _widget->label->setText(label);
}

void Component::regenerateWidget()
{
    _widget = new ComponentWidget();
    QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(show()));
    QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(raise()));
}



ComponentPrototype::ComponentPrototype(ConductanceProxy *proxy) :
    inst(),
    proxy(proxy)
{}

ComponentPrototype::~ComponentPrototype()
{
    for ( Component *c : inst )
        delete c;
}



Component *ComponentPrototype::create(int idx) {
    Component *c = new Component(proxy, idx);
    inst.push_back(c);
    return c;
}

void ComponentPrototype::createAll() {
    for ( Component *c : inst )
        delete c;
    inst.clear();
    inst.reserve(proxy->size());
    for ( size_t i = 0; i < proxy->size(); i++ ) {
        create(i)->importData();
    }
}

void ComponentPrototype::clearInactive()
{
    for ( int i = 0; i < inst.size(); i++ ) {
        if ( inst[i]->widget()->active->isChecked() ) {
            inst[i]->regenerateWidget();
            inst[i]->setIndex(i);
        } else {
            delete inst[i];
            inst.remove(i);
            --i;
        }
    }
}

void ComponentPrototype::exportData()
{
    proxy->resize(inst.size());
    for ( Component *c : inst )
        c->exportData();
}
