#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentWidget.h"
#include "ChannelListModel.h"

class GenericComponent
{
public:
    GenericComponent() {}
    virtual ~GenericComponent() {}

    virtual void importData() = 0;
    virtual void exportData() = 0;

    virtual void setIndex(int i) = 0;

    virtual ComponentWidget *widget() = 0;
    virtual void regenerateWidget() = 0;
};

class ProxiedComponent : public GenericComponent
{
public:
    ProxiedComponent(ConductanceProxy *proxy, size_t idx) :
        proxy(proxy), idx(idx), dlg(proxy->createDialog(idx))
    {
        regenerateWidget();
        _widget->label->setText(proxy->prettyName() + " " + QString::number(idx));
    }
    ~ProxiedComponent() { delete dlg; }

    void importData()
    {
        dlg->importData();
        _widget->active->setChecked(proxy->param(idx).active);
    }

    void exportData()
    {
        if ( idx >= proxy->size() )
            proxy->resize(idx+1);
        proxy->param(idx).active = _widget->active->isChecked();
        dlg->exportData();
    }

    void setIndex(int i)
    {
        idx = i;
        _widget->label->setText(proxy->prettyName() + " " + QString::number(idx));
        dlg->setIndex(idx);
    }

    inline ComponentWidget *widget() { return _widget; }

    void regenerateWidget()
    {
        _widget = new ComponentWidget();
        QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(show()));
        QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(raise()));
    }

private:
    ConductanceProxy *proxy;
    size_t idx;
    ComponentWidget *_widget;
    ConductanceDlg *dlg;
};

template <class ComponentDlg>
class Component : public GenericComponent
{
public:
    Component(QString label, std::vector<typename ComponentDlg::param_type> *params,
              int idx) :
        params(params),
        dlg(new ComponentDlg(idx)),
        idx(idx),
        label(label)
    {
        regenerateWidget();
        _widget->label->setText(label + " " + QString::number(idx));
    }

    ~Component()
    {
        delete dlg;
    }

    void importData()
    {
        dlg->importData(params->at(idx));
        _widget->active->setChecked(params->at(idx).active);
    }

    void exportData()
    {
        if ( idx >= (int) params->size() )
            params->resize(idx+1);
        dlg->exportData(params->at(idx));
        (*params)[idx].active = _widget->active->isChecked();
    }

    void setIndex(int i)
    {
        idx = i;
        _widget->label->setText(label + " " + QString::number(idx));
        dlg->setIndex(idx);
    }

    inline ComponentWidget *widget() { return _widget; }

    void regenerateWidget()
    {
        _widget = new ComponentWidget();
        QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(show()));
        QObject::connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(raise()));
    }

protected:
    std::vector<typename ComponentDlg::param_type> *params;
    ComponentDlg *dlg;
    int idx;
    QString label;
    ComponentWidget *_widget;
};

#endif // COMPONENT_H
