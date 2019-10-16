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

#include "ChannelListModel.h"
#include "Global.h"
#include "ChannelIndex.h"

std::unique_ptr<ChannelListModel> &ChannelListModel::ownedModels(size_t flags) {
    static std::unique_ptr<ChannelListModel> m[ChannelType::__MAX];
    return m[flags];
}

ChannelListModel *ChannelListModel::getModel(int displayFlags)
{
    if ( !ownedModels(displayFlags) ) {
        ownedModels(displayFlags).reset(new ChannelListModel(displayFlags));
        ownedModels(displayFlags)->updateChns();
    }
    return ownedModels(displayFlags).get();
}

void ChannelListModel::updateChns_static(ChannelIndex removeDeviceDex)
{
    for ( int i = 0; i < ChannelType::__MAX; i++ )
        if ( ownedModels(i) )
            ownedModels(i)->updateChns(removeDeviceDex);
}

void ChannelListModel::updateChns_static_noargs()
{
    updateChns_static(ChannelIndex());
}

void ChannelListModel::destruct()
{
    for ( int i = 0; i < ChannelType::__MAX; i++ )
        if ( ownedModels(i) )
            ownedModels(i).release()->deleteLater();
}



ChannelListModel::ChannelListModel(int displayFlags, QObject *parent)
    : QAbstractListModel(parent),
      displayFlags(displayFlags),
      size(0)
{
    for ( DAQProxy *proxy : DeviceManager::Register() )
        daqHelpers.push_back(DAQHelper(proxy, this));
    for ( ModelProxy *proxy : ModelManager::Register() )
        modelHelpers.push_back(ModelHelper(proxy, this));
    for ( ConductanceProxy *proxy : ConductanceManager::Register() )
        conductanceHelpers.push_back(ConductanceHelper(proxy, this));
    connect(&Devices, SIGNAL(removedDevice(ChannelIndex)), this, SLOT(updateChns(ChannelIndex)));
    // Owner connects model removals
}

void ChannelListModel::updateCount(ChannelListModel *from)
{
    if ( from ) {
        size = from->size;

        for ( size_t i = 0; i < modelHelpers.size(); i++ )
            modelHelpers[i].nInst = from->modelHelpers[i].nInst;
        for ( size_t i = 0; i < daqHelpers.size(); i++ ) {
            daqHelpers[i].nAI = from->daqHelpers[i].nAI;
            daqHelpers[i].nAO = from->daqHelpers[i].nAO;
        }
        for ( size_t i = 0; i < conductanceHelpers.size(); i++ )
            conductanceHelpers[i].nAssign = from->conductanceHelpers[i].nAssign;
    } else {
        size = 0;
        if ( displayFlags & Blank )
            size++;
        if ( displayFlags & None )
            size++;

        for ( ModelHelper &h : modelHelpers )
            h.updateCount();
        for ( DAQHelper &h : daqHelpers )
            h.updateCount();
        for ( ConductanceHelper &h : conductanceHelpers )
            h.updateCount();
    }
}

void ChannelListModel::DAQHelper::updateCount()
{
    if ( parent->displayFlags & AnalogIn ) {
        nAI.clear();
        for ( size_t i = 0; i < proxy->size(); i++ ) {
            int s = proxy->param(i).inChn.size();
            nAI.push_back(s);
            parent->size += s;
        }
    }
    if ( parent->displayFlags & AnalogOut ) {
        nAO.clear();
        for ( size_t i = 0; i < proxy->size(); i++ ) {
            int s = proxy->param(i).outChn.size();
            nAO.push_back(s);
            parent->size += s;
        }
    }
}

void ChannelListModel::ModelHelper::updateCount()
{
    nInst.resize(proxy->size());
    if ( parent->displayFlags & Prototype ) {
        parent->size += nInst.size();
    }
    if ( parent->displayFlags & Virtual ) {
        for ( size_t i = 0; i < proxy->size(); i++ ) {
            nInst[i] = proxy->param(i).numInst();
            parent->size += proxy->param(i).numInst();
        }
    }
}

