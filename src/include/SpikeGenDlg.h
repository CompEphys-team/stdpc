#ifndef SPIKEGENDLG_H
#define SPIKEGENDLG_H

#include "ModelDlg.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QStyledItemDelegate>
#include <QStandardItemModel>
#include "WideComboBox.h"
#include "ChannelListModel.h"

namespace Ui {
class SpikeGenDlg;
}

class STDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    STDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}
    ~STDelegate() {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
                          const QModelIndex &/*index*/) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const override;
};

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

    QStandardItemModel model;
    STDelegate delegate;

    void addInstRow(int row, QCheckBox *active, QCheckBox *vSave, QDoubleSpinBox *vBias,
                    WideComboBox *bdChannel, QDoubleSpinBox *bdThresh);
    void setCellCheckBox(int row, int column, QCheckBox *box);

    void importST(std::vector<std::vector<double>> &);
    void exportST(std::vector<std::vector<double>> &);

private slots:
    void growInstTable(bool = true);
};

#endif // SPIKEGENDLG_H
