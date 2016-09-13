#include "ChannelListModel.h"
#include "Global.h"
#include "ChannelIndex.h"
#include <QAbstractItemView>
#include <QApplication>

ChannelListModel::ChannelListModel(int displayFlags, QObject *parent)
    : QAbstractListModel(parent),
      displayFlags(displayFlags),
      size(0),
      hSimul(DAQHelper(DAQClass::Simul, &SDAQp, this)),
      hDD1200(DAQHelper(DAQClass::DD1200, &DigiDatap, this)),
      hNI(DAQHelper(DAQClass::NI, &NIDAQp, this)),
      nPHH(0)
{
}

void ChannelListModel::updateCount(ChannelListModel *from)
{
    if ( from ) {
        size = from->size;

        hSimul.nAI = from->hSimul.nAI;
        hSimul.nAO = from->hSimul.nAO;
        hDD1200.nAI = from->hDD1200.nAI;
        hDD1200.nAO = from->hDD1200.nAO;
        hNI.nAI = from->hNI.nAI;
        hNI.nAO = from->hNI.nAO;

        nPHH = from->nPHH;
        nVHH = from->nVHH;
    } else {
        int s;
        size = 0;
        if ( displayFlags & Blank )
            size++;
        if ( displayFlags & None )
            size++;
        if ( displayFlags & SpikeGen )
            size++;
        if ( displayFlags & Prototype ) {
            nPHH = HHNeuronp.size();
            size += nPHH;
        }
        if ( displayFlags & Virtual ) {
            nVHH.clear();
            for ( HHNeuronData &model : HHNeuronp ) {
                s = model.inst.size();
                nVHH.push_back(s);
                size += s;
            }
        }
        hSimul.updateCount();
        hDD1200.updateCount();
        hNI.updateCount();
    }
}

void ChannelListModel::DAQHelper::updateCount()
{
    if ( parent->displayFlags & AnalogIn ) {
        int s = p->inChn.size();
        nAI = s;
        parent->size += s;
    }
    if ( parent->displayFlags & AnalogOut ) {
        int s = p->outChn.size();
        nAO = s;
        parent->size += s;
    }
}

void ChannelListModel::updateChns()
{
    ChannelIndex dex, blank;
    blank.isValid = true;
    dex = blank;
    QModelIndexList currentIdx, newIdx;
    ChannelListModel newM(displayFlags);
    newM.updateCount();

    // Ignore always-unchanged Blank, None, SpikeGen
    if ( displayFlags & Prototype ) {
        dex.isPrototype = true;
        dex.modelClass = ModelClass::HH;
        for ( dex.modelID = 0; dex.modelID < nPHH; dex.modelID++ ) {
            currentIdx.append(index(dex, Prototype));
            newIdx.append(createIndex(newM.index(dex, Prototype).row(), 0));
        }
    }
    dex = blank;
    if ( displayFlags & Virtual ) {
        dex.isVirtual = true;
        dex.modelClass = ModelClass::HH;
        for ( dex.modelID = 0; dex.modelID < nVHH.size(); dex.modelID++ ) {
            for ( dex.instID = 0; dex.instID < nVHH[dex.modelID]; dex.instID++ ) {
                currentIdx.append(index(dex, Virtual));
                newIdx.append(createIndex(newM.index(dex, Virtual).row(), 0));
            }
        }
    }
    hSimul.updateChns(currentIdx, newIdx, newM);
    hDD1200.updateChns(currentIdx, newIdx, newM);
    hNI.updateChns(currentIdx, newIdx, newM);

    emit layoutAboutToBeChanged();
    updateCount(&newM);
    changePersistentIndexList(currentIdx, newIdx);
    emit layoutChanged();
}