void ChannelListModel::ConductanceHelper::updateCount()
{
    if ( parent->displayFlags & Conductance ) {
        nAssign.clear();
        for ( size_t i = 0; i < proxy->size(); i++ ) {
            int s = proxy->param(i).numAssignments();
            nAssign.push_back(s);
            parent->size += s;
        }
    }
}

void ChannelListModel::updateChns(ChannelIndex removeDeviceDex)
{
    rmDevDex = removeDeviceDex;
    QModelIndexList currentIdx, newIdx;
    ChannelListModel newM(displayFlags);
    newM.updateCount();

    // Ignore always-unchanged Blank, None

    for ( ModelHelper &h : modelHelpers )
        h.updateChns(currentIdx, newIdx, newM);
    for ( DAQHelper &h : daqHelpers )
        h.updateChns(currentIdx, newIdx, newM);
    for ( ConductanceHelper &h : conductanceHelpers )
        h.updateChns(currentIdx, newIdx, newM);

    emit layoutAboutToBeChanged();
    updateCount(&newM);
    rmDevDex = ChannelIndex();
    changePersistentIndexList(currentIdx, newIdx);
    emit layoutChanged();
}

void ChannelListModel::DAQHelper::updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM)
{
    bool rmDev;
    int mvOffset = 0;
    if ( parent->displayFlags & AnalogIn ) {
        ChannelIndex dex(proxy, 0, 0, true);
        for ( dex.devID = 0; dex.devID < nAI.size(); dex.devID++ ) {
            rmDev = parent->rmDevDex == ChannelIndex(proxy, dex.devID);
            for ( dex.chanID = 0; dex.chanID < nAI[dex.devID]; dex.chanID++ ) {
                currentIdx.append(parent->index(dex, AnalogIn));
                if ( rmDev ) {
                    newIdx.append(QModelIndex());
                } else {
                    dex.devID -= mvOffset;
                    newIdx.append(parent->createIndex(newM.index(dex, AnalogIn).row(), 0));
                    dex.devID += mvOffset;
                }
            }
            if ( rmDev )
                mvOffset = 1;
        }
    }

    mvOffset = 0;
    if ( parent->displayFlags & AnalogOut ) {
        ChannelIndex dex(proxy, 0, 0, false);
        for ( dex.devID = 0; dex.devID < nAO.size(); dex.devID++ ) {
            rmDev = parent->rmDevDex == ChannelIndex(proxy, dex.devID);
            for ( dex.chanID = 0; dex.chanID < nAO[dex.devID]; dex.chanID++ ) {
                currentIdx.append(parent->index(dex, AnalogOut));
                if ( rmDev ) {
                    newIdx.append(QModelIndex());
                } else {
                    dex.devID -= mvOffset;
                    newIdx.append(parent->createIndex(newM.index(dex, AnalogOut).row(), 0));
                    dex.devID += mvOffset;
                }
            }
            if ( rmDev )
                mvOffset = 1;
        }
    }
}

void ChannelListModel::ModelHelper::updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM)
{
    bool rmModel;
    int mvOffset = 0;
    if ( parent->displayFlags & Prototype ) {
        ChannelIndex dex(proxy);
        for ( dex.modelID = 0; dex.modelID < nInst.size(); dex.modelID++ ) {
            currentIdx.append(parent->index(dex, Prototype));
            if ( parent->rmDevDex == ChannelIndex(proxy, dex.modelID) ) {
                newIdx.append(QModelIndex());
                mvOffset = 1;
            } else {
                dex.modelID -= mvOffset;
                newIdx.append(parent->createIndex(newM.index(dex, Prototype).row(), 0));
                dex.modelID += mvOffset;
            }
        }
    }

    mvOffset = 0;
    if ( parent->displayFlags & Virtual ) {
        ChannelIndex dex(proxy, 0, 0);
        for ( dex.modelID = 0; dex.modelID < nInst.size(); dex.modelID++ ) {
            rmModel = parent->rmDevDex == ChannelIndex(proxy, dex.modelID);
            for ( dex.instID = 0; dex.instID < nInst[dex.modelID]; dex.instID++ ) {
                currentIdx.append(parent->index(dex, Virtual));
                if ( rmModel ) {
                    newIdx.append(QModelIndex());
                } else {
                    dex.modelID -= mvOffset;
                    newIdx.append(parent->createIndex(newM.index(dex, Virtual).row(), 0));
                    dex.modelID += mvOffset;
                }
            }
            if ( rmModel )
                mvOffset = 1;
        }
    }
}

