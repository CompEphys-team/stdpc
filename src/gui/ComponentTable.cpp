#include "ComponentTable.h"

ComponentTable::ComponentTable(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(1);
}

ComponentTable::~ComponentTable()
{
    for ( ComponentPrototype *p : proto )
        delete p;
}

void ComponentTable::makeFactory()
{
    factory = new ComponentFactoryWidget(this);
    connect(factory->button, SIGNAL(clicked(bool)), this, SLOT(addComponent()));
    for ( ComponentPrototype *p : proto )
        factory->combo->addItem(p->label());
}

void ComponentTable::init(QVector<ComponentPrototype *> prototypes)
{
    proto = prototypes;
    idx = QVector<int>(proto.size(), 0);
    makeFactory();
    setCellWidget(0, 0, factory);
}

void ComponentTable::importData(bool activeOnly)
{
    clear();
    comp.clear();
    int i = 0;
    for ( ComponentPrototype *p : proto ) {
        if ( activeOnly )
            p->clearInactive();
        else
            p->createAll();
        comp += p->inst;
        idx[i++] = p->inst.size();
    }
    i = 0;
    setColumnCount(comp.size() + 1);
    for ( Component *c : comp ) {
        setCellWidget(0, i++, c->widget());
    }

    makeFactory();
    setCellWidget(0, i, factory);
}

void ComponentTable::exportData()
{
    for ( ComponentPrototype *p : proto )
        p->exportData();
}

void ComponentTable::activateAll()
{
    for ( Component *c : comp )
        c->widget()->active->setChecked(true);
}

void ComponentTable::deactivateAll()
{
    for ( Component *c : comp )
        c->widget()->active->setChecked(false);
}

void ComponentTable::addComponent()
{
    int i = factory->combo->currentIndex();
    Component *c = proto[i]->create(idx[i]++);
    insertColumn(comp.size());
    setCellWidget(0, comp.size(), c->widget());
    comp.append(c);
    scrollTo(model()->index(0, comp.size()));
}
