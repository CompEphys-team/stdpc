#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentWidget.h"
#include "ChannelListModel.h"

class Component
{
public:
    Component(ConductanceProxy *proxy, size_t idx);

    void importData();
    void exportData();

    void setIndex(int i);

    inline ComponentWidget *widget() { return _widget; }
    void regenerateWidget();

protected:
    ConductanceProxy *proxy;
    size_t idx;
    ComponentWidget *_widget;
    ConductanceDlg *dlg;
};

class ComponentPrototype
{
public:
    ComponentPrototype(ConductanceProxy *proxy);
    ~ComponentPrototype();

    inline QString label() { return proxy->prettyName(); }

    Component *create(int idx);
    void createAll();
    void clearInactive();
    void exportData();

    QVector<Component *> inst;

protected:
    ConductanceProxy *proxy;
};

#endif // COMPONENT_H
