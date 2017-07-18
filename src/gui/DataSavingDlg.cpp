#include "DataSavingDlg.h"
#include "ui_DataSavingDlg.h"
#include "Global.h"


DataSavingDlg::DataSavingDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    SaveFileNameDlg= new QFileDialog(this, QString("Save File Dialog"), QString("."),
           QString("*.dat"));
    SaveFileNameDlg->setAcceptMode(QFileDialog::AcceptSave);

    QObject::connect(browseButton, SIGNAL(released()), SaveFileNameDlg, SLOT(show()));
    QObject::connect(SaveFileNameDlg, SIGNAL(accepted()), SLOT(updateSaveFileName()));
}


DataSavingDlg::~DataSavingDlg()
{
}

void DataSavingDlg::updateSaveFileName()
{
    QStringList fnlist= SaveFileNameDlg->selectedFiles();
    leFileName->setText(*fnlist.begin());
}

void DataSavingDlg::exportData()
{
    dataSavingPs.fileName = leFileName->text();
    double sFreq = 1000 * leSavingFreq->text().toDouble();   // kHz -> Hz
    if ( sFreq <= 0.0 ) sFreq = 1000; // set to 1kHz by default
    dataSavingPs.savingFreq = sFreq;
    dataSavingPs.isBinary = bgFormat->checkedButton() == rbBinary;
}

void DataSavingDlg::importData()
{
    QString sFreq;
    leFileName->setText(dataSavingPs.fileName);
    sFreq.setNum(dataSavingPs.savingFreq / 1000);   // Hz -> kHz
    leSavingFreq->setText(sFreq);
    (dataSavingPs.isBinary ? rbBinary : rbAscii)->setChecked(true);
}
