#ifndef HHMODELDLG_H
#define HHMODELDLG_H

#include "ModelDlg.h"
#include <QCheckBox>
#include <QDoubleSpinBox>

namespace Ui {
class HHModelDlg;
}

class HHModelDlg : public ModelDlg
{
    Q_OBJECT

public:
    explicit HHModelDlg(size_t idx, QWidget *parent = 0);
    ~HHModelDlg();

    void importData();
    void exportData();
    void setIndex(size_t);

private:
    Ui::HHModelDlg *ui;
    QVector<QCheckBox*> actives, vSaves, spkDs, iSaves;
    QVector<QDoubleSpinBox*> vBiases, spkThrs, iBiases;
    QMetaObject::Connection activec, vSavec, vBiasc, spkDc, spkThrc, iSavec, iBiasc;

    QString label;

    void addRow(int row, QCheckBox *active,
                QCheckBox *vSave, QDoubleSpinBox *vBias, QCheckBox *spkD, QDoubleSpinBox *spkThr,
                QCheckBox *iSave, QDoubleSpinBox *iBias);
    void setCellCheckBox(int row, int column, QCheckBox *box);

private slots:
    void growTable(bool reactive = true);
    void addMultiple();
};

#endif // HHMODELDLG_H
