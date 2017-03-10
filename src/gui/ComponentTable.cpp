#include "ComponentTable.h"

ComponentTable::ComponentTable(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(1);
}

ComponentTable::~ComponentTable()
{
    for ( ComponentPrototypeBase *p : proto )
        delete p;
}

void ComponentTable::makeFactory()
{
    factory = new ComponentFactoryWidget(this);
    connect(factory->button, SIGNAL(clicked(bool)), this, SLOT(addComponent()));
    for ( ComponentPrototypeBase *p : proto )
        factory->combo->addItem(p->label);
}

void ComponentTable::init(QVector<ComponentPrototypeBase *> prototypes, ChannelListModel *in, ChannelListModel *out)
{
    proto = prototypes;
    idx = QVector<int>(proto.size(), 0);
    this->in = in;
    this->out = out;
    makeFactory();
    setCellWidget(0, 0, factory);
}

void ComponentTable::importData(bool activeOnly)
{
    clear();
    comp.clear();
    int i = 0;
    for ( ComponentPrototypeBase *p : proto ) {
        if ( activeOnly )
            p->clearInactive();
        else
            p->createAll(in, out);
        comp += p->inst;
        idx[i++] = p->inst.size();
    }
    i = 0;
    setColumnCount(comp.size() + 1);
    for ( GenericComponent *c : comp ) {
        setCellWidget(0, i++, c->widget());
    }

    makeFactory();
    setCellWidget(0, i, factory);
}

void ComponentTable::exportData()
{
    for ( ComponentPrototypeBase *p : proto )
        p->exportData();
}

void ComponentTable::activateAll()
{
    for ( GenericComponent *c : comp )
        c->widget()->active->setChecked(true);
}

void ComponentTable::deactivateAll()
{
    for ( GenericComponent *c : comp )
        c->widget()->active->setChecked(false);
}

void ComponentTable::addComponent()
{
    int i = factory->combo->currentIndex();
    GenericComponent *c = proto[i]->create(idx[i]++, in, out);
    insertColumn(comp.size());
    setCellWidget(0, comp.size(), c->widget());
    comp.append(c);
    scrollTo(model()->index(0, comp.size()));
}
