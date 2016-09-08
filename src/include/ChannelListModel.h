#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QComboBox>
#include "ChannelIndex.h"

class ChannelListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ChannelListModel(int displayFlags, QObject *parent = 0);

    enum ChannelType {
        Blank =      1,
        None =       2,
        SpikeGen =   4,
        Virtual   =  8, // Model instances
        AnalogIn =   16,
        AnalogOut =  32,
        Prototype =  64, // Model prototypes, applies multiplexed over all instances

        In = AnalogIn | SpikeGen | Virtual | Prototype,
        Out = AnalogOut | Virtual | Prototype
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int index(int channel) const;
    QModelIndex index(const ChannelIndex &dex, ChannelType type) const;

    inline bool areAnalogsOffset() const { return offsetAnalogs; }

    void subordinate(QComboBox *cb);
    static void fixComboBoxWidth(QComboBox *cb);

public slots:
    void updateChns();

private:
    const int displayFlags;
    int size;
    int nAI, nAO, nPHH;
    QVector<int> nVHH;
    bool offsetAnalogs;
};

#endif // CHANNELLISTMODEL_H
