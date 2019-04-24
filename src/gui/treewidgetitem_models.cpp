#include "treewidgetitem_models.h"
#include "ui_treewidgetitem_models.h"

TreeWidgetItem_Models::TreeWidgetItem_Models(const QMap<QString, ConductanceProxy *> &proxies, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeWidgetItem_Models)
{
    ui->setupUi(this);

    QVector<ComponentPrototype *> prototypes;
    for ( ConductanceProxy *proxy : proxies )
        prototypes.push_back(new ComponentPrototype(proxy));
    ui->toolsTable->init(prototypes);

    connect(ui->ToolsActivate, SIGNAL(clicked(bool)), ui->toolsTable, SLOT(activateAll()));
    connect(ui->ToolsDeactivate, SIGNAL(clicked(bool)), ui->toolsTable, SLOT(deactivateAll()));
    connect(ui->ToolsClear, &QPushButton::clicked, [=](){ui->toolsTable->importData(true);});
    connect(ui->ToolsReset, SIGNAL(clicked(bool)), ui->toolsTable, SLOT(importData()));
}

TreeWidgetItem_Models::~TreeWidgetItem_Models()
{
    delete ui;
}

void TreeWidgetItem_Models::importData(bool activeOnly)
{
    ui->toolsTable->importData(activeOnly);
}

void TreeWidgetItem_Models::exportData()
{
    ui->toolsTable->exportData();
}
