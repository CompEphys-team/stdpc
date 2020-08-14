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

#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QComboBox>
#include "ChannelIndex.h"
#include "ObjectDataTypes.h"
#include "ModelManager.h"
#include "DeviceManager.h"
#include "ConductanceManager.h"

class ChannelListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ChannelListModel(int displayFlags, QObject *parent = nullptr);

    enum ChannelType {
        Blank =      1,
        None =       2,
        Conductance = 4,
        Virtual   =  8, // Model instances
        AnalogIn =   16,
        AnalogOut =  32,
        Prototype =  64, // Model prototypes, applies multiplexed over all instances
        Directional = 128, // Model prototypes and instances: Separate entries for input and output channel, directional ChannelIndex

        __MAX = 256, // Internal use only: Maximum enum value

        In = AnalogIn | Virtual | Prototype,
        Out = AnalogOut | Virtual | Prototype
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    using QAbstractListModel::index; // squelch -Woverloaded-virtual
    int index(const ChannelIndex &dex) const;
    QModelIndex index(const ChannelIndex &dex, ChannelType type) const;

    static ChannelListModel *getModel(int displayFlags);
    static void updateChns_static(ChannelIndex removeDeviceDex);
    static void updateChns_static_noargs();
    static void destruct();

public slots:
    void updateChns(ChannelIndex removeDeviceDex = ChannelIndex());

protected:
    void updateCount(ChannelListModel *from = nullptr);

    class DAQHelper {
    private:
        ChannelListModel *const parent;
        DAQProxy *const proxy;
    public:
        DAQHelper(DAQProxy *proxy, ChannelListModel *parent) : parent(parent), proxy(proxy), nAI(0), nAO(0) {}
        void updateCount();
        void updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM);
        bool data(int row, int role, int &offset, QVariant &ret) const;
        bool index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const;
        std::vector<size_t> nAI, nAO;
    };

    class ModelHelper {
    private:
        ChannelListModel *const parent;
        ModelProxy *proxy;
        bool inChn;
    public:
        ModelHelper(ModelProxy *proxy, ChannelListModel *parent, bool inChn = true) :
            parent(parent), proxy(proxy), inChn(inChn), nInst(0) {}
        void updateCount();
        void updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM);
        bool data(int row, int role, int &offset, QVariant &ret) const;
        bool index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const;
        std::vector<size_t> nInst;
    };

    class ConductanceHelper {
    private:
        ChannelListModel *const parent;
        ConductanceProxy *proxy;
    public:
        ConductanceHelper(ConductanceProxy *proxy, ChannelListModel *parent) : parent(parent), proxy(proxy), nAssign(0) {}
        void updateCount();
        void updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM);
        bool data(int row, int role, int &offset, QVariant &ret) const;
        bool index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const;
        std::vector<size_t> nAssign;
    };

    const int displayFlags;
    int size;

    std::vector<DAQHelper> daqHelpers;
    std::vector<ModelHelper> modelHelpers;
    std::vector<ConductanceHelper> conductanceHelpers;

    ChannelIndex rmDevDex;

    static std::unique_ptr<ChannelListModel> &ownedModels(size_t flags);
};

#endif // CHANNELLISTMODEL_H
