#include "NIDAQDlg.h"
#include "Tools.cpp"
#include "Nidaq.h"

NIDAQDlg::NIDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
    setupUi(this);
    connect(inChannels, SIGNAL(clicked(bool)), this, SLOT(openInChnDlg()));
    connect(outChannels, SIGNAL(clicked(bool)), this, SLOT(openOutChnDlg()));
    label = DAQLabel->text();
    setIndex(idx);
}

void NIDAQDlg::setIndex(size_t no)
{
    DAQLabel->setText(label.arg(no));
    DAQDlg::setIndex(no);
}

void NIDAQDlg::exportData(bool forceInit)
{
    bool change= false;
    getEntry(NIDAQProxy::p[idx].deviceName, DeviceNameE->text(), change);
    DAQDlg::exportData(change || forceInit);
}

void NIDAQDlg::importData()
{
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
