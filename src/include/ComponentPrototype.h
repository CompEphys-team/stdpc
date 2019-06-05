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

#ifndef COMPONENTPROTOTYPE_H
#define COMPONENTPROTOTYPE_H

#include "ChannelListModel.h"
#include "Component.h"

class ComponentPrototypeBase
{
public:
    ComponentPrototypeBase(QString label) :
        label(label),
        inst()
    {}

    virtual ~ComponentPrototypeBase()
    {
        for ( GenericComponent *c : inst )
            delete c;
    }

    virtual GenericComponent *create(int idx, ChannelListModel *in, ChannelListModel *out) = 0;
    virtual void createAll(ChannelListModel *in, ChannelListModel *out) = 0;

    void clearInactive()
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

    virtual void exportData() = 0;

    QString label;
    QVector<GenericComponent *> inst;
};

template <class ComponentDlg>
class ComponentPrototype : public ComponentPrototypeBase
{
public:
    ComponentPrototype(QString const& label, std::vector<typename ComponentDlg::param_type> *params) :
        ComponentPrototypeBase(label),
        params(params)
    {}
    ~ComponentPrototype() {}

    GenericComponent *create(int idx, ChannelListModel *in, ChannelListModel *out)
    {
        Component<ComponentDlg> *c = new Component<ComponentDlg>(label, params, idx, in, out);
        inst.push_back(c);
        return c;
    }

    void createAll(ChannelListModel *in, ChannelListModel *out)
    {
        for ( GenericComponent *c : inst )
            delete c;
        inst.clear();
        inst.reserve(params->size());
        for ( size_t i = 0; i < params->size(); i++ ) {
            create(i, in, out)->importData();
        }
    }

    void exportData()
    {
        params->resize(inst.size());
        for ( GenericComponent *c : inst )
            c->exportData();
    }

private:
    std::vector<typename ComponentDlg::param_type> *params;
};

#endif // COMPONENTPROTOTYPE_H
