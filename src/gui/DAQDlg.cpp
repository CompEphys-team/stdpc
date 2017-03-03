#include "DAQDlg.h"

DAQDlg::DAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    QDialog(parent),
    idx(idx),
    proxy(proxy),
    inDlg(new InputChannelDlg(idx, proxy, this)),
    outDlg(new OutputChannelDlg(idx, proxy, this))
{
    inDlg->init();
    outDlg->init();
}

void DAQDlg::importData()
{
    inDlg->importData();
    outDlg->importData();
}

void DAQDlg::exportData(bool forceInit)
{
    inDlg->exportData();
    outDlg->exportData();
    if ( forceInit ) {
        initDAQ();
        inDlg->importData();
        outDlg->importData();
    }
}

void DAQDlg::setIndex(size_t i) {
    inDlg->setIndex(i);
    outDlg->setIndex(i);
    idx = i;
}

void DAQDlg::initDAQ()
{
    QString name;
    DeviceStatus status = Devices.initSingle(name, proxy, idx);
    emit deviceStatusChanged(status, name);
    inDlg->init();
    outDlg->init();
    didInit = true;
}
