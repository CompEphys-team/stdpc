#include "SpikeGenDlg.h"
#include "ui_SpikeGenDlg.h"
#include "SpkGen.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <fstream>

SpikeGenDlg::SpikeGenDlg(size_t idx, QWidget *parent) :
    ModelDlg(idx, parent),
    ui(new Ui::SpikeGenDlg),
    clm(ChannelListModel::getModel(ChannelListModel::AnalogIn | ChannelListModel::Virtual | ChannelListModel::None))
{
    ui->setupUi(this);
    setIndex(idx);
    ui->instTable->setHorizontalHeaderLabels({"Active",
                                              "",
                                              "Save",
                                              "Bias (mV)",
                                              "",
                                              "Burst detect\nchannel",
                                              "Burst detect\nthreshold (mV)"
                                             });
    ui->instTable->setColumnWidth(0, 40);
    ui->instTable->setColumnWidth(1, 10);
    ui->instTable->setColumnWidth(2, 60);
    ui->instTable->setColumnWidth(3, 80);
    ui->instTable->setColumnWidth(4, 10);
    ui->instTable->setColumnWidth(5, 80);
    ui->instTable->setColumnWidth(6, 80);
    growInstTable(false);

    ui->STTable->setModel(&model);
    ui->STTable->setItemDelegate(&delegate);
    connect(&model, &QStandardItemModel::dataChanged, [this](QModelIndex,QModelIndex index,QVector<int>){
        if ( !model.data(index).isValid() )
            return;
        if ( index.row() == model.rowCount()-1 )
            model.setRowCount(model.rowCount()+1);
        if ( index.column() == model.columnCount()-1 )
            model.setColumnCount(model.columnCount()+1);
    });

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
        os.precision(10);
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
        ui->Period->setEnabled(i == 0);
    });
    connect(ui->tOContiguous, &QCheckBox::stateChanged, [this](int){
        ui->bdStrict->setEnabled(ui->tOContiguous->isChecked() && ui->tUContiguous->isChecked());
    });
    connect(ui->tUContiguous, &QCheckBox::stateChanged, [this](int){
        ui->bdStrict->setEnabled(ui->tOContiguous->isChecked() && ui->tUContiguous->isChecked());
    });

    std::vector<std::vector<double>> defaults(1, {.03,.05,.068,.088,.112,.138,.168,.204,.248,.308});
    importST(defaults);
}

SpikeGenDlg::~SpikeGenDlg()
{
    delete ui;
}

void SpikeGenDlg::setIndex(size_t no)
{
    ui->titleLabel->setText(QString("%1 %2").arg(SpkGenProxy::get()->prettyName()).arg(no));
    ModelDlg::setIndex(no);
}

void SpikeGenDlg::importData()
{
    ui->leLabel->setText(SpkGenProxy::p[idx].label);

    ui->SGLUTableCombo->setCurrentIndex(SpkGenProxy::p[idx].LUTables);
    ui->VSpike->setValue(SpkGenProxy::p[idx].VSpike * 1e3);
    ui->Width->setValue(5e3 / SpkGenProxy::p[idx].spkTimeScaling);
    ui->VRest->setValue(SpkGenProxy::p[idx].VRest * 1e3);

    ui->BurstDetectionCombo->setCurrentIndex(SpkGenProxy::p[idx].bdType);
    ui->tUnder->setValue(SpkGenProxy::p[idx].bdtUnder * 1e3);
    ui->tOver->setValue(SpkGenProxy::p[idx].bdtOver * 1e3);
    ui->tUContiguous->setChecked(SpkGenProxy::p[idx].bdtUnderCont);
    ui->tOContiguous->setChecked(SpkGenProxy::p[idx].bdtOverCont);
    ui->bdStrict->setChecked(SpkGenProxy::p[idx].bdStrictlyCont);

    ui->Period->setValue(SpkGenProxy::p[idx].period * 1e3);
    ui->Loop->setChecked(SpkGenProxy::p[idx].loopBursts);

    importST(SpkGenProxy::p[idx].SpikeT);

    QCheckBox *active, *vSave;
    QDoubleSpinBox *vBias, *bdThreshold;
    WideComboBox *bdChannel;
    actives.clear();
    vSaves.clear();
    vBiases.clear();
    bdChannels.clear();
    bdThresholds.clear();
    ui->instTable->setRowCount(0);
    int row = 0;
    for ( SgInstData const& inst : SpkGenProxy::p[idx].inst ) {
        active = new QCheckBox();
        vSave = new QCheckBox();
        vBias = new QDoubleSpinBox();
        bdChannel = new WideComboBox();
        bdThreshold = new QDoubleSpinBox();
        addInstRow(row++, active, vSave, vBias, bdChannel, bdThreshold);
        active->setChecked(inst.active);
        vSave->setChecked(inst.inChn.chnlSaving);
        vBias->setValue(inst.inChn.bias * 1e3);
        bdChannel->setCurrentIndex(clm->index(inst.bdChannel));
        bdThreshold->setValue(inst.bdThresh * 1e3);
    }
    growInstTable(false);
}

