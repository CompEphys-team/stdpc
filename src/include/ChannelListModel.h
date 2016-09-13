#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QComboBox>
#include "ChannelIndex.h"
#include "ObjectDataTypes.h"

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

    int index(const ChannelIndex &dex) const;
    QModelIndex index(const ChannelIndex &dex, ChannelType type) const;

    void subordinate(QComboBox *cb);
    static void fixComboBoxWidth(QComboBox *cb);

public slots:
    void updateChns();

protected:
    void updateCount(ChannelListModel *from = nullptr);

    class DAQHelper {
    private:
        const DAQClass daqClass;
        DAQData *const p;
        ChannelListModel *const parent;
    public:
        DAQHelper(DAQClass c, DAQData *p, ChannelListModel *parent) : daqClass(c), p(p), parent(parent), nAI(0), nAO(0) {}
        void updateCount();
        void updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM);
        bool data(int row, int role, int &offset, QVariant &ret) const;
        bool index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const;
        int nAI, nAO;
    };

    const int displayFlags;
    int size;
    DAQHelper hSimul, hDD1200, hNI;
    int nPHH;
    QVector<int> nVHH;
};

#endif // CHANNELLISTMODEL_H
