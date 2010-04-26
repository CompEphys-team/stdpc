#ifndef DataSavingDlg_H
#define DataSavingDlg_H

#include <QDialog>
#include <QFileDialog>
#include "ui_DataSavingDlg.h"


namespace Ui {
    class DataSavingDlg;
}

class DataSavingDlg : public QDialog, private Ui::DataSavingDlg
{
    Q_OBJECT

public:

    QFileDialog *SaveFileNameDlg;
    DataSavingDlg(QWidget *parent = 0);
    ~DataSavingDlg();
    void exportData();
    void importData();

private slots:
    void updateSaveFileName();

};

#endif // DataSavingDlg_H
