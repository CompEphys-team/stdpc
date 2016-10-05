#ifndef COMPONENTTABLE_H
#define COMPONENTTABLE_H

#include <QTableWidget>
#include "ComponentPrototype.h"

#include "ChemSynDlg.h"
#include "AbSynDlg.h"
#include "GapJunctionDlg.h"
#include "DestexheSynDlg.h"
#include "AlphaBetaHHDlg.h"
#include "HHDlg.h"


class ComponentTable : public QTableWidget
{
    Q_OBJECT

public:
    ComponentTable(QWidget *parent = 0);
    ~ComponentTable();

    void init(QVector<ComponentPrototypeBase *> prototypes, ChannelListModel *in, ChannelListModel *out);

public slots:
    void importData(bool activeOnly = false);
    void exportData();

    void activateAll();
    void deactivateAll();

private slots:
    void addComponent();

private:
    QVector<ComponentPrototypeBase *> proto;
    QVector<GenericComponent *> comp;
    QVector<int> idx;
    ComponentFactoryWidget *factory;
    ChannelListModel *in, *out;

    void makeFactory();
};

#endif // COMPONENTTABLE_H
