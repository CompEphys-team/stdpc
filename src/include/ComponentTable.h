#ifndef COMPONENTTABLE_H
#define COMPONENTTABLE_H

#include <QTableWidget>
#include "Component.h"

class ComponentTable : public QTableWidget
{
    Q_OBJECT

public:
    ComponentTable(QWidget *parent = 0);
    ~ComponentTable();

    void init(QVector<ComponentPrototype *> prototypes);

public slots:
    void importData(bool activeOnly = false);
    void exportData();

    void activateAll();
    void deactivateAll();

private slots:
    void addComponent();

private:
    QVector<ComponentPrototype *> proto;
    QVector<Component *> comp;
    QVector<int> idx;
    ComponentFactoryWidget *factory;

    void makeFactory();
};

#endif // COMPONENTTABLE_H
