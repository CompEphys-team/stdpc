#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QComboBox>

class ChannelListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ChannelListModel(int displayFlags, QObject *parent = 0);

    enum ChannelType {
        Blank =      1,
        None =       2,
        SpikeGen =   4,
        Virtual   =  8,
        AnalogIn =   16,
        AnalogOut =  32,

        In = AnalogIn | SpikeGen | Virtual,
        Out = AnalogOut | Virtual
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int index(int channel) const;
    QModelIndex index(const QPoint &p) const;

    static void fixComboBoxWidth(QComboBox *cb);

public slots:
    void updateChns();

private:
    const int displayFlags;
    int size;
    int nAI, nAO, nV;
};

#endif // CHANNELLISTMODEL_H
