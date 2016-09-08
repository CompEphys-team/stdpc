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

void ChannelListModel::updateChns()
{
    int count=0, nAI=0, nAO=0, nPHH=0;
    QVector<int> nVHH;
    ChannelType reset = None;

    // Reverse order (set reset to the highest changing list entry type) to allow switch/no-break below
    if ( displayFlags & AnalogOut ) {
        if ( (int)outChnp.size() != this->nAO )
            reset = AnalogOut;
        nAO = outChnp.size();
    }
    if ( displayFlags & AnalogIn ) {
        if ( (int)inChnp.size() != this->nAI )
            reset = AnalogIn;
        nAI = inChnp.size();
    }
    if ( displayFlags & Virtual ) {
        if ( (int) HHNeuronp.size() != this->nVHH.size() ) {
            reset = Virtual;
            nVHH.clear();
            for ( HHNeuronData &model : HHNeuronp ) {
                int s = model.inst.size();
                nVHH.push_back(s);
                count += s;
            }
        } else {
            nVHH = this->nVHH;
            for ( int i = 0; i < nVHH.size(); i++ ) {
                int s = HHNeuronp[i].inst.size();
                if ( s != nVHH[i] ) {
                    reset = Virtual;
                    nVHH[i] = s;
                }
                count += nVHH[i];
            }
        }
    }
    if ( displayFlags & Prototype ) {
        if ( (int) HHNeuronp.size() != this->nPHH )
            reset = Prototype;
        nPHH = HHNeuronp.size();
    }
    if ( displayFlags & SpikeGen )   count++;
    if ( displayFlags & None )       count++;
    if ( displayFlags & Blank )      count++;

    if ( reset > None ) {
        int i, j;
        QModelIndexList from, to;
        ChannelIndex dex;
        emit layoutAboutToBeChanged();

        // Collect all rows that are liable to change, starting with the first that does change
        switch ( reset ) {
        case Prototype:
            dex.isPrototype = true;
            dex.isVirtual = true;
            dex.modelClass = ChannelIndex::HH;
            for ( i = 0; i < this->nPHH; i++ ) {
                dex.modelID = i;
                from.append(index(dex, Prototype));
            }
        case Virtual:
            dex.isPrototype = false;
            dex.isVirtual = true;
            dex.modelClass = ChannelIndex::HH;
            for ( i = 0; i < this->nVHH.size(); i++ ) {
                dex.modelID = i;
                for ( j = 0; j < this->nVHH[i]; j++ ) {
                    dex.instID = j;
                    from.append(index(dex, Virtual));
                }
            }
        case AnalogIn:
            dex.isPrototype = false;
            dex.isVirtual = false;
            dex.inChn = true;
            for ( i = 0; i < this->nAI; i++ ) {
                dex.index = i + offsetAnalogs*INCHN_OFFSET;
                from.append(index(dex, AnalogIn));
            }
        case AnalogOut:
            dex.isPrototype = false;
            dex.isVirtual = false;
            dex.inChn = false;
            for ( i = 0; i < this->nAO; i++ ) {
                dex.index = i + offsetAnalogs*OUTCHN_OFFSET;
                from.append(index(dex, AnalogOut));
            }
        default: break;
        }

        // Update internals
        size = count + nAI + nAO + nPHH;
        swap(this->nAI, nAI);
        swap(this->nAO, nAO);
        swap(this->nPHH, nPHH);
        swap(this->nVHH, nVHH);

        // Create replacement indices
        switch ( reset ) {
        case Prototype:
            dex.isPrototype = true;
            dex.isVirtual = true;
            dex.modelClass = ChannelIndex::HH;
            for ( i = 0; i < min(nPHH, this->nPHH); i++ ) {
                dex.modelID = i;
                to.append(index(dex, Prototype));
            }
            for ( ; i < nPHH; i++ ) {
                to.append(QModelIndex());
            }
        case Virtual:
            dex.isPrototype = false;
            dex.isVirtual = true;
            dex.modelClass = ChannelIndex::HH;
            for ( i = 0; i < min(nVHH.size(), this->nVHH.size()); i++ ) {
                dex.modelID = i;
                for ( j = 0; j < min(nVHH[i], this->nVHH[i]); j++ ) {
                    dex.instID = j;
                    to.append(index(dex, Virtual));
                }
                for ( ; j < nVHH[i]; j++ ) {
                    dex.instID = j;
                    to.append(QModelIndex());
                }
            }
            for ( ; i < nVHH.size(); i++ ) {
                dex.modelID = i;
                for ( j = 0; j < nVHH[i]; j++ ) {
                    dex.instID = j;
                    to.append(QModelIndex());
                }
            }
        case AnalogIn:
            dex.isPrototype = false;
            dex.isVirtual = false;
            dex.inChn = true;
            // For existing rows that continue to exist, use new placement
            for ( i = 0; i < min(nAI, this->nAI); i++ ) {
                dex.index = i + offsetAnalogs*INCHN_OFFSET;
                to.append(index(dex, AnalogIn));
            }
            // For existing rows that are dropped, use an invalid index
            for ( ; i < nAI; i++ ) {
                to.append(QModelIndex());
            }
            // No action for new rows
            // No break to allow items farther down the list to adjust, too
        case AnalogOut:
            dex.isPrototype = false;
            dex.isVirtual = false;
            dex.inChn = false;
            for ( i = 0; i < min(nAO, this->nAO); i++ ) {
                dex.index = i + offsetAnalogs*OUTCHN_OFFSET;
                to.append(index(dex, AnalogOut));
            }
            for ( ; i < nAO; i++ ) {
                to.append(QModelIndex());
            }
        default: break;
        }

        changePersistentIndexList(from, to);
        emit layoutChanged();
    }
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
                return createIndex(row + dex.modelID, 0);
            }
            row += nPHH;
        }
        if ( displayFlags & Virtual ) {
            for ( int i = 0; i < nVHH.size(); i++ ) {
                if ( type & Virtual && dex.modelClass == ChannelIndex::HH && dex.modelID == i )
                    return createIndex(row + dex.instID, 0);
                row += nVHH[i];
            }
        }
        if ( displayFlags & AnalogIn ) { // Are AIs in the list?
            if ( type & AnalogIn ) { // Is the requested index an AI?
                return createIndex(row + dex.index - offsetAnalogs*INCHN_OFFSET, 0); // If so, return the appropriate index
            }
            row += nAI; // Else, increase the offset
        }
        if ( displayFlags & AnalogOut ) {
            if ( type & AnalogOut ) {
                return createIndex(row + dex.index - offsetAnalogs*OUTCHN_OFFSET, 0);
            }
            row += nAO;
        }
    }
    return QModelIndex();
}

void ChannelListModel::subordinate(QComboBox *cb)
{
    cb->setModel(this);
    connect(this, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(cb);});
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
