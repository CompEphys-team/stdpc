#ifndef COMPONENTTABLE_H
#define COMPONENTTABLE_H

#include <QTableWidget>
#include "ComponentWidget.h"
#include "ChannelListModel.h"

#include "ChemSynDlg.h"
#include "AbSynDlg.h"
#include "GapJunctionDlg.h"
#include "DestexheSynDlg.h"
#include "AlphaBetaHHDlg.h"
#include "HHDlg.h"

class GenericComponent
{
public:
    GenericComponent() {}
    virtual ~GenericComponent() {}

    virtual GenericComponent *create(int idx, ChannelListModel *in, ChannelListModel *out) = 0;
    virtual QVector<GenericComponent *> createAll(ChannelListModel *in, ChannelListModel *out) = 0;
    virtual void importData() = 0;
    virtual void exportData() = 0;

    virtual QString const& label() = 0;
    virtual ComponentWidget *widget() = 0;
};

template <class ComponentDlg>
class Component : public GenericComponent
{
public:
    Component(QString const& label, std::vector<typename ComponentDlg::param_type> *params) :
        _label(label), params(params), dlg(nullptr), idx(0), _widget(nullptr) {}
    ~Component() { delete dlg; }

    GenericComponent *create(int idx, ChannelListModel *in, ChannelListModel *out);
    QVector<GenericComponent *> createAll(ChannelListModel *in, ChannelListModel *out);

    void importData();
    void exportData();

    inline ComponentWidget *widget() { return _widget; }
    inline QString const& label() { return _label; }

protected:
    QString _label;
    std::vector<typename ComponentDlg::param_type> *params;
    ComponentDlg *dlg;
    int idx;
    ComponentWidget *_widget;
};


class ComponentTable : public QTableWidget
{
    Q_OBJECT

public:
    ComponentTable(QWidget *parent = 0);
    ~ComponentTable() {}

    void init(QVector<GenericComponent *> prototypes, ChannelListModel *in, ChannelListModel *out);

    void importData();
    void exportData();

private slots:
    void addComponent();

private:
    QVector<GenericComponent *> proto, comp;
    QVector<int> idx;
    ComponentFactoryWidget *factory;
    ChannelListModel *in, *out;

    void makeFactory();
};

#endif // COMPONENTTABLE_H
