#include "ChannelListModel.h"
#include "Global.h"

ChannelListModel::ChannelListModel(int displayFlags, QObject *parent)
    : QAbstractListModel(parent),
      displayFlags(displayFlags),
      size(0), nAI(0), nAO(0), nV(0)
{
}

void ChannelListModel::updateChns()
{
    int count=0, nAI=0, nAO=0;
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
        // TODO
    }
    if ( displayFlags & SpikeGen )   count++;
    if ( displayFlags & None )       count++;
    if ( displayFlags & Blank )      count++;

    if ( reset > None ) {
        int i;
        QModelIndexList from, to;
        emit layoutAboutToBeChanged();

        // Collect all rows that are liable to change, starting with the first that does change
        switch ( reset ) {
        case Virtual:
            // TODO
        case AnalogIn:
            for ( i = 0; i < this->nAI; i++ ) {
                from.append(index(QPoint(i, AnalogIn)));
            }
        case AnalogOut:
            for ( i = 0; i < this->nAO; i++ ) {
                from.append(index(QPoint(i, AnalogOut)));
            }
        default: break;
        }

        // Update internals
        size = count + nAI + nAO;
        swap(this->nAI, nAI);
        swap(this->nAO, nAO);

        // Create replacement indices
        switch ( reset ) {
        case Virtual:
            // TODO
        case AnalogIn:
            // For existing rows that continue to exist, use new placement
            for ( i = 0; i < min(nAI, this->nAI); i++ ) {
                to.append(index(QPoint(i, AnalogIn)));
            }
            // For existing rows that are dropped, use an invalid index
            for ( ; i < nAI; i++ ) {
                to.append(QModelIndex());
            }
            // No action for new rows
            // No break to allow items farther down the list to adjust, too
        case AnalogOut:
            for ( i = 0; i < min(nAO, this->nAO); i++ ) {
                to.append(index(QPoint(i, AnalogOut)));
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
    if (!index.isValid() || !(role==Qt::DisplayRole || role>=Qt::UserRole))
        return QVariant();

    int offset = 0, row = index.row();
    if ( displayFlags & Blank ) {
        if ( row == offset )
            switch ( role ) {
            case Qt::DisplayRole:   return QString();
            case Qt::UserRole:      return QPoint(CHAN_NONE, Blank);
            case Qt::UserRole + 1:  return QVariant(true);
            }
        offset++;
    }
    if ( displayFlags & None ) {
        if ( row == offset )
            switch ( role ) {
            case Qt::DisplayRole:   return QString("None");
            case Qt::UserRole:      return QPoint(CHAN_NONE, None);
            case Qt::UserRole + 1:  return QVariant(true);
            }
        offset++;
    }
    if ( displayFlags & SpikeGen ) {
        if ( row == offset ) {
            switch ( role ) {
            case Qt::DisplayRole:   return QString("SG");
            case Qt::UserRole:      return QPoint(CHAN_SG, SpikeGen);
            case Qt::UserRole + 1:  return QVariant(true);
            }
        }
        offset++;
    }
    if ( displayFlags & Virtual ) {
        // TODO
        offset += nV;
    }
    if ( displayFlags & AnalogIn ) {
        if ( row-offset < nAI )
            switch ( role ) {
            case Qt::DisplayRole:   return QString("AI %1").arg(row-offset);
            case Qt::UserRole:      return QPoint(row-offset, AnalogIn);
            case Qt::UserRole + 1:  return QVariant(inChnp[row-offset].active);
            }
        offset += nAI;
    }
    if ( displayFlags & AnalogOut ) {
        if ( row-offset < nAO )
            switch ( role ) {
            case Qt::DisplayRole:   return QString("AO %1").arg(row-offset);
            case Qt::UserRole:      return QPoint(row-offset, AnalogOut);
            case Qt::UserRole + 1:  return QVariant(outChnp[row-offset].active);
            }
        offset += nAO;
    }

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
    return index(QPoint(channel, displayFlags)).row();
}

QModelIndex ChannelListModel::index(const QPoint &p) const
{
    int chan = p.x(), type = p.y(), row = 0;

    // Resolve aggregate types
    if ( type == In ) {
        if ( chan >= CHAN_VIRTUAL ) {
            type = Virtual;
            chan -= CHAN_VIRTUAL;
        } else if ( chan == CHAN_SG ) {
            type = SpikeGen;
        } else {
            type = AnalogIn;
        }
    } else if ( type == Out ) {
        if ( chan >= CHAN_VIRTUAL ) {
            type = Virtual;
            chan -= CHAN_VIRTUAL;
        } else {
            type = AnalogOut;
        }
    }

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
        if ( displayFlags & Virtual ) {
            // TODO
            row += nV;
        }
        if ( displayFlags & AnalogIn ) {
            if ( type & AnalogIn ) {
                return createIndex(row + chan, 0);
            }
            row += nAI;
        }
        if ( displayFlags & AnalogOut ) {
            if ( type & AnalogOut ) {
                return createIndex(row + chan, 0);
            }
            row += nAO;
        }
    }
    return QModelIndex();
}
