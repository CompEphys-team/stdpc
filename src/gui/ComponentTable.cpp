#include "ComponentTable.h"

template class Component<ChemSynDlg>;
template class Component<abSynDlg>;
template class Component<GapJunctionDlg>;
template class Component<DestexheSynDlg>;
template class Component<AlphaBetaHHDlg>;
template class Component<HHDlg>;

template <class Dlg>
GenericComponent *Component<Dlg>::create(int idx, ChannelListModel *in, ChannelListModel *out)
{
    Component<Dlg> *c = new Component<Dlg>(_label, params);
    c->dlg = new Dlg(idx, in, out);
    c->idx = idx;
    c->_widget = new ComponentWidget();
    c->_widget->label->setText(_label + " " + QString::number(idx));
    QObject::connect(c->_widget->params, SIGNAL(clicked(bool)), c->dlg, SLOT(show()));
    return c;
}

template <class Dlg>
QVector<GenericComponent *> Component<Dlg>::createAll(ChannelListModel *in, ChannelListModel *out)
{
    QVector<GenericComponent *> vec;
    vec.reserve(params->size());
    GenericComponent *c;
    for ( size_t i = 0; i < params->size(); i++ ) {
        c = create(i, in, out);
        c->importData();
        vec.push_back(c);
    }
    return vec;
}

template <class Dlg>
void Component<Dlg>::importData()
{
    dlg->importData(params->at(idx));
    _widget->active->setChecked(params->at(idx).active);
}

template <class Dlg>
void Component<Dlg>::exportData()
{
    if ( idx >= (int) params->size() )
        params->resize(idx+1);
    dlg->exportData(params->at(idx));
    (*params)[idx].active = _widget->active->isChecked();
}



ComponentTable::ComponentTable(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(1);
}

void ComponentTable::makeFactory()
{
    factory = new ComponentFactoryWidget(this);
    connect(factory->button, SIGNAL(clicked(bool)), this, SLOT(addComponent()));
    for ( GenericComponent *c : proto )
        factory->combo->addItem(c->label());
}

void ComponentTable::init(QVector<GenericComponent *> prototypes, ChannelListModel *in, ChannelListModel *out)
{
    proto = prototypes;
    idx = QVector<int>(proto.size(), 0);
    this->in = in;
    this->out = out;
    makeFactory();
    setCellWidget(0, 0, factory);
}

void ComponentTable::importData()
{
    clear();
    for ( GenericComponent *c : comp )
        delete c;
    comp.clear();

    int i = 0;
    for ( GenericComponent *p : proto ) {
        QVector<GenericComponent*> comps = p->createAll(in, out);
        comp.append(comps);
        idx[i++] = comps.size();
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
    for ( GenericComponent *c : comp )
        c->exportData();
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
