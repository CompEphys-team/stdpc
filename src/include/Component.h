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

template <class ComponentDlg>
class Component : public GenericComponent
{
public:
    Component(QString label, std::vector<typename ComponentDlg::param_type> *params,
              int idx, ChannelListModel *in, ChannelListModel *out) :
        params(params),
        dlg(new ComponentDlg(idx, in, out)),
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
