#include "ChannelListModel.h"
#include "Global.h"
#include "ChannelIndex.h"
#include <QAbstractItemView>
#include <QApplication>

ChannelListModel::ChannelListModel(int displayFlags, QObject *parent)
    : QAbstractListModel(parent),
      displayFlags(displayFlags),
      size(0), nAI(0), nAO(0), nPHH(0),
      offsetAnalogs(displayFlags&AnalogOut && displayFlags&AnalogIn)
{
}

void ChannelListModel::updateCount(ChannelListModel *swapAgainst)
{
    if ( swapAgainst ) {
        using std::swap;
        swap(size, swapAgainst->size);
        swap(nPHH, swapAgainst->nPHH);
        swap(nVHH, swapAgainst->nVHH);
        swap(nAI, swapAgainst->nAI);
        swap(nAO, swapAgainst->nAO);
    } else {
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
                int s = model.inst.size();
                nVHH.push_back(s);
                size += s;
            }
        }
        if ( displayFlags & AnalogIn ) {
            nAI = inChnp.size();
            size += nAI;
        }
        if ( displayFlags & AnalogOut ) {
            nAO = outChnp.size();
            size += nAO;
        }
    }
}

void ChannelListModel::updateChns()
{
    ChannelIndex dex;
    QModelIndexList currentIdx, newIdx;
    ChannelListModel newM(displayFlags);
    newM.updateCount();

    // Ignore always-unchanged Blank, None, SpikeGen
    if ( displayFlags & Prototype ) {
        dex.isPrototype = true;
        dex.isVirtual = true;
        dex.modelClass = ChannelIndex::HH;
        for ( dex.modelID = 0; dex.modelID < nPHH; dex.modelID++ ) {
            currentIdx.append(index(dex, Prototype));
            newIdx.append(createIndex(newM.index(dex, Prototype).row(), 0));
        }
    }
    if ( displayFlags & Virtual ) {
        dex.isPrototype = false;
        dex.isVirtual = true;
        dex.modelClass = ChannelIndex::HH;
        for ( dex.modelID = 0; dex.modelID < nVHH.size(); dex.modelID++ ) {
            for ( dex.instID = 0; dex.instID < nVHH[dex.modelID]; dex.instID++ ) {
                currentIdx.append(index(dex, Virtual));
                newIdx.append(createIndex(newM.index(dex, Virtual).row(), 0));
            }
        }
    }
    if ( displayFlags & AnalogIn ) {
        dex.isPrototype = false;
        dex.isVirtual = false;
        dex.inChn = true;
        for ( int i = 0; i < nAI; i++ ) {
            dex.index = i + offsetAnalogs*INCHN_OFFSET;
            currentIdx.append(index(dex, AnalogIn));
            newIdx.append(createIndex(newM.index(dex, AnalogIn).row(), 0));
        }
    }
    if ( displayFlags & AnalogOut ) {
        dex.isPrototype = false;
        dex.isVirtual = false;
        dex.inChn = false;
        for ( int i = 0; i < nAO; i++ ) {
            dex.index = i + offsetAnalogs*OUTCHN_OFFSET;
            currentIdx.append(index(dex, AnalogOut));
            newIdx.append(createIndex(newM.index(dex, AnalogOut).row(), 0));
        }
    }

    emit layoutAboutToBeChanged();
    updateCount(&newM);
    changePersistentIndexList(currentIdx, newIdx);
    emit layoutChanged();
}

int ChannelListModel::rowCount(const QModelIndex &) const
{
    return size;
}

