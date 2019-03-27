#include "SynapticNoiseDlg.h"
#include "ui_SynapticNoiseDlg.h"
#include "SynapticNoise.h"

SynapticNoiseDlg::SynapticNoiseDlg(size_t idx, QWidget *parent) :
    ConductanceDlg(idx, parent),
    ui(new Ui::SynapticNoiseDlg)
{
    ui->setupUi(this);
    setIndex(idx);
    
    QVector<AssignmentCellBase<CurrentAssignment>*> vec;
    vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::active, "Active", 47));
    vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::VChannel, "V in", 165,
       ChannelListModel::getModel(ChannelListModel::AnalogIn | ChannelListModel::Prototype | ChannelListModel::Virtual | ChannelListModel::Blank)));
    vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::IChannel, "I out", 165,
       ChannelListModel::getModel(ChannelListModel::AnalogOut | ChannelListModel::Prototype | ChannelListModel::Virtual | ChannelListModel::Blank)));
    vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::save, "Save", 30));
    ui->assignments->init(vec);
}

SynapticNoiseDlg::~SynapticNoiseDlg()
{
    delete ui;
}

void SynapticNoiseDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    ui->DlgLabel->setText(QString("%1 %2").arg(SynapticNoiseProxy::get()->prettyName()).arg(no));
}

void SynapticNoiseDlg::importData()
{
    SynapticNoiseData &p = SynapticNoiseProxy::p[idx];
    ui->leLabel->setText(p.label);
    ui->assignments->importData(p.assign);
    ui->Vrev->setValue(p.Vrev * 1e3);
    ui->tau->setValue(p.tau * 1e3);
    ui->g0->setValue(p.g0 * 1e9);
    ui->stddev->setValue(std::sqrt(p.D * p.tau / 2e-18));
    ui->ignoreNegative->setChecked(p.ignoreNegative);
}

void SynapticNoiseDlg::exportData()
{
    SynapticNoiseData &p = SynapticNoiseProxy::p[idx];
    p.label = ui->leLabel->text();
    ui->assignments->exportData(p.assign);
    p.Vrev = ui->Vrev->value() * 1e-3;
    p.tau = ui->tau->value() * 1e-3;
    p.g0 = ui->g0->value() * 1e-9;
    p.D = 2e-18 * ui->stddev->value()*ui->stddev->value() / p.tau; // nS^2 / s = 10^-18 S^2/s (variance = D*tau/2)
    p.ignoreNegative = ui->ignoreNegative->isChecked();
}

void SynapticNoiseDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( button->text() == "Close" )
        hide();
    emit labelChanged(ui->leLabel->text());
}
