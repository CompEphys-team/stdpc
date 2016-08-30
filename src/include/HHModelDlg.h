#ifndef HHMODELDLG_H
#define HHMODELDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "ModelInstDlg.h"

namespace Ui {
class HHModelDlg;
}

class HHModelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HHModelDlg(QWidget *parent = 0);
    ~HHModelDlg();

    void importData();
    void exportData();

private:
    Ui::HHModelDlg *ui;
    QVector<QCheckBox*> boxes;
    QVector<QDoubleSpinBox*> Cs, gLeaks, ELeaks;
    QMetaObject::Connection boxc, Cc, gLc, ELc, btnc;
    QVector<ModelInstDlg*> instDlg;

    void addRow(int row, QCheckBox *box, QDoubleSpinBox *C, QDoubleSpinBox *gLeak, QDoubleSpinBox *ELeak,
                QPushButton *btn);

private slots:
    void growTable();
};

#endif // HHMODELDLG_H
