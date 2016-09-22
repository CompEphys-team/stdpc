#ifndef DATASAVER_H
#define DATASAVER_H

#include <QString>
#include <QVector>
#include <fstream>

using namespace std;

class DataSaver
{
private:
    QString filename;
    bool isBinary;
    ofstream os;

public:
    DataSaver();
    bool InitDataSaving(QString, bool);
    void SaveLine(QVector<double>);
    void SaveHeader(QVector<QString>);
    void EndDataSaving();
};

#endif // DATASAVER_H
