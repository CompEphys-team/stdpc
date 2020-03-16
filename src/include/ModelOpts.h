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

#ifndef MODELOPTS_H
#define MODELOPTS_H

#include "DaqOptsBase.h"
#include "ModelManager.h"

class ModelOpts : public DaqOptsBase
{
public:
    ModelOpts(QWidget *parent, ModelProxy *proxy, size_t idx);
    ~ModelOpts();

    void removeDevice();
    void importData();
    void exportData();
    void setIndex(size_t i);
    inline DaqWidget *widget() { return _widget; }
    void regenerateWidget();

protected:
    void activeChanged();

    ModelProxy *proxy;
    ModelDlg *dlg;
    DaqWidget *_widget;

protected slots:
    void setLabel();
};

class ModelOptsPrototype : public DaqOptsPrototypeBase
{
public:
    ModelOptsPrototype(ModelProxy *proxy) : proxy(proxy) {}
    ~ModelOptsPrototype() {}

    inline QString label() { return proxy->prettyName(); }

    DaqOptsBase *create(size_t idx, QWidget *parent);
    void createAll(QWidget *parent);
    void exportData(bool);

protected:
    ModelProxy *proxy;
};

#endif // MODELOPTS_H
