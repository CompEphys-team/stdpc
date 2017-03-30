#ifndef SPIKEGENDLG_H
#define SPIKEGENDLG_H

#include "ModelDlg.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include "WideComboBox.h"
#include "ChannelListModel.h"

namespace Ui {
class SpikeGenDlg;
}

class SpikeGenDlg : public ModelDlg
{
    Q_OBJECT

public:
    explicit SpikeGenDlg(size_t idx, QWidget *parent = 0);
    ~SpikeGenDlg();

    void importData();
    void exportData();
    void setIndex(size_t);

private:
    Ui::SpikeGenDlg *ui;
    QString label;

    ChannelListModel *clm;

    QVector<QCheckBox*> actives, vSaves;
    QVector<QDoubleSpinBox*> vBiases, bdThresholds;
    QVector<WideComboBox*> bdChannels;
    QMetaObject::Connection activec, vSavec, vBiasc, bdChannelc, bdThresholdc;

    void addInstRow(int row, QCheckBox *active, QCheckBox *vSave, QDoubleSpinBox *vBias,
                    WideComboBox *bdChannel, QDoubleSpinBox *bdThresh);
    void setCellCheckBox(int row, int column, QCheckBox *box);

    QDoubleSpinBox *makeSTCell(int row, int col);
    void importST(std::vector<std::vector<double>> &);
    void exportST(std::vector<std::vector<double>> &);

private slots:
    void growInstTable(bool = true);
    void growSTTable(int row, int col);
};

#endif // SPIKEGENDLG_H
