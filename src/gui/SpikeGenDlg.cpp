#include "SpikeGenDlg.h"
#include "ui_SpikeGenDlg.h"
#include "Global.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>

SpikeGenDlg::SpikeGenDlg(int idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpikeGenDlg),
    clm(ChannelListModel::AnalogIn | ChannelListModel::Virtual, this)
{
    ui->setupUi(this);
    label = ui->titleLabel->text();
    setIndex(idx);

    ui->instTable->setHorizontalHeaderLabels({"Active",
                                              "",
                                              "Save",
                                              "Bias (mV)"});
    ui->instTable->setColumnWidth(0, 40);
    ui->instTable->setColumnWidth(1, 10);
    ui->instTable->setColumnWidth(2, 80);
    ui->instTable->setColumnWidth(3, 80);
    growInstTable(false);

    connect(ui->btnLoadST, &QPushButton::clicked, [this](bool){
        QString fname = QFileDialog::getOpenFileName(this, "Select spike time file...");
        if ( fname.isEmpty() )
            return;
        ifstream is(fname.toStdString());
        if ( !is.good() ) {
            QMessageBox::warning(this, "Warning", QString("Failed to open file %1 for reading.").arg(fname));
            return;
        }
        std::vector<std::vector<double>> vec;
        int sn;
        is >> sn;
        while ( is.good() ) {
            std::vector<double> tmp(sn);
            for (int i = 0; i < sn && is.good(); i++) {
                is >> tmp[i];
            }
            vec.push_back(tmp);
            is >> sn;
        }
        this->importST(vec);
    });

    connect(ui->btnSaveST, &QPushButton::clicked, [this](bool){
        QString fname = QFileDialog::getSaveFileName(this, "Save spike times to file...");
        if ( fname.isEmpty() )
            return;
        ofstream os(fname.toStdString());
        if ( !os.good() ) {
            QMessageBox::warning(this, "Warning", QString("Failed to open file %1 for writing.").arg(fname));
            return;
        }
        std::vector<std::vector<double>> tmp;
        this->exportST(tmp);
        for ( int row = 0; row < (int)tmp.size(); row++ ) {
            os << tmp[row].size() << " ";
            for ( int col = 0; col < (int)tmp[row].size(); col++ ) {
                os << tmp[row][col] << " ";
            }
            os << '\n';
        }
    });

    connect(ui->BurstDetectionCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int i){
        ui->bdGroupW->setEnabled(i > 0);
    });

    ui->SGbdChannelCombo->setModel(&clm);
    connect(parent, SIGNAL(channelsChanged()), &clm, SLOT(updateChns()));
    connect(parent, SIGNAL(modelRemoved(ChannelIndex)), &clm, SLOT(updateChns(ChannelIndex)));
}



SpikeGenDlg::~SpikeGenDlg()
{
    delete ui;
}

void SpikeGenDlg::setIndex(int no)
{
    idx = no;
    ui->titleLabel->setText(label.arg(no));
    emit channelsChanged();
}

void SpikeGenDlg::importData()
{
    ui->SGLUTableCombo->setCurrentIndex(SGp[idx].LUTables);
    ui->VSpike->setValue(SGp[idx].VSpike * 1e3);
    ui->Width->setValue(5e3 / SGp[idx].spkTimeScaling);
    ui->VRest->setValue(SGp[idx].VRest * 1e3);

    ui->BurstDetectionCombo->setCurrentIndex(SGp[idx].bdType);
    ui->SGbdChannelCombo->setCurrentIndex(clm.index(SGp[idx].bdChannel));
    ui->Threshold->setValue(SGp[idx].bdThresh * 1e3);
    ui->tUnder->setValue(SGp[idx].bdtUnder * 1e3);
    ui->tOver->setValue(SGp[idx].bdtOver * 1e3);

    ui->Period->setValue(SGp[idx].period * 1e3);
    ui->Loop->setChecked(SGp[idx].loopBursts);

    importST(SGp[idx].SpikeT);

    QCheckBox *active, *vSave;
    QDoubleSpinBox *vBias;
    actives.clear();
    vSaves.clear();
    vBiases.clear();
    ui->instTable->setRowCount(0);
    int row = 0;
    for ( vInstData const& inst : SGp[idx].inst ) {
        active = new QCheckBox();
        vSave = new QCheckBox();
        vBias = new QDoubleSpinBox();
        addInstRow(row++, active, vSave, vBias);
        active->setChecked(inst.active);
        vSave->setChecked(inst.inChn.chnlSaving);
        vBias->setValue(inst.inChn.bias * 1e3);
    }
    growInstTable(false);
}

void SpikeGenDlg::importST(std::vector<std::vector<double>> &vec)
{
    int row = 0, col = 0;
    ui->STTable->clear();
    ui->STTable->setRowCount(vec.size() + 1);
    for ( std::vector<double> &tmp : vec )
        col = qMax(int(tmp.size()), col);
    ui->STTable->setColumnCount(col + 1);

    QDoubleSpinBox *cell;
    for ( row = 0; row < ui->STTable->rowCount(); row++ ) {
        for ( col = 0; col < ui->STTable->columnCount(); col++ ) {
            cell = makeSTCell(row, col);
            if ( row < (int)vec.size() && col < (int)vec[row].size() ) {
                cell->setValue(vec[row][col] * 1e3);
            }
        }
    }
}

