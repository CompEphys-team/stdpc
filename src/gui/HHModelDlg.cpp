#include "HHModelDlg.h"
#include "ui_HHModelDlg.h"
#include "Global.h"

/// Developer's note: In the interest of channel index stability, do not allow removing/reordering rows

HHModelDlg::HHModelDlg(int idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HHModelDlg),
    idx(idx)
{
    ui->setupUi(this);
    ui->titleLabel->setText(ui->titleLabel->text().arg(idx));
    ui->table->setHorizontalHeaderLabels({"Active",
                                          "",
                                          "VChan\nSave",
                                          "VChan\nBias (mV)",
                                          "VChan\nDetect spikes",
                                          "VChan\nThreshold (mV)",
                                          "",
                                          "IChan\nSave",
                                          "IChan\nBias (nA)"});
    ui->table->setColumnWidth(0, 40);
    ui->table->setColumnWidth(1, 10);
    ui->table->setColumnWidth(2, 80);
    ui->table->setColumnWidth(3, 80);
    ui->table->setColumnWidth(4, 80);
    ui->table->setColumnWidth(5, 100);
    ui->table->setColumnWidth(6, 10);
    ui->table->setColumnWidth(7, 80);
    ui->table->setColumnWidth(8, 80);
    growTable(false);

    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addMultiple()));
}

HHModelDlg::~HHModelDlg()
{
    delete ui;
}

void HHModelDlg::importData()
{
    int row = 0;
    QCheckBox *active, *vSave, *spkD, *iSave;
    QDoubleSpinBox *vBias, *spkThr, *iBias;

    actives.clear();
    vSaves.clear();
    vBiases.clear();
    spkDs.clear();
    spkThrs.clear();
    iSaves.clear();
    iBiases.clear();
    ui->table->setRowCount(0);

    for ( vInstData const& inst : HHNeuronp[idx].inst ) {
        active = new QCheckBox();
        vSave = new QCheckBox();
        vBias = new QDoubleSpinBox();
        spkD = new QCheckBox();
        spkThr = new QDoubleSpinBox();
        iSave = new QCheckBox();
        iBias = new QDoubleSpinBox();
        addRow(row++, active, vSave, vBias, spkD, spkThr, iSave, iBias);
        active->setChecked(inst.active);
        vSave->setChecked(inst.inChn.chnlSaving);
        vBias->setValue(inst.inChn.bias * 1e3);
        spkD->setChecked(inst.inChn.spkDetect);
        spkThr->setValue(inst.inChn.spkDetectThresh * 1e3);
        iSave->setChecked(inst.outChn.chnlSaving);
        iBias->setValue(inst.outChn.bias * 1e9);
    }
    growTable(false);

    ui->C->setValue(HHNeuronp[idx].C * 1e9);
    ui->gLeak->setValue(HHNeuronp[idx].gLeak * 1e9);
    ui->ELeak->setValue(HHNeuronp[idx].ELeak * 1e3);
}

void HHModelDlg::exportData(bool)
{
    vInstData inst;
    HHNeuronp[idx].inst.clear();
    HHNeuronp[idx].inst.reserve(ui->table->rowCount() - 1);
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        inst.active = actives[i]->isChecked();
        inst.inChn.chnlSaving = vSaves[i]->isChecked();
        inst.inChn.bias = vBiases[i]->value() * 1e-3;
        inst.inChn.spkDetect = spkDs[i]->isChecked();
        inst.inChn.spkDetectThresh = spkThrs[i]->value() * 1e-3;
        inst.outChn.chnlSaving = iSaves[i]->isChecked();
        inst.outChn.bias = iBiases[i]->value() * 1e-9;
        HHNeuronp[idx].inst.push_back(inst);
    }

    HHNeuronp[idx].C = ui->C->value() * 1e-9;
    HHNeuronp[idx].gLeak = ui->gLeak->value() * 1e-9;
    HHNeuronp[idx].ELeak = ui->ELeak->value() * 1e-3;
    emit channelsChanged();
}

void HHModelDlg::accept()
{
    exportData();
    QDialog::accept();
}

