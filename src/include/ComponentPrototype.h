#ifndef COMPONENTPROTOTYPE_H
#define COMPONENTPROTOTYPE_H

#include "ChannelListModel.h"
#include "Component.h"

class ComponentPrototypeBase
{
public:
    ComponentPrototypeBase() :
        inst()
    {}

    virtual ~ComponentPrototypeBase()
    {
        for ( GenericComponent *c : inst )
            delete c;
    }

    virtual QString label() = 0;

    virtual GenericComponent *create(int idx) = 0;
    virtual void createAll() = 0;

    void clearInactive()
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

    virtual void exportData() = 0;

    QVector<GenericComponent *> inst;
};

class ProxiedComponentPrototype : public ComponentPrototypeBase
{
public:
    ProxiedComponentPrototype(ConductanceProxy *proxy) : proxy(proxy) {}
    ~ProxiedComponentPrototype() {}

    inline QString label() { return proxy->prettyName(); }

    inline GenericComponent *create(int idx) {
        ProxiedComponent *c = new ProxiedComponent(proxy, idx);
        inst.push_back(c);
        return c;
    }

    inline void createAll() {
        for ( GenericComponent *c : inst )
            delete c;
        inst.clear();
        inst.reserve(proxy->size());
        for ( size_t i = 0; i < proxy->size(); i++ ) {
            create(i)->importData();
        }
    }

    void exportData()
    {
        proxy->resize(inst.size());
        for ( GenericComponent *c : inst )
            c->exportData();
    }

private:
    ConductanceProxy *proxy;
};

template <class ComponentDlg>
class ComponentPrototype : public ComponentPrototypeBase
{
public:
    ComponentPrototype(QString const& label, std::vector<typename ComponentDlg::param_type> *params) :
        _label(label),
        params(params)
    {}
    ~ComponentPrototype() {}

    QString label() { return _label; }

    GenericComponent *create(int idx)
    {
        Component<ComponentDlg> *c = new Component<ComponentDlg>(_label, params, idx);
        inst.push_back(c);
        return c;
    }

    void createAll()
    {
        for ( GenericComponent *c : inst )
            delete c;
        inst.clear();
        inst.reserve(params->size());
        for ( size_t i = 0; i < params->size(); i++ ) {
            create(i)->importData();
        }
    }

    void exportData()
    {
        params->resize(inst.size());
        for ( GenericComponent *c : inst )
            c->exportData();
    }

private:
    QString _label;
    std::vector<typename ComponentDlg::param_type> *params;
};

#endif // COMPONENTPROTOTYPE_H
