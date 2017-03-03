#include "DaqWidget.h"
#include "ui_DaqFactoryWidget.h"
#include "ui_DaqWidget.h"

DaqFactoryWidget::DaqFactoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DaqFactoryWidget),
    combo(ui->combo),
    button(ui->button)
{
    ui->setupUi(this);
}

DaqFactoryWidget::~DaqFactoryWidget()
{
    delete ui;
}


DaqWidget::DaqWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DaqWidget),
    params(ui->params),
    active(ui->active)
{
    ui->setupUi(this);
}

DaqWidget::~DaqWidget()
{
    delete ui;
}

void DaqWidget::setLabel(const QString &newLabel) {
    label = newLabel;
    ui->label->setText(label);
}

void DaqWidget::statusChanged(DeviceStatus s)
{
    if ( s == DeviceStatus::Failed )
        ui->label->setText(QString("<html><head/><body><p style='font-weight:600; color:#ff0000;'>%1</p></body></html>").arg(label));
    else
        ui->label->setText(label);
}
