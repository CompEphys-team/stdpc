#ifndef MODELINSTDLG_H
#define MODELINSTDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include "ObjectDataTypes.h"

namespace Ui {
class ModelInstDlg;
}

class ModelInstDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ModelInstDlg(QWidget *parent = 0);
    ~ModelInstDlg();

    void importData(std::vector<vInstData> const&);
    void exportData(std::vector<vInstData> &);

private:
    Ui::ModelInstDlg *ui;
    QVector<QCheckBox*> actives, vSaves, spkDs, iSaves;
    QVector<QDoubleSpinBox*> vBiases, spkThrs, iBiases;
    QMetaObject::Connection activec, vSavec, vBiasc, spkDc, spkThrc, iSavec, iBiasc;

    void addRow(int row, QCheckBox *active,
                QCheckBox *vSave, QDoubleSpinBox *vBias, QCheckBox *spkD, QDoubleSpinBox *spkThr,
                QCheckBox *iSave, QDoubleSpinBox *iBias);
    void setCellCheckBox(int row, int column, QCheckBox *box);

private slots:
    void growTable(bool doConnections = true);
    void addMultiple();
};

#endif // MODELINSTDLG_H
