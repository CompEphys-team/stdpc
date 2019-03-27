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
    DataSavingDlg(QWidget *parent = nullptr);
    ~DataSavingDlg();
    void exportData();
    void importData();

private slots:
    void on_browse_clicked();
};

#endif // DataSavingDlg_H
