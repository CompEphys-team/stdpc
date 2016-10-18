#ifndef SPIKEGENDLG_H
#define SPIKEGENDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include "ObjectDataTypes.h"
#include "ChannelListModel.h"

namespace Ui {
class SpikeGenDlg;
}

class SpikeGenDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SpikeGenDlg(int idx, QWidget *parent = 0);
    ~SpikeGenDlg();

    void importData();
    void exportData(bool = false);
    void setIndex(int);

    typedef SGData param_type;
    typedef std::false_type isDAQ;

public slots:
    void accept();
    void reject();

signals:
    void channelsChanged();
    void modelStatusChanged();

private:
    Ui::SpikeGenDlg *ui;
    QString label;
    int idx;

    ChannelListModel clm;

    QVector<QCheckBox*> actives, vSaves;
    QVector<QDoubleSpinBox*> vBiases;
    QMetaObject::Connection activec, vSavec, vBiasc;

    void addInstRow(int row, QCheckBox *active, QCheckBox *vSave, QDoubleSpinBox *vBias);
    void setCellCheckBox(int row, int column, QCheckBox *box);

    QDoubleSpinBox *makeSTCell(int row, int col);
    void importST(std::vector<std::vector<double>> &);
    void exportST(std::vector<std::vector<double>> &);

private slots:
    void growInstTable(bool = true);
    void growSTTable(int row, int col);
};

#endif // SPIKEGENDLG_H
