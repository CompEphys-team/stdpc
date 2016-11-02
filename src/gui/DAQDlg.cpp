#include "DAQDlg.h"

DAQDlg::DAQDlg(size_t idx, DAQ *board, QWidget *parent) :
    QDialog(parent),
    idx(idx),
    inDlg(new InputChannelDlg(this)),
    outDlg(new OutputChannelDlg(this))
{
    inDlg->init(board);
    outDlg->init(board);
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
    inDlg->dex = ChannelIndex(ChannelIndex::Analog, proxy()->daqClass(), i);
    idx = i;
}

void DAQDlg::initDAQ()
{
    QString name;
    DeviceStatus status = Devices.initSingle(name, proxy(), idx);
    emit deviceStatusChanged(status, name);
    DAQ *board = Devices.getDevice(ChannelIndex(ChannelIndex::Analog, proxy()->daqClass(), idx));
    inDlg->init(board);
    outDlg->init(board);
    didInit = true;
}
