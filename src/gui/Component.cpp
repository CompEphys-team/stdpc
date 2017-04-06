#include "Component.h"


Component::Component(ConductanceProxy *proxy, size_t idx) :
    proxy(proxy), idx(idx), dlg(proxy->createDialog(idx))
{
    regenerateWidget();
    _widget->label->setText(proxy->prettyName() + " " + QString::number(idx));
}

void Component::importData()
{
    dlg->importData();
    _widget->active->setChecked(proxy->param(idx).active);
}

void Component::exportData()
{
    if ( idx >= proxy->size() )
        proxy->resize(idx+1);
    proxy->param(idx).active = _widget->active->isChecked();
    dlg->exportData();
}

void Component::setIndex(int i)
{
    idx = i;
    _widget->label->setText(proxy->prettyName() + " " + QString::number(idx));
    dlg->setIndex(idx);
}

void Component::regenerateWidget()
{
    _widget = new ComponentWidget();
    QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(show()));
    QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(raise()));
}



ComponentPrototype::ComponentPrototype(ConductanceProxy *proxy) :
    inst(),
    proxy(proxy)
{}

ComponentPrototype::~ComponentPrototype()
{
    for ( Component *c : inst )
        delete c;
}



Component *ComponentPrototype::create(int idx) {
    Component *c = new Component(proxy, idx);
    inst.push_back(c);
    return c;
}

void ComponentPrototype::createAll() {
    for ( Component *c : inst )
        delete c;
    inst.clear();
    inst.reserve(proxy->size());
    for ( size_t i = 0; i < proxy->size(); i++ ) {
        create(i)->importData();
    }
}

void ComponentPrototype::clearInactive()
{
    for ( int i = 0; i < inst.size(); i++ ) {
        if ( inst[i]->widget()->active->isChecked() ) {
            inst[i]->regenerateWidget();
            inst[i]->setIndex(i);
        } else {
            delete inst[i];
            inst.remove(i);
            --i;
        }
    }
}

void ComponentPrototype::exportData()
{
    proxy->resize(inst.size());
    for ( Component *c : inst )
        c->exportData();
}