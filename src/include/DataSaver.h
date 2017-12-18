#ifndef DATASAVER_H
#define DATASAVER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDataStream>
#include <QFile>
#include <fstream>
#include <memory>
#include "CircularFifo.h"
#include "ObjectDataTypes.h"

class DataSaver : public QObject
{
    Q_OBJECT

private:
    dataSavingParams p;

    std::vector<std::unique_ptr<QDataStream>> binaryStreams;
    std::vector<std::unique_ptr<QFile>> binaryFiles;
    std::ofstream os;

    std::string lineEnding;

protected:
    bool initBinary(QVector<ChannelIndex> channels);
    bool initAscii(QVector<ChannelIndex> channels);

public:
    DataSaver();
    bool init(dataSavingParams p_, QVector<ChannelIndex> channels);

    std::vector<std::unique_ptr<CircularFifo<double>>> q;

public slots:
    void SaveLine();
    void EndDataSaving();
};

#endif // DATASAVER_H