void ChannelListModel::ConductanceHelper::updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM)
{
    if ( parent->displayFlags & Conductance ) {
        ChannelIndex dex(proxy, 0, 0);
        for ( dex.conductanceID = 0; dex.conductanceID < nAssign.size(); dex.conductanceID++ ) {
            for ( dex.assignID = 0; dex.assignID < nAssign[dex.conductanceID]; dex.assignID++ ) {
                currentIdx.append(parent->index(dex, Conductance));
                newIdx.append(parent->createIndex(newM.index(dex, Conductance).row(), 0));
            }
        }
    }
}

int ChannelListModel::rowCount(const QModelIndex &) const
{
    return size;
}

QVariant ChannelListModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    if ( !index.isValid() && role == Qt::UserRole ) {
        ret.setValue(ChannelIndex::None());
        return ret;
    }
    if (!index.isValid() || !(role==Qt::DisplayRole || role>=Qt::UserRole))
        return QVariant();

    int offset = 0, row = index.row();
    if ( displayFlags & Blank ) {
        if ( row == offset ) {
            ret.setValue(ChannelIndex::None());
            switch ( role ) {
            case Qt::DisplayRole:   return QString();
            case Qt::UserRole:      return ret;
            case Qt::UserRole + 1:  return QVariant(true);
            }
        }
        offset++;
    }
    if ( displayFlags & None ) {
        if ( row == offset ) {
            ret.setValue(ChannelIndex::None());
            switch ( role ) {
            case Qt::DisplayRole:   return QString("None");
            case Qt::UserRole:      return ret;
            case Qt::UserRole + 1:  return QVariant(true);
            }
        }
        offset++;
    }
    for ( ModelHelper const& h : modelHelpers )
        if ( h.data(row, role, offset, ret) )
            return ret;
    for ( DAQHelper const& h : daqHelpers )
        if ( h.data(row, role, offset, ret) )
            return ret;
    for ( ConductanceHelper const& h : conductanceHelpers )
        if ( h.data(row, role, offset, ret) )
            return ret;

    if ( role == Qt::UserRole ) {
        ret.setValue(ChannelIndex::None());
        return ret;
    } else {
        return QVariant();
    }
}

