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
        cerr << "exportData start" << endl;
    VStepsProxy::p[idx].holdV= ui->holdVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].startV= ui->startVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].endV= ui->endVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].deltaV= ui->deltaVE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].t0= ui->t0E->text().toDouble()*1e-3;
    VStepsProxy::p[idx].tStep= ui->tStepE->text().toDouble()*1e-3;
    VStepsProxy::p[idx].tHold= ui->tHoldE->text().toDouble()*1e-3;
    // automatically create one instance
    vInstData inst;
    VStepsProxy::p[idx].inst.clear();
    VStepsProxy::p[idx].inst.reserve(1);
    inst.active = 1;

    // outChn is the input to the model - this is not used.
    inst.outChn.active = false;

    // inChn is the output from the model; save by default
    // Other channel properties are not used by the model.
    inst.inChn.active = true;
    inst.inChn.chnlSaving = true;

    VStepsProxy::p[idx].inst.push_back(inst);

    emit channelsChanged();
    emit modelStatusChanged();
}

