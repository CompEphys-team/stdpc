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

#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentWidget.h"
#include "ChannelListModel.h"

class Component : public QObject
{
    Q_OBJECT
public:
    Component(ConductanceProxy *proxy, size_t idx);

    void importData();
    void exportData();

    void setIndex(int i);

    inline ComponentWidget *widget() { return _widget; }
    void regenerateWidget();

protected:
    ConductanceProxy *proxy;
    size_t idx;
    ComponentWidget *_widget;
    ConductanceDlg *dlg;

protected slots:
    void setLabel();
    void setExplicitLabel(QString label);
};

class ComponentPrototype
{
public:
    ComponentPrototype(ConductanceProxy *proxy);
    ~ComponentPrototype();

    inline QString label() { return proxy->prettyName(); }

    Component *create(int idx);
    void createAll();
    void clearInactive();
    void exportData();

    QVector<Component *> inst;

protected:
    ConductanceProxy *proxy;
};

#endif // COMPONENT_H