void SpikeGenDlg::importST(std::vector<std::vector<double>> &vec)
{
    model.clear();
    model.setRowCount(vec.size() + 1);
    for ( size_t row = 0; row < vec.size(); row++ ) {
        if ( model.columnCount() < int(vec[row].size()+1) )
            model.setColumnCount(vec[row].size()+1);
        for ( size_t col = 0; col < vec[row].size(); col++ )
            model.setData(model.index(row, col), QVariant(vec[row][col] * 1e3));
    }
}

void SpikeGenDlg::exportData()
{
    SpkGenProxy::p[idx].label = ui->leLabel->text();

    SpkGenProxy::p[idx].LUTables = ui->SGLUTableCombo->currentIndex();
    SpkGenProxy::p[idx].VSpike = ui->VSpike->value() / 1e3;
    SpkGenProxy::p[idx].spkTimeScaling = 5e3 / ui->Width->value();
    SpkGenProxy::p[idx].VRest = ui->VRest->value() / 1e3;

    SpkGenProxy::p[idx].bdType = ui->BurstDetectionCombo->currentIndex();
    SpkGenProxy::p[idx].bdtUnder = ui->tUnder->value() / 1e3;
    SpkGenProxy::p[idx].bdtOver = ui->tOver->value() / 1e3;
    SpkGenProxy::p[idx].bdtUnderCont = ui->tUContiguous->isChecked();
    SpkGenProxy::p[idx].bdtOverCont = ui->tOContiguous->isChecked();
    SpkGenProxy::p[idx].bdStrictlyCont = ui->bdStrict->isChecked();

    SpkGenProxy::p[idx].period = ui->Period->value() / 1e3;
    SpkGenProxy::p[idx].loopBursts = ui->Loop->isChecked();

    exportST(SpkGenProxy::p[idx].SpikeT);

    SgInstData inst;
    int nRows = ui->instTable->rowCount() - 1;
    SpkGenProxy::p[idx].inst.clear();
    SpkGenProxy::p[idx].inst.reserve(nRows);
    for ( int i = 0; i < nRows; i++ ) {
        inst.active = actives[i]->isChecked();
        inst.inChn.chnlSaving = vSaves[i]->isChecked();
        inst.inChn.bias = vBiases[i]->value() * 1e-3;
        inst.bdChannel = bdChannels[i]->currentData().value<ChannelIndex>();
        inst.bdThresh = bdThresholds[i]->value() * 1e-3;
        SpkGenProxy::p[idx].inst.push_back(inst);
    }

    emit channelsChanged();
    emit modelStatusChanged();
}

void SpikeGenDlg::exportST(std::vector<std::vector<double>> &vec)
{
    vec.clear();
    for ( int row = 0; row < model.rowCount() - 1; row++ ) {
        // Populate tmp from model
        std::vector<double> tmp;
        for ( int col = 0; col < model.columnCount() - 1; col++ ) {
            QVariant value = model.data(model.index(row, col));
            if ( value.isValid() )
                tmp.push_back(value.toDouble() * 1e-3);
        }
        if ( !tmp.empty() ) {
            // Populate SpikeT row with ascending non-zero elements
            std::sort(tmp.begin(), tmp.end());
            vec.push_back(tmp);
        }
    }

    importST(vec); // Ensure changes (sorting, dropped values) are reflected in the model
}

void SpikeGenDlg::addInstRow(int row, QCheckBox *active, QCheckBox *vSave, QDoubleSpinBox *vBias,
                             WideComboBox *bdChannel, QDoubleSpinBox *bdThresh)
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

    ui->instTable->setCellWidget(row, 5, bdChannel);
    bdChannel->setModel(clm);
    ui->instTable->setCellWidget(row, 6, bdThresh);
    bdThresh->setRange(-1000, 1000);

    actives.insert(row, active);
    vSaves.insert(row, vSave);
    vBiases.insert(row, vBias);
    bdChannels.insert(row, bdChannel);
    bdThresholds.insert(row, bdThresh);
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
    disconnect(bdChannelc);
    disconnect(bdThresholdc);

    if ( reactive && !actives.empty() )
        actives.last()->setChecked(true);

    QCheckBox *active = new QCheckBox();
    QCheckBox *vSave = new QCheckBox();
    QDoubleSpinBox *vBias = new QDoubleSpinBox();
    WideComboBox *bdChannel = new WideComboBox();
    QDoubleSpinBox *bdThresh = new QDoubleSpinBox();

    addInstRow(ui->instTable->rowCount(), active, vSave, vBias, bdChannel, bdThresh);

    activec = connect(active, SIGNAL(stateChanged(int)), this, SLOT(growInstTable()));
    vSavec = connect(vSave, SIGNAL(stateChanged(int)), this, SLOT(growInstTable()));
    vBiasc = connect(vBias, SIGNAL(valueChanged(double)), this, SLOT(growInstTable()));
    bdChannelc = connect(bdChannel, SIGNAL(currentIndexChanged(int)), this, SLOT(growInstTable()));
    bdThresholdc = connect(bdThresh, SIGNAL(valueChanged(double)), this, SLOT(growInstTable()));
}



QWidget *STDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setDecimals(6);
    editor->setMinimum(0);
    editor->setMaximum(999999.999999);
    editor->setFrame(false);
    return editor;
}

void STDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(index.model()->data(index, Qt::EditRole).toDouble());
}

void STDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    if ( value > 0 || index.column() == 0 )
        model->setData(index, value, Qt::EditRole);
    else
        model->setData(index, QVariant(), Qt::EditRole);
}

void STDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
