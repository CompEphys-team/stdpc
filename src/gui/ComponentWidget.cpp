#include "ComponentWidget.h"
#include "ui_ComponentFactoryWidget.h"
#include "ui_ComponentWidget.h"

ComponentFactoryWidget::ComponentFactoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentFactoryWidget),
    combo(ui->combo),
    button(ui->button)
{
    ui->setupUi(this);
}

ComponentFactoryWidget::~ComponentFactoryWidget()
{
    delete ui;
}


ComponentWidget::ComponentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentWidget),
    label(ui->label),
    params(ui->params),
    active(ui->active)
{
    ui->setupUi(this);
}

ComponentWidget::~ComponentWidget()
{
    delete ui;
}
