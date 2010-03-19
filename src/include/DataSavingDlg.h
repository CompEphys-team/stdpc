#ifndef DataSavingDlg_H
#define DataSavingDlg_H

#include <QDialog>
#include "ui_DataSavingDlg.h"


namespace Ui {
    class DataSavingDlg;
}

class DataSavingDlg : public QDialog, private Ui::DataSavingDlg
{
    Q_OBJECT

public:
    
    DataSavingDlg(QWidget *parent = 0);
    ~DataSavingDlg();
    QString GetFileName();
    QVector<bool> GetSaveSettings();

};

#endif // DataSavingDlg_H
