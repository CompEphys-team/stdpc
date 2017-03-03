#ifndef DAQOPTS_H
#define DAQOPTS_H

#include "DaqOptsBase.h"
#include "DAQDlg.h"

class DaqOpts : public DaqOptsBase
{
public:
    DaqOpts(QWidget *parent, DAQProxy *proxy, size_t idx);
    ~DaqOpts();

    void removeDevice();
    void importData();
    void exportData();
    void setIndex(size_t i);
    inline DaqWidget *widget() { return _widget; }
    void regenerateWidget();

protected:
    void activeChanged();

    DAQProxy *proxy;
    DAQDlg *dlg;
    DaqWidget *_widget;
};

class DaqOptsPrototype : public DaqOptsPrototypeBase
{
public:
    DaqOptsPrototype(DAQProxy *proxy) : proxy(proxy) {}
    ~DaqOptsPrototype() {}

    inline QString label() { return proxy->prettyName(); }

    DaqOptsBase *create(size_t idx, QWidget *parent);
    void createAll(QWidget *parent);
    void exportData(bool ignoreDAQ = false);

private:
    DAQProxy *proxy;
};

#endif // DAQOPTS_H