bool ChannelListModel::DAQHelper::data(int row, int role, int &offset, QVariant &ret) const
{
    bool rmDev;
    int mvOffset = 0;
    if ( parent->displayFlags & AnalogIn ) {
        for ( size_t i = 0; i < nAI.size(); i++ ) {
            // parent->rmDevDex comes through parent->updateChns(rmDevDex), called when a device is removed.
            // During the update process, data(...) is called while ChannelListModel internals and the caller
            // (i.e. the dropdown's ListView) are stale, but the outside world (i.e. Devices, params) has already changed.
            // Thus, we need to adjust indexing for calls to the outside world, omitting the rmDevDex device and shifting
            // its later siblings down into its space.
            // Correctness of data returned probably isn't too important, as this occurs /before/ the model is
            // fully updated, but let's give it the new values anyway, just in case it sticks.
            rmDev = parent->rmDevDex == ChannelIndex(proxy, i);
            if ( row-offset < (int)nAI[i] ) { // Caller and internal: Stale index
                if ( rmDev ) { // Stale caller meets removed device: Return rubbish
                    ret = QVariant();
                    return true;
                }
                ChannelIndex dex(proxy, i - mvOffset, row - offset, true); // DevID display value: Adjusted index
                DAQData *p = Devices.all()[proxy->daqClass()][dex.devID]->params(); // Outside world: Adjusted index
                switch ( role ) {
                case Qt::DisplayRole:   ret = dex.prettyName();                         return true;
                case Qt::UserRole:      ret.setValue(dex);                              return true;
                case Qt::UserRole + 1:  ret = p->active && p->inChn[dex.chanID].active; return true;
                }
            }
            offset += nAI[i];
            if ( rmDev ) // Stale caller meets moved device(s): Adjust external indices accordingly
                mvOffset = 1;
        }
    }

    mvOffset = 0;
    if ( parent->displayFlags & AnalogOut ) {
        for ( size_t i = 0; i < nAO.size(); i++ ) {
            rmDev = parent->rmDevDex == ChannelIndex(proxy, i);
            if ( row-offset < (int)nAO[i] ) {
                if ( rmDev ) {
                    ret = QVariant();
                    return true;
                }
                ChannelIndex dex(proxy, i - mvOffset, row - offset, false);
                DAQData *p = Devices.all()[proxy->daqClass()][dex.devID]->params();
                switch ( role ) {
                case Qt::DisplayRole:   ret = dex.prettyName();                          return true;
                case Qt::UserRole:      ret.setValue(dex);                               return true;
                case Qt::UserRole + 1:  ret = p->active && p->outChn[dex.chanID].active; return true;
                }
            }
            offset += nAO[i];
            if ( rmDev )
                mvOffset = 1;
        }
    }
    return false;
}

bool ChannelListModel::ModelHelper::data(int row, int role, int &offset, QVariant &ret) const
{
    int mvOffset = 0;
    if ( parent->displayFlags & Prototype ) {
        if ( row-offset < (int)nInst.size() ) {
            if ( parent->rmDevDex.isValid && parent->rmDevDex.isPrototype && parent->rmDevDex.modelClass == proxy->modelClass() ) {
                if ( (int)parent->rmDevDex.modelID == row-offset ) {
                    ret = QVariant();
                    return true;
                } else if ( (int)parent->rmDevDex.modelID < row-offset ) {
                    mvOffset = 1;
                }
            }
            ChannelIndex dex(proxy, row - offset - mvOffset);
            switch ( role ) {
            case Qt::DisplayRole:   ret = dex.prettyName();                 return true;
            case Qt::UserRole:      ret.setValue(dex);                      return true;
            case Qt::UserRole + 1:  ret = proxy->param(dex.modelID).active; return true;
            }
        }
        offset += nInst.size();
    }

    bool rmModel;
    mvOffset = 0;
    if ( parent->displayFlags & Virtual ) {
        for ( size_t i = 0; i < nInst.size(); i++ ) {
            rmModel = parent->rmDevDex == ChannelIndex(proxy, i);
            if ( row-offset < (int)nInst[i] ) {
                if ( rmModel ) {
                    ret = QVariant();
                    return true;
                }
                ChannelIndex dex(proxy, i - mvOffset, row - offset);
                ModelData &p = proxy->param(dex.modelID);
                switch ( role ) {
                case Qt::DisplayRole:   ret = dex.prettyName();                          return true;
                case Qt::UserRole:      ret.setValue(dex);                               return true;
                case Qt::UserRole + 1:  ret = p.active && p.instance(dex.instID).active; return true;
                }
            }
            offset += nInst[i];
            if ( rmModel )
                mvOffset = 1;
        }
    }
    return false;
}

bool ChannelListModel::ConductanceHelper::data(int row, int role, int &offset, QVariant &ret) const
{
    if ( parent->displayFlags & Conductance ) {
        for ( size_t i = 0; i < nAssign.size(); i++ ) {
            if ( row-offset < (int)nAssign[i] ) {
                ChannelIndex dex(proxy, i, row - offset);
                ConductanceData &p = proxy->param(dex.conductanceID);
                switch ( role ) {
                case Qt::DisplayRole:   ret = dex.prettyName();                              return true;
                case Qt::UserRole:      ret.setValue(dex);                                   return true;
                case Qt::UserRole + 1:  ret = p.active && p.assignment(dex.assignID).active; return true;
                }
            }
            offset += nAssign[i];
        }
    }
    return false;
}

