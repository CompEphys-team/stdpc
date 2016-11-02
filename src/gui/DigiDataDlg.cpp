#include "DigiDataDlg.h"
#include "Tools.cpp"
#include "DigiData.h"

DigiDataDlg::DigiDataDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
    setupUi(this);
    connect(inChannels, SIGNAL(clicked(bool)), this, SLOT(openInChnDlg()));
    connect(outChannels, SIGNAL(clicked(bool)), this, SLOT(openOutChnDlg()));
    label = DAQLabel->text();
    setIndex(idx);
}

void DigiDataDlg::setIndex(size_t no)
{
    DAQLabel->setText(label.arg(no));
    DAQDlg::setIndex(no);
}

void DigiDataDlg::exportData(bool forceInit)
{
    bool change= false;
    bool success;
    short int tmp= (short int) BaseAddressE->text().toInt(&success,16);
    if (success) {
        getEntry(DigiDataProxy::p[idx].baseAddress, tmp, change);
    }
    DAQDlg::exportData(forceInit || change);
}

void DigiDataDlg::importData()
{
    QString num;
    num.setNum(DigiDataProxy::p[idx].baseAddress,16);
    BaseAddressE->setText(num);
    DAQDlg::importData();
}

void DigiDataDlg::backup()
{
    bak = DigiDataProxy::p[idx];
}

void DigiDataDlg::restoreBackup()
{
    DigiDataProxy::p[idx] = bak;
}