void HHModelDlg::reject()
{
    importData();
    QDialog::reject();
}

void HHModelDlg::addRow(int row, QCheckBox *active,
                          QCheckBox *vSave, QDoubleSpinBox *vBias, QCheckBox *spkD, QDoubleSpinBox *spkThr,
                          QCheckBox *iSave, QDoubleSpinBox *iBias)
{
    ui->table->insertRow(row);
    ui->table->setRowHeight(row, 25);
    QStringList labels;
    for ( int i = 0; i < ui->table->rowCount(); i++ )
        labels << QString::number(i);
    ui->table->setVerticalHeaderLabels(labels);

    setCellCheckBox(row, 0, active);

    setCellCheckBox(row, 2, vSave);
    ui->table->setCellWidget(row, 3, vBias);
    setCellCheckBox(row, 4, spkD);
    ui->table->setCellWidget(row, 5, spkThr);

    setCellCheckBox(row, 7, iSave);
    ui->table->setCellWidget(row, 8, iBias);

    vBias->setRange(-1000, 1000);
    spkThr->setRange(-1000, 1000);
    iBias->setRange(-1e6, 1e6);

    actives.insert(row, active);
    vSaves.insert(row, vSave);
    vBiases.insert(row, vBias);
    spkDs.insert(row, spkD);
    spkThrs.insert(row, spkThr);
    iSaves.insert(row, iSave);
    iBiases.insert(row, iBias);
}

void HHModelDlg::setCellCheckBox(int row, int column, QCheckBox *box)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(box);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    widget->setLayout(layout);
    ui->table->setCellWidget(row, column, widget);
}

void HHModelDlg::growTable(bool reactive)
{
    disconnect(activec);
    disconnect(vSavec);
    disconnect(vBiasc);
    disconnect(spkDc);
    disconnect(spkThrc);
    disconnect(iSavec);
    disconnect(iBiasc);

    if ( reactive && !actives.empty() )
        actives.last()->setChecked(true);

    QCheckBox *active = new QCheckBox();
    QCheckBox *vSave = new QCheckBox();
    QDoubleSpinBox *vBias = new QDoubleSpinBox();
    QCheckBox *spkD = new QCheckBox();
    QDoubleSpinBox *spkThr = new QDoubleSpinBox();
    QCheckBox *iSave = new QCheckBox();
    QDoubleSpinBox *iBias = new QDoubleSpinBox();

    addRow(ui->table->rowCount(), active, vSave, vBias, spkD, spkThr, iSave, iBias);

    activec = connect(active, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    vSavec = connect(vSave, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    vBiasc = connect(vBias, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
    spkDc = connect(spkD, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    spkThrc = connect(spkThr, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
    iSavec = connect(iSave, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    iBiasc = connect(iBias, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
}

void HHModelDlg::addMultiple()
{
    QCheckBox *active, *vSave, *spkD, *iSave;
    QDoubleSpinBox *vBias, *spkThr, *iBias;
    int insRow = ui->table->rowCount() - 1;
    int copyRow = insRow ? insRow - 1 : 0;
    for ( int i = 0; i < ui->addNumber->value(); i++ ) {
        active = new QCheckBox();
        vSave = new QCheckBox();
        vBias = new QDoubleSpinBox();
        spkD = new QCheckBox();
        spkThr = new QDoubleSpinBox();
        iSave = new QCheckBox();
        iBias = new QDoubleSpinBox();

        addRow(insRow, active, vSave, vBias, spkD, spkThr, iSave, iBias);

        if ( insRow > 0 ) {
            active->setChecked(actives[copyRow]->isChecked());
            vSave->setChecked(vSaves[copyRow]->isChecked());
            vBias->setValue(vBiases[copyRow]->value());
            spkD->setChecked(spkDs[copyRow]->isChecked());
            spkThr->setValue(spkThrs[copyRow]->value());
            iSave->setChecked(iSaves[copyRow]->isChecked());
            iBias->setValue(iBiases[copyRow]->value());
        } else {
            active->setChecked(true);
        }
    }
}
