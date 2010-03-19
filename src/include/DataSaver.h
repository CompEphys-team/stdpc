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
    ofstream *os;

public:
    DataSaver();
    void InitDataSaving(QString);
    void SaveLine(QVector<double>);
    void EndDataSaving();
};

#endif // DATASAVER_H
