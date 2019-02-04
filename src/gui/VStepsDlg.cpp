#include "VStepsDlg.h"
#include "ui_VStepsDlg.h"
#include "SpkGen.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <fstream>

VStepsDlg::VStepsDlg(size_t idx, QWidget *parent) :
    ModelDlg(idx, parent),
    ui(new Ui::VStepsDlg),
    clm(ChannelListModel::Virtual | ChannelListModel::None, this)
{
    ui->setupUi(this);
    label = ui->titleLabel->text();
    ui->titleLabel->setText(label.arg(idx));
}

VStepsDlg::~VStepsDlg()
{
    delete ui;
}

void VStepsDlg::setIndex(size_t no)
{
    ui->titleLabel->setText(label.arg(no));
    ModelDlg::setIndex(no);
}

void VStepsDlg::importData()
{
    QString num;
    num.setNum(VStepsProxy::p[idx].holdV*1e3);
    ui->holdVE->setText(num);
    num.setNum(VStepsProxy::p[idx].startV*1e3);
    ui->startVE->setText(num);
    num.setNum(VStepsProxy::p[idx].endV*1e3);
    ui->endVE->setText(num);
    num.setNum(VStepsProxy::p[idx].deltaV*1e3);
    ui->deltaVE->setText(num);
    num.setNum(VStepsProxy::p[idx].t0*1e3);
    ui->t0E->setText(num);
    num.setNum(VStepsProxy::p[idx].tStep*1e3);
    ui->tStepE->setText(num);
    num.setNum(VStepsProxy::p[idx].tHold*1e3);
    ui->tHoldE->setText(num);
}


void VStepsDlg::exportData()
{
    VStepsProxy::p[idx].holdV= ui->holdVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].startV= ui->startVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].endV= ui->endVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].deltaV= ui->deltaVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].t0= ui->t0E->text().toDouble()*1e-3;
    VStepsProxy::p[idx].tStep= ui->tStepE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].tHold= ui->tHoldE->text().toDouble()*1e-3;

    emit channelsChanged();
    emit modelStatusChanged();
}

