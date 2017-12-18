#ifndef DAQOPTSBASE_H
#define DAQOPTSBASE_H

#include "DaqWidget.h"

class DaqOptsBase : public QObject
{
    Q_OBJECT

public:
    DaqOptsBase(QWidget *parent, size_t idx) : QObject(parent), idx(idx) {}
    virtual ~DaqOptsBase() {}
    virtual void removeDevice() = 0;

    virtual void importData() = 0;
    virtual void exportData() = 0;

    virtual void setIndex(size_t i) = 0;

    virtual DaqWidget *widget() = 0;
    virtual void regenerateWidget() = 0;

signals:
    void removedModel(ChannelIndex);

protected slots:
    virtual void activeChanged() = 0;
    virtual void setLabel() = 0;

protected:
    size_t idx;
};

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
                inst[i]->removeDevice();
                delete inst[i];
                inst.remove(i);
                --i;
            }
        }
    }

    virtual void exportData(bool ignoreDAQ = false) = 0;

    QVector<DaqOptsBase *> inst;
};

#endif // DAQOPTSBASE_H