QVariant ChannelListModel::data(const QModelIndex &index, int role) const
{
    ChannelIndex dex;
    if ( !index.isValid() && role == Qt::UserRole )
        return (int) CHAN_NONE;
    if (!index.isValid() || !(role==Qt::DisplayRole || role>=Qt::UserRole))
        return QVariant();

    int offset = 0, row = index.row();
    if ( displayFlags & Blank ) {
        if ( row == offset )
            switch ( role ) {
            case Qt::DisplayRole:   return QString();
            case Qt::UserRole:      return (int) CHAN_NONE;
            case Qt::UserRole + 1:  return QVariant(true);
            }
        offset++;
    }
    if ( displayFlags & None ) {
        if ( row == offset )
            switch ( role ) {
            case Qt::DisplayRole:   return QString("None");
            case Qt::UserRole:      return (int) CHAN_NONE;
            case Qt::UserRole + 1:  return QVariant(true);
            }
        offset++;
    }
    if ( displayFlags & SpikeGen ) {
        if ( row == offset ) {
            switch ( role ) {
            case Qt::DisplayRole:   return QString("Spike generator");
            case Qt::UserRole:      return (int) CHAN_SG;
            case Qt::UserRole + 1:  return QVariant(true);
            }
        }
        offset++;
    }
    if ( displayFlags & Prototype ) {
        dex.isPrototype = true;
        dex.isVirtual = true;
        if ( row-offset < nPHH ) {
            dex.modelClass = ChannelIndex::HH;
            dex.modelID = row-offset;
            switch ( role ) {
            case Qt::DisplayRole:   return QString("HH %1:all (model %1, all instances)").arg(row-offset+1);
            case Qt::UserRole:      return dex.toInt();
            case Qt::UserRole + 1:  return QVariant(HHNeuronp[row-offset].active);
            }
        }
        offset += nPHH;
    }
    if ( displayFlags & Virtual ) {
        dex.isPrototype = false;
        dex.isVirtual = true;
        for ( int i = 0; i < nVHH.size(); i++ ) {
            if ( row-offset < nVHH[i] ) {
                dex.modelClass = ChannelIndex::HH;
                dex.modelID = i;
                dex.instID = row-offset;
                switch ( role ) {
                case Qt::DisplayRole:   return QString("HH %1:%2 (model %1, instance %2)").arg(i+1).arg(row-offset+1);
                case Qt::UserRole:      return dex.toInt();
                case Qt::UserRole + 1:  return QVariant(HHNeuronp[i].inst[row-offset].active);
                }
            }
            offset += nVHH[i];
        }
    }
    if ( displayFlags & AnalogIn ) {
        if ( row-offset < nAI )
            switch ( role ) {
            case Qt::DisplayRole:   return QString("AI %1").arg(row-offset);
            case Qt::UserRole:      return row-offset + offsetAnalogs*INCHN_OFFSET;
            case Qt::UserRole + 1:  return QVariant(inChnp[row-offset].active);
            }
        offset += nAI;
    }
    if ( displayFlags & AnalogOut ) {
        if ( row-offset < nAO )
            switch ( role ) {
            case Qt::DisplayRole:   return QString("AO %1").arg(row-offset);
            case Qt::UserRole:      return row-offset + offsetAnalogs*OUTCHN_OFFSET;
            case Qt::UserRole + 1:  return QVariant(outChnp[row-offset].active);
            }
        offset += nAO;
    }

    if ( role == Qt::UserRole )
        return (int) CHAN_NONE;
    else
        return QVariant();
}

Qt::ItemFlags ChannelListModel::flags(const QModelIndex &index) const
{
    return data(index, Qt::UserRole + 1).toBool()
            ? Qt::ItemIsEnabled | Qt::ItemIsSelectable
            : Qt::NoItemFlags;
}

int ChannelListModel::index(int channel) const
{
    ChannelIndex dex(channel);
    ChannelType type = displayFlags & Blank ? Blank : None;
    if ( dex.isPrototype )
        type = Prototype;
    else if ( dex.isVirtual )
        type = Virtual;
    else if ( dex.isSG )
        type = SpikeGen;
    else if ( dex.isNone )
        type = None;
    else if ( (offsetAnalogs && channel >= OUTCHN_OFFSET) || (!offsetAnalogs && displayFlags & AnalogOut) ) {
        type = AnalogOut;
        dex.inChn = false;
    } else if ( (offsetAnalogs && channel >= INCHN_OFFSET) || (!offsetAnalogs && displayFlags & AnalogIn) ) {
        type = AnalogIn;
        dex.inChn = true;
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
            if ( type & Prototype && dex.modelClass == ChannelIndex::HH ) {
                if ( dex.modelID >= 0 && dex.modelID < nPHH )
                    return createIndex(row + dex.modelID, 0);
                else
                    return QModelIndex();
            }
            row += nPHH;
        }
        if ( displayFlags & Virtual ) {
            for ( int i = 0; i < nVHH.size(); i++ ) {
                if ( type & Virtual && dex.modelClass == ChannelIndex::HH && dex.modelID == i ) {
                    if ( dex.instID >= 0 && dex.instID < nVHH[i] )
                        return createIndex(row + dex.instID, 0);
                    else
                        return QModelIndex();
                }
                row += nVHH[i];
            }
        }
        if ( displayFlags & AnalogIn ) { // Are AIs in the list?
            if ( type & AnalogIn ) { // Is the requested index an AI?
                int i = dex.index - offsetAnalogs*INCHN_OFFSET;
                if ( i >= 0 && i < nAI ) // Is it valid?
                    return createIndex(row + i, 0); // If so, return the appropriate index
                else
                    return QModelIndex();
            }
            row += nAI; // Else, increase the offset
        }
        if ( displayFlags & AnalogOut ) {
            if ( type & AnalogOut ) {
                int i = dex.index - offsetAnalogs*OUTCHN_OFFSET;
                if ( i >= 0 && i < nAO )
                    return createIndex(row + i, 0);
                else
                    return QModelIndex();
            }
            row += nAO;
        }
    }
    return QModelIndex();
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
