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

#ifndef DAQOPTSBASE_H
#define DAQOPTSBASE_H

#include "DaqWidget.h"

class DaqOptsBase : public QObject
{
    Q_OBJECT

public:
    DaqOptsBase(QWidget *parent, size_t idx) : QObject(parent), idx(idx) {}
    virtual ~DaqOptsBase() {}
    virtual void removeDevice() = 0;

    virtual void importData() = 0;
    virtual void exportData() = 0;

    virtual void setIndex(size_t i) = 0;

    virtual DaqWidget *widget() = 0;
    virtual void regenerateWidget() = 0;

signals:
    void removedModel(ChannelIndex);

protected slots:
    virtual void activeChanged() = 0;
    virtual void setLabel() = 0;

protected:
    size_t idx;
};

class DaqOptsPrototypeBase
{
public:
    DaqOptsPrototypeBase() :
        inst()
    {}

    virtual ~DaqOptsPrototypeBase()
    {
        for ( DaqOptsBase *c : inst )
            delete c;
    }

    virtual QString label() = 0;

    virtual DaqOptsBase *create(size_t idx, QWidget *parent) = 0;
    virtual void createAll(QWidget *parent) = 0;

    void clearInactive()
    {
        for ( int i = 0; i < inst.size(); i++ ) {
            if ( inst[i]->widget()->active->isChecked() ) {
                inst[i]->regenerateWidget();
                inst[i]->widget()->active->setChecked(true);
                inst[i]->setIndex(i);
            } else {
                inst[i]->removeDevice();
                delete inst[i];
                inst.remove(i);
                --i;
            }
        }
    }

    virtual void exportData(bool ignoreDAQ = false) = 0;

    QVector<DaqOptsBase *> inst;
};

#endif // DAQOPTSBASE_H
