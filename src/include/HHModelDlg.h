#ifndef HHMODELDLG_H
#define HHMODELDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include "ObjectDataTypes.h"

namespace Ui {
class HHModelDlg;
}
enum class DeviceStatus;

class HHModelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HHModelDlg(int idx, QWidget *parent = 0);
    ~HHModelDlg();

    void importData();
    void exportData(bool = false);

    typedef HHNeuronData param_type;
    typedef false_type isDAQ;

public slots:
    void accept();
    void reject();

signals:
    void channelsChanged();
    void modelStatusChanged();

private:
    Ui::HHModelDlg *ui;
    QVector<QCheckBox*> actives, vSaves, spkDs, iSaves;
    QVector<QDoubleSpinBox*> vBiases, spkThrs, iBiases;
    QMetaObject::Connection activec, vSavec, vBiasc, spkDc, spkThrc, iSavec, iBiasc;

    int idx;

    void addRow(int row, QCheckBox *active,
                QCheckBox *vSave, QDoubleSpinBox *vBias, QCheckBox *spkD, QDoubleSpinBox *spkThr,
                QCheckBox *iSave, QDoubleSpinBox *iBias);
    void setCellCheckBox(int row, int column, QCheckBox *box);

private slots:
    void growTable(bool reactive = true);
    void addMultiple();
};

#endif // HHMODELDLG_H
