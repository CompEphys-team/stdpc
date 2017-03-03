#ifndef DAQTABLE_H
#define DAQTABLE_H

#include <QTableWidget>
#include "DaqOptsBase.h"

class DaqTable : public QTableWidget
{
    Q_OBJECT

public:
    DaqTable(QWidget *parent = 0);
    ~DaqTable();

    void init(QVector<DaqOptsPrototypeBase *> prototypes, QWidget *parent);

public slots:
    void importData(bool activeOnly = false);
    void exportData(bool ignoreDAQ = false);

private slots:
    void addDaqOpts();

private:
    QVector<DaqOptsPrototypeBase *> proto;
    QVector<DaqOptsBase *> comp;
    QVector<int> idx;
    DaqFactoryWidget *factory;
    QWidget *parent;

    void makeFactory();
};

#endif // DAQTABLE_H
