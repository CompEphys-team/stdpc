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

#ifndef DAQOPTS_H
#define DAQOPTS_H

#include "DaqOptsBase.h"
#include "DAQDlg.h"

class DaqOpts : public DaqOptsBase
{
public:
    DaqOpts(QWidget *parent, DAQProxy *proxy, size_t idx);
    ~DaqOpts();

    void removeDevice();
    void importData();
    void exportData();
    void setIndex(size_t i);
    inline DaqWidget *widget() { return _widget; }
    void regenerateWidget();

protected:
    void activeChanged();

    DAQProxy *proxy;
    DAQDlg *dlg;
    DaqWidget *_widget;
};

class DaqOptsPrototype : public DaqOptsPrototypeBase
{
public:
    DaqOptsPrototype(DAQProxy *proxy) : proxy(proxy) {}
    ~DaqOptsPrototype() {}

    inline QString label() { return proxy->prettyName(); }

    DaqOptsBase *create(size_t idx, QWidget *parent);
    void createAll(QWidget *parent);
    void exportData(bool ignoreDAQ = false);

private:
    DAQProxy *proxy;
};

#endif // DAQOPTS_H
