#include "NIDAQDlg.h"
#include "Util.h"
#include "Nidaq.h"

NIDAQDlg::NIDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
    setupUi(this);
    connect(inChannels, SIGNAL(clicked(bool)), this, SLOT(openInChnDlg()));
    connect(outChannels, SIGNAL(clicked(bool)), this, SLOT(openOutChnDlg()));
    setIndex(idx);
}

void NIDAQDlg::setIndex(size_t no)
{
    DAQLabel->setText(QString("%1 %2").arg(NIDAQProxy::get()->prettyName()).arg(no));
    DAQDlg::setIndex(no);
}

void NIDAQDlg::exportData(bool forceInit)
{
    bool change= false;
    getEntry<QString>(NIDAQProxy::p[idx].label, leLabel->text(), change);
    getEntry(NIDAQProxy::p[idx].deviceName, DeviceNameE->text(), change);
    DAQDlg::exportData(change || forceInit);
}

void NIDAQDlg::importData()
{
    leLabel->setText(NIDAQProxy::p[idx].label);
    DeviceNameE->setText(NIDAQProxy::p[idx].deviceName);
    DAQDlg::importData();
}

void NIDAQDlg::backup()
{
    bak = NIDAQProxy::p[idx];
}

void NIDAQDlg::restoreBackup()
{
    NIDAQProxy::p[idx] = bak;
}
