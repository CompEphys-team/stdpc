#ifndef DAQTABLE_H
#define DAQTABLE_H

#include <QTableWidget>
#include "DaqWidget.h"

#include "SimulDAQDlg.h"
#include "DigiDataDlg.h"
#ifdef NATIONAL_INSTRUMENTS
#include "NIDAQDlg.h"
#endif

#include "HHModelDlg.h"

class GenericDaqOpts : public QObject
{
    Q_OBJECT

public:
    GenericDaqOpts(QWidget *parent) : parent(parent) {}
    virtual ~GenericDaqOpts() {}

    virtual GenericDaqOpts *create(int idx) = 0;
    virtual QVector<GenericDaqOpts *> createAll() = 0;
    virtual void importData() = 0;
    virtual void exportData(bool ignoreDAQ = false) = 0;

    virtual QString const& label() = 0;
    virtual DaqWidget *widget() = 0;

    QWidget *parent;

protected slots:
    virtual void activeChanged() = 0;
};

template <class DaqDlg>
class DaqOpts : public GenericDaqOpts
{
public:
    DaqOpts(QWidget *parent, QString const& label, std::vector<typename DaqDlg::param_type> *params) :
        GenericDaqOpts(parent), _label(label), params(params), dlg(nullptr), idx(0), _widget(nullptr) {}
    ~DaqOpts() { delete dlg; }

    GenericDaqOpts *create(int idx);
    QVector<GenericDaqOpts *> createAll();

    void importData();
    void exportData(bool ignoreDAQ = false);

    inline DaqWidget *widget() { return _widget; }
    inline QString const& label() { return _label; }

    void activeChanged();

    QString _label;
    std::vector<typename DaqDlg::param_type> *params;
    DaqDlg *dlg;
    int idx;
    DaqWidget *_widget;
};


class DaqTable : public QTableWidget
{
    Q_OBJECT

public:
    DaqTable(QWidget *parent = 0);
    ~DaqTable() {}

    void init(QVector<GenericDaqOpts *> prototypes);

    void importData();
    void exportData(bool ignoreDAQ = false);

private slots:
    void addDaqOpts();

private:
    QVector<GenericDaqOpts *> proto, comp;
    QVector<int> idx;
    DaqFactoryWidget *factory;

    void makeFactory();
};

#endif // DAQTABLE_H
