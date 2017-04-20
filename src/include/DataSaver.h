#ifndef DATASAVER_H
#define DATASAVER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <fstream>
#include <memory>
#include "CircularFifo.h"

class DataSaver : public QObject
{
    Q_OBJECT

private:
    QString filename;
    bool isBinary;
    std::ofstream os;

public:
    DataSaver();
    bool InitDataSaving(QString, bool);
    void SaveHeader(QVector<QString>, double savingFreq);

    std::vector<std::unique_ptr<CircularFifo<float>>> q;

public slots:
    void SaveLine();
    void EndDataSaving();
};

#endif // DATASAVER_H
