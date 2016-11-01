#include "DigiDataDlg.h"
#include "Tools.cpp"
#include "DigiData.h"
#include "DeviceManager.h"

DigiDataDlg::DigiDataDlg(int no, QWidget *parent) : DAQDlg(no, parent)
{
    setupUi(this);
    label = DAQLabel->text();
    setIndex(no);
    DAQ *board = Devices.getDevice(ChannelIndex(DAQClass::DD1200, idx));
    inDlg->init(board);
    outDlg->init(board);
}

void DigiDataDlg::setIndex(int no)
{
    idx = no;
    DAQLabel->setText(label.arg(no));
    ChannelIndex dex(DAQClass::DD1200, no);
    inDlg->dex = dex;
    emit channelsChanged();
}

bool DigiDataDlg::exportData(bool forceInit)
{
    bool change= false;
    bool success;
    bool devOK = true;

    short int tmp= (short int) BaseAddressE->text().toInt(&success,16);
    if (success) {
        getEntry(DigiDatap[idx].baseAddress, tmp, change);
    }
    inDlg->exportData();
    outDlg->exportData();
    if ( change || forceInit ) {
        devOK = initDAQ() != DeviceStatus::Failed;
        inDlg->importData();
        outDlg->importData();
    }
    return devOK;
}

void DigiDataDlg::importData()
{
    QString num;
    num.setNum(DigiDatap[idx].baseAddress,16);
    BaseAddressE->setText(num);
    inDlg->importData();
    outDlg->importData();
}

void DigiDataDlg::open()
{
    didInit = false;
    backup = DigiDatap[idx];
    QDialog::open();
}

void DigiDataDlg::accept()
{
    exportData();
    QDialog::accept();
}

void DigiDataDlg::reject()
{
    DigiDatap[idx] = backup;
    if ( didInit )
        initDAQ();
    importData();
    QDialog::reject();
}


DeviceStatus DigiDataDlg::initDAQ()
{
    QString name;
    DeviceStatus status = Devices.initSingle(name, Devices.Register()["DigiData"], idx);
    emit deviceStatusChanged(status, name);
    DAQ *board = Devices.getDevice(ChannelIndex(DAQClass::DD1200, idx));
    inDlg->init(board);
    outDlg->init(board);
    didInit = true;
    return status;
}


void DigiDataDlg::on_inChannels_clicked()
{
    exportData();
    inDlg->open();
}

void DigiDataDlg::on_outChannels_clicked()
{
    exportData();
    outDlg->open();
}
