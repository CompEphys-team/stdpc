#include "DataSavingDlg.h"
#include "ui_DataSavingDlg.h"


DataSavingDlg::DataSavingDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}


DataSavingDlg::~DataSavingDlg()
{
}

QString DataSavingDlg::GetFileName()
{
    return leFileName->text();
}


QVector<bool> DataSavingDlg::GetSaveSettings()
{
    QCheckBox* saveElectrode;
    int numOfElectrodes = 4;
    QVector<bool> saveSettings = QVector<bool>(numOfElectrodes);

    for ( int i=0; i<numOfElectrodes; i++ )
    {
        saveElectrode = findChild<QCheckBox *>(QString("saveElectrode_")+QString::number(i+1));
        saveSettings[i] = saveElectrode->isChecked();
    }

    return saveSettings;
}