Qt::ItemFlags ChannelListModel::flags(const QModelIndex &index) const
{
    return data(index, Qt::UserRole + 1).toBool()
            ? Qt::ItemIsEnabled | Qt::ItemIsSelectable
            : Qt::NoItemFlags;
}

int ChannelListModel::index(const ChannelIndex &dex) const
{
    ChannelType type = displayFlags & Blank ? Blank : None;
    if ( dex.isPrototype )
        type = Prototype;
    else if ( dex.isVirtual )
        type = Virtual;
    else if ( dex.isNone )
        type = None;
    else if ( dex.isAnalog && !dex.isInChn ) {
        type = AnalogOut;
    } else if ( dex.isAnalog && dex.isInChn ) {
        type = AnalogIn;
    } else if ( dex.isConductance ) {
        type = Conductance;
    }
    return index(dex, type).row();
}

QModelIndex ChannelListModel::index(const ChannelIndex &dex, ChannelType type) const {
    int row = 0;
    // Walk the list
    if ( type & displayFlags ) {
        if ( displayFlags & Blank ) {
            if ( type & Blank )
                return createIndex(row, 0);
            row++;
        }
        if ( displayFlags & None ) {
            if ( type & None )
                return createIndex(row, 0);
            row++;
        }
        QModelIndex ret;
        for ( ModelHelper const& h : modelHelpers )
            if ( h.index(dex, type, row, ret) )
                return ret;
        for ( DAQHelper const& h : daqHelpers )
            if ( h.index(dex, type, row, ret) )
                return ret;
        for ( ConductanceHelper const& h : conductanceHelpers )
            if ( h.index(dex, type, row, ret) )
                return ret;
    }
    return QModelIndex();
}

bool ChannelListModel::DAQHelper::index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const
{
    if ( parent->displayFlags & AnalogIn ) {
        for ( size_t i = 0; i < nAI.size(); i++ ) {
            if ( type & AnalogIn && dex.daqClass == proxy->daqClass() && dex.devID == i ) {
                if ( dex.chanID < nAI[i] )
                    ret = parent->createIndex(dex.chanID + offset, 0);
                return true;
            }
            offset += nAI[i];
        }
    }
    if ( parent->displayFlags & AnalogOut ) {
        for ( size_t i = 0; i < nAO.size(); i++ ) {
            if ( type & AnalogOut && dex.daqClass == proxy->daqClass() && dex.devID == i ) {
                if ( dex.chanID < nAO[i] )
                    ret = parent->createIndex(dex.chanID + offset, 0);
                return true;
            }
            offset += nAO[i];
        }
    }
    return false;
}

bool ChannelListModel::ModelHelper::index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const
{
    if ( parent->displayFlags & Prototype ) {
        if ( type & Prototype && dex.modelClass == proxy->modelClass() ) {
            if ( dex.modelID < nInst.size() )
                ret = parent->createIndex(dex.modelID + offset, 0);
            return true;
        }
        offset += nInst.size();
    }
    if ( parent->displayFlags & Virtual ) {
        for ( size_t i = 0; i < nInst.size(); i++ ) {
            if ( type & Virtual && dex.modelClass == proxy->modelClass() && dex.modelID == i ) {
                if ( dex.instID < nInst[i] )
                    ret = parent->createIndex(dex.instID + offset, 0);
                return true;
            }
            offset += nInst[i];
        }
    }
    return false;
}

bool ChannelListModel::ConductanceHelper::index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const
{
    if ( parent->displayFlags & Conductance ) {
        for ( size_t i = 0; i < nAssign.size(); i++ ) {
            if ( type & Conductance && dex.conductanceClass == proxy->conductanceClass() && dex.conductanceID == i ) {
                if ( dex.assignID < nAssign[i] )
                    ret = parent->createIndex(dex.assignID + offset, 0);
                return true;
            }
            offset += nAssign[i];
        }
    }
    return false;
}
