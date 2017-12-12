#include "ModelOpts.h"
#include "MainWin.h"

ModelOpts::ModelOpts(QWidget *parent, ModelProxy *proxy, size_t idx) :
    DaqOptsBase(parent, idx),
    proxy(proxy)
{
    if ( idx >= proxy->size() ) {
        proxy->resize(idx+1);
        proxy->param(idx).active = false;
    }

    dlg = proxy->createDialog(idx, parent);

    regenerateWidget();
    setIndex(idx);

    connect(dlg, SIGNAL(accepted()), this, SLOT(setLabel()));
    connect(dlg, SIGNAL(channelsChanged()), parent, SIGNAL(channelsChanged()));
    connect(dlg, SIGNAL(modelStatusChanged()), parent, SLOT(updateStartButton()));
    connect(this, SIGNAL(removedModel(ChannelIndex)), parent, SIGNAL(modelRemoved(ChannelIndex)));
    static_cast<MyMainWindow*>(parent)->channelsChanged();
}

ModelOpts::~ModelOpts()
{
    delete dlg;
}

void ModelOpts::removeDevice()
{
    proxy->remove(idx);
    emit removedModel(ChannelIndex(proxy, idx));
}

void ModelOpts::importData()
{
    dlg->importData();
    _widget->active->setChecked(proxy->param(idx).active);
}

void ModelOpts::exportData()
{
    if ( idx >= proxy->size() )
        proxy->resize(idx+1);
    proxy->param(idx).active = _widget->active->isChecked();
    dlg->exportData();
}

void ModelOpts::setIndex(size_t i)
{
    idx = i;
    setLabel();
    dlg->setIndex(idx);
}

void ModelOpts::setLabel()
{
    QString label = proxy->param(idx).label;
    if ( label.isEmpty() )
        label = QString("%1 %2").arg(proxy->prettyName()).arg(idx);
    _widget->setLabel(label);
}

void ModelOpts::regenerateWidget()
{
    _widget = new DaqWidget();
    connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(open()));
    connect(_widget->active, SIGNAL(clicked(bool)), this, SLOT(activeChanged()));
}

void ModelOpts::activeChanged()
{
    proxy->param(idx).active = _widget->active->isChecked();
    dlg->exportData();
}



DaqOptsBase *ModelOptsPrototype::create(size_t idx, QWidget *parent)
{
    DaqOptsBase *c = new ModelOpts(parent, proxy, idx);
    inst.push_back(c);
    return c;
}

void ModelOptsPrototype::createAll(QWidget *parent)
{
    for ( DaqOptsBase *c : inst )
        delete c;
    inst.clear();
    inst.reserve(proxy->size());
    for ( size_t i = 0; i < proxy->size(); i++ ) {
        create(i, parent)->importData();
    }
}

void ModelOptsPrototype::exportData(bool)
{
    proxy->resize(inst.size());
    for ( DaqOptsBase *c : inst )
        c->exportData();
}