void SpikeGenDlg::growSTTable(int row, int col)
{
    if ( row == ui->STTable->rowCount()-1 ) {
        ui->STTable->setRowCount(row+2);
        for ( int i = 0; i < ui->STTable->columnCount(); i++ ) {
            makeSTCell(row+1, i);
        }
    }
    if ( col == ui->STTable->columnCount()-1 ) {
        ui->STTable->setColumnCount(col+2);
        for ( int i = 0; i < ui->STTable->rowCount(); i++ ) {
            makeSTCell(i, col+1);
        }
    }
}

QDoubleSpinBox *SpikeGenDlg::makeSTCell(int row, int col)
{
    QDoubleSpinBox *cell = new QDoubleSpinBox();
    cell->setRange(0., 100000.);
    cell->setSingleStep(10.);
    connect(cell, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double){
        growSTTable(row, col);
    });
    ui->STTable->setCellWidget(row, col, cell);
    return cell;
}

void SpikeGenDlg::exportData(bool)
{
    SGp[idx].LUTables = ui->SGLUTableCombo->currentIndex();
    SGp[idx].VSpike = ui->VSpike->value() / 1e3;
    SGp[idx].spkTimeScaling = 5e3 / ui->Width->value();
    SGp[idx].VRest = ui->VRest->value() / 1e3;

    SGp[idx].bdType = ui->BurstDetectionCombo->currentIndex();
    SGp[idx].bdChannel = ui->SGbdChannelCombo->currentData().value<ChannelIndex>();
    SGp[idx].bdThresh = ui->Threshold->value() / 1e3;
    SGp[idx].bdtUnder = ui->tUnder->value() / 1e3;
    SGp[idx].bdtOver = ui->tOver->value() / 1e3;

    SGp[idx].period = ui->Period->value() / 1e3;
    SGp[idx].loopBursts = ui->Loop->isChecked();

    if ( ui->STTable->rowCount() && ui->STTable->columnCount() ) // Don't export initially empty table
        exportST(SGp[idx].SpikeT);
    // Ensure any changes (sorting, dropping rows) are reflected in the UI
    importST(SGp[idx].SpikeT);

    vInstData inst;
    SGp[idx].inst.clear();
    SGp[idx].inst.reserve(ui->instTable->rowCount() - 1);
    for ( int i = 0; i < ui->instTable->rowCount() - 1; i++ ) {
        inst.active = actives[i]->isChecked();
        inst.inChn.chnlSaving = vSaves[i]->isChecked();
        inst.inChn.bias = vBiases[i]->value() * 1e-3;
        SGp[idx].inst.push_back(inst);
    }

    emit channelsChanged();
    emit modelStatusChanged();
}

void SpikeGenDlg::exportST(std::vector<std::vector<double>> &vec)
{
    int row, col, emptyRows = 0;
    std::list<double> tmp;
    vec.clear();
    vec.resize(ui->STTable->rowCount() - 1);
    for ( row = 0; row < ui->STTable->rowCount() - 1; row++ ) {
        // Populate tmp from table
        tmp.clear();
        for ( col = 0; col < ui->STTable->columnCount() - 1; col++ ) {
            tmp.push_back(qobject_cast<QDoubleSpinBox*>(ui->STTable->cellWidget(row, col))->value());
        }
        // Populate SpikeT row with ascending non-zero elements
        tmp.sort();
        bool initial = true;
        int zeros = 0;
        col = 0;
        for ( double &val : tmp ) {
            if ( val <= 0.0 ) {
                ++zeros;
                continue;
            } else if ( initial ) {
                vec[row - emptyRows].resize(tmp.size() - zeros);
                initial = false;
            }
            vec[row - emptyRows][col] = val * 1e-3;
            ++col;
        }
        if ( initial )
            ++emptyRows;
    }
    vec.resize(row - emptyRows);
}

void SpikeGenDlg::accept()
{
    exportData();
    QDialog::accept();
}

void SpikeGenDlg::reject()
{
    importData();
    QDialog::reject();
}

void SpikeGenDlg::addInstRow(int row, QCheckBox *active, QCheckBox *vSave, QDoubleSpinBox *vBias)
{
    ui->instTable->insertRow(row);
    ui->instTable->setRowHeight(row, 25);
    QStringList labels;
    for ( int i = 0; i < ui->instTable->rowCount(); i++ )
        labels << QString::number(i);
    ui->instTable->setVerticalHeaderLabels(labels);

    setCellCheckBox(row, 0, active);

    setCellCheckBox(row, 2, vSave);
    ui->instTable->setCellWidget(row, 3, vBias);
    vBias->setRange(-1000, 1000);

    actives.insert(row, active);
    vSaves.insert(row, vSave);
    vBiases.insert(row, vBias);
}

void SpikeGenDlg::setCellCheckBox(int row, int column, QCheckBox *box)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(box);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    widget->setLayout(layout);
    ui->instTable->setCellWidget(row, column, widget);
}

void SpikeGenDlg::growInstTable(bool reactive)
{
    disconnect(activec);
    disconnect(vSavec);
    disconnect(vBiasc);

    if ( reactive && !actives.empty() )
        actives.last()->setChecked(true);

    QCheckBox *active = new QCheckBox();
    QCheckBox *vSave = new QCheckBox();
    QDoubleSpinBox *vBias = new QDoubleSpinBox();

    addInstRow(ui->instTable->rowCount(), active, vSave, vBias);

    activec = connect(active, SIGNAL(stateChanged(int)), this, SLOT(growInstTable()));
    vSavec = connect(vSave, SIGNAL(stateChanged(int)), this, SLOT(growInstTable()));
    vBiasc = connect(vBias, SIGNAL(valueChanged(double)), this, SLOT(growInstTable()));
}
