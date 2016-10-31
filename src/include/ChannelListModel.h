#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QComboBox>
#include "ChannelIndex.h"
#include "ObjectDataTypes.h"
#include "ModelManager.h"

class ChannelListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ChannelListModel(int displayFlags, QObject *parent = 0);

    enum ChannelType {
        Blank =      1,
        None =       2,
        Virtual   =  8, // Model instances
        AnalogIn =   16,
        AnalogOut =  32,
        Prototype =  64, // Model prototypes, applies multiplexed over all instances

        In = AnalogIn | Virtual | Prototype,
        Out = AnalogOut | Virtual | Prototype
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int index(const ChannelIndex &dex) const;
    QModelIndex index(const ChannelIndex &dex, ChannelType type) const;

public slots:
    void updateChns(ChannelIndex removeDeviceDex = ChannelIndex());

protected:
    void updateCount(ChannelListModel *from = nullptr);

    template <typename T>
    class DAQHelper {
    private:
        ChannelListModel *const parent;
    public:
        DAQHelper(ChannelListModel *parent) : parent(parent), nAI(0), nAO(0) {}
        void updateCount();
        void updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM);
        bool data(int row, int role, int &offset, QVariant &ret) const;
        bool index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const;
        QVector<int> nAI, nAO;
    };

    class ModelHelper {
    private:
        ChannelListModel *const parent;
        ModelProxy *proxy;
    public:
        ModelHelper(ModelProxy *proxy, ChannelListModel *parent) : parent(parent), proxy(proxy), nInst(0) {}
        void updateCount();
        void updateChns(QModelIndexList &currentIdx, QModelIndexList &newIdx, ChannelListModel &newM);
        bool data(int row, int role, int &offset, QVariant &ret) const;
        bool index(const ChannelIndex &dex, ChannelType type, int &offset, QModelIndex &ret) const;
        std::vector<size_t> nInst;
    };

    const int displayFlags;
    int size;
    DAQHelper<SDAQData> hSimul;
    DAQHelper<DigiDataData> hDD1200;
#ifdef NATIONAL_INSTRUMENTS
    DAQHelper<NIDAQData> hNI;
#endif
    std::vector<ModelHelper> modelHelpers;

    ChannelIndex rmDevDex;
};

#endif // CHANNELLISTMODEL_H
