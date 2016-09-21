#include "DigiDataDlg.h"
#include "Tools.cpp"
#include "DigiData.h"
#include "DeviceManager.h"

DigiDataDlg::DigiDataDlg(int no, QWidget *parent) : DAQDlg(no, parent)
{
    setupUi(this);
    DAQLabel->setText(DAQLabel->text().arg(no));
    ChannelIndex dex;
    dex.isValid = true;
    dex.isAnalog = true;
    dex.daqClass = DAQClass::DD1200;
    dex.devID = no;
    board = Devices.getDevice(dex);
    inDlg->init(board);
    outDlg->init(board);
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
    if ( change || forceInit )
        devOK = initDAQ() != DeviceStatus::Failed;
    inDlg->exportData();
    outDlg->exportData();
    emit channelsChanged();
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
    emit channelsChanged();
    importData();
    QDialog::reject();
}


DeviceStatus DigiDataDlg::initDAQ()
{
    QString name;
    DeviceStatus status = Devices.initSingle<DigiDataData>(name, idx);
    emit deviceStatusChanged(status, name);
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
