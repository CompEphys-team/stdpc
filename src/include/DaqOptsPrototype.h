#ifndef DAQOPTSPROTOTYPE_H
#define DAQOPTSPROTOTYPE_H

#include "DaqOpts.h"

class DaqOptsPrototypeBase
{
public:
    DaqOptsPrototypeBase() :
        inst()
    {}

    virtual ~DaqOptsPrototypeBase()
    {
        for ( DaqOptsBase *c : inst )
            delete c;
    }

    virtual QString label() = 0;

    virtual DaqOptsBase *create(size_t idx, QWidget *parent) = 0;
    virtual void createAll(QWidget *parent) = 0;

    void clearInactive()
    {
        for ( int i = 0; i < inst.size(); i++ ) {
            if ( inst[i]->widget()->active->isChecked() ) {
                inst[i]->regenerateWidget();
                inst[i]->widget()->active->setChecked(true);
                inst[i]->setIndex(i);
            } else {
                inst[i]->removeDevice(i);
                delete inst[i];
                inst.remove(i);
                --i;
            }
        }
    }

    virtual void exportData(bool ignoreDAQ = false) = 0;

    QVector<DaqOptsBase *> inst;
};

template <class DaqDlg>
class DaqOptsPrototype : public DaqOptsPrototypeBase
{
public:
    DaqOptsPrototype(QString const& label, std::vector<typename DaqDlg::param_type> *params) :
        _label(label),
        params(params)
    {}
    ~DaqOptsPrototype() {}

    inline QString label() { return _label; }

    DaqOptsBase *create(size_t idx, QWidget *parent)
    {
        DaqOpts<DaqDlg> *c = new DaqOpts<DaqDlg>(parent, _label, params, idx);
        inst.push_back(c);
        return c;
    }

    void createAll(QWidget *parent)
    {
        for ( DaqOptsBase *c : inst )
            delete c;
        inst.clear();
        inst.reserve(params->size());
        for ( size_t i = 0; i < params->size(); i++ ) {
            create(i, parent)->importData();
        }
    }

    void exportData(bool ignoreDAQ = false)
    {
        if ( !ignoreDAQ || !DaqDlg::isDAQ::value ) {
            if ( !DaqDlg::isDAQ::value )
                while ( params->size() && params->back().removed )
                    params->erase(params->end() - 1);
            params->resize(inst.size());
            for ( DaqOptsBase *c : inst )
                c->exportData();
        }
    }

private:
    QString _label;
    std::vector<typename DaqDlg::param_type> *params;
};

#endif // DAQOPTSPROTOTYPE_H