void ChannelListModel::DAQHelper::updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM)
{
    ChannelIndex dex;
    dex.isValid = true;
    if ( parent->displayFlags & AnalogIn ) {
        dex.isAnalog = true;
        dex.isInChn = true;
        dex.daqClass = daqClass;
        dex.devID = 0;
        for ( dex.chanID = 0; dex.chanID < nAI; dex.chanID++ ) {
            currentIdx.append(parent->index(dex, AnalogIn));
            newIdx.append(parent->createIndex(newM.index(dex, AnalogIn).row(), 0));
        }
    }
    if ( parent->displayFlags & AnalogOut ) {
        dex.isAnalog = true;
        dex.isInChn = false;
        dex.daqClass = daqClass;
        dex.devID = 0;
        for ( dex.chanID = 0; dex.chanID < nAO; dex.chanID++ ) {
            currentIdx.append(parent->index(dex, AnalogOut));
            newIdx.append(parent->createIndex(newM.index(dex, AnalogOut).row(), 0));
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
    ChannelIndex dex;
    dex.isValid = true;
    if ( !index.isValid() && role == Qt::UserRole ) {
        dex.isNone = true;
        ret.setValue(dex);
        return ret;
    }
    if (!index.isValid() || !(role==Qt::DisplayRole || role>=Qt::UserRole))
        return QVariant();

    int offset = 0, row = index.row();
    if ( displayFlags & Blank ) {
        if ( row == offset ) {
            dex.isNone = true;
            ret.setValue(dex);
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
            dex.isNone = true;
            ret.setValue(dex);
            switch ( role ) {
            case Qt::DisplayRole:   return QString("None");
            case Qt::UserRole:      return ret;
            case Qt::UserRole + 1:  return QVariant(true);
            }
        }
        offset++;
    }
    if ( displayFlags & SpikeGen ) {
        if ( row == offset ) {
            dex.isSG = true;
            ret.setValue(dex);
            switch ( role ) {
            case Qt::DisplayRole:   return QString("Spike generator");
            case Qt::UserRole:      return ret;
            case Qt::UserRole + 1:  return QVariant(true);
            }
        }
        offset++;
    }
    if ( displayFlags & Prototype ) {
        if ( row-offset < nPHH ) {
            dex.isPrototype = true;
            dex.modelClass = ModelClass::HH;
            dex.modelID = row-offset;
            ret.setValue(dex);
            switch ( role ) {
            case Qt::DisplayRole:   return QString("HH %1:all (model %1, all instances)").arg(row-offset+1);
            case Qt::UserRole:      return ret;
            case Qt::UserRole + 1:  return QVariant(HHNeuronp[row-offset].active);
            }
        }
        offset += nPHH;
    }
    if ( displayFlags & Virtual ) {
        for ( int i = 0; i < nVHH.size(); i++ ) {
            if ( row-offset < nVHH[i] ) {
                dex.isVirtual = true;
                dex.modelClass = ModelClass::HH;
                dex.modelID = i;
                dex.instID = row-offset;
                ret.setValue(dex);
                switch ( role ) {
                case Qt::DisplayRole:   return QString("HH %1:%2 (model %1, instance %2)").arg(i+1).arg(row-offset+1);
                case Qt::UserRole:      return ret;
                case Qt::UserRole + 1:  return QVariant(HHNeuronp[i].inst[row-offset].active);
                }
            }
            offset += nVHH[i];
        }
    }
    if ( hSimul.data(row, role, offset, ret) )
        return ret;
    if ( hDD1200.data(row, role, offset, ret) )
        return ret;
    if ( hNI.data(row, role, offset, ret) )
        return ret;

    if ( role == Qt::UserRole ) {
        dex.isNone = true;
        ret.setValue(dex);
        return ret;
    } else {
        return QVariant();
    }
}

bool ChannelListModel::DAQHelper::data(int row, int role, int &offset, QVariant &ret) const
{
    ChannelIndex dex;
    dex.isValid = true;
    dex.isAnalog = true;
    dex.daqClass = daqClass;
    if ( parent->displayFlags & AnalogIn ) {
        if ( row-offset < nAI ) {
            dex.devID = 0;
            dex.isInChn = true;
            dex.chanID = row - offset;
            switch ( role ) {
            case Qt::DisplayRole:   ret = dex.prettyName();                         return true;
            case Qt::UserRole:      ret.setValue(dex);                              return true;
            case Qt::UserRole + 1:  ret = p->active && p->inChn[dex.chanID].active; return true;
            }
        }
        offset += nAI;
    }
    if ( parent->displayFlags & AnalogOut ) {
        if ( row-offset < nAO ) {
            dex.devID = 0;
            dex.isInChn = false;
            dex.chanID = row - offset;
            switch ( role ) {
            case Qt::DisplayRole:   ret = dex.prettyName();                          return true;
            case Qt::UserRole:      ret.setValue(dex);                               return true;
            case Qt::UserRole + 1:  ret = p->active && p->outChn[dex.chanID].active; return true;
            }
        }
        offset += nAO;
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
    else if ( dex.isSG )
        type = SpikeGen;
    else if ( dex.isNone )
        type = None;
    else if ( dex.isAnalog && !dex.isInChn ) {
        type = AnalogOut;
    } else if ( dex.isAnalog && dex.isInChn ) {
        type = AnalogIn;
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
        if ( displayFlags & SpikeGen ) {
            if ( type & SpikeGen ) {
                return createIndex(row, 0);
            }
            row++;
        }
        if ( displayFlags & Prototype ) {
            if ( type & Prototype && dex.modelClass == ModelClass::HH ) {
                if ( dex.modelID >= 0 && dex.modelID < nPHH )
                    return createIndex(row + dex.modelID, 0);
                else
                    return QModelIndex();
            }
            row += nPHH;
        }
        if ( displayFlags & Virtual ) {
            for ( int i = 0; i < nVHH.size(); i++ ) {
                if ( type & Virtual && dex.modelClass == ModelClass::HH && dex.modelID == i ) {
                    if ( dex.instID >= 0 && dex.instID < nVHH[i] )
                        return createIndex(row + dex.instID, 0);
                    else
                        return QModelIndex();
                }
                row += nVHH[i];
            }
        }
        QModelIndex ret;
        if ( hSimul.index(dex, type, row, ret) )
            return ret;
        if ( hDD1200.index(dex, type, row, ret) )
            return ret;
        if ( hNI.index(dex, type, row, ret) )
            return ret;
    }
    return QModelIndex();
}

bool ChannelListModel::DAQHelper::index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const
{
    if ( parent->displayFlags & AnalogIn ) {
        if ( type & AnalogIn && dex.daqClass == daqClass ) {
            if ( dex.chanID >= 0 && dex.chanID < nAI )
                ret = parent->createIndex(dex.chanID + offset, 0);
            return true;
        }
        offset += nAI;
    }
    if ( parent->displayFlags & AnalogOut ) {
        if ( type & AnalogOut && dex.daqClass == daqClass ) {
            if ( dex.chanID >= 0 && dex.chanID < nAO )
                ret = parent->createIndex(dex.chanID + offset, 0);
            return true;
        }
        offset += nAO;
    }
    return false;
}

void ChannelListModel::subordinate(QComboBox *cb)
{
    cb->setModel(this);
    connect(this, &ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(cb);});
    fixComboBoxWidth(cb);
}

void ChannelListModel::fixComboBoxWidth(QComboBox *cb)
{
    Qt::TextElideMode old = cb->view()->textElideMode();
    cb->view()->setTextElideMode(Qt::ElideNone);
    int scroll = cb->count() <= cb->maxVisibleItems() ? 0 :
        QApplication::style()->pixelMetric(QStyle::PixelMetric::PM_ScrollBarExtent);

    int max = 0;

    for (int i = 0; i < cb->count(); i++)
    {
        int width = cb->view()->fontMetrics().width(cb->itemText(i));
        if (max < width)
            max = width;
    }

    cb->view()->setMinimumWidth(scroll + max + 10);
    cb->view()->setTextElideMode(old);
}
