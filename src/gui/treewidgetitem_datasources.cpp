#include "treewidgetitem_datasources.h"
#include "ui_treewidgetitem_datasources.h"
#include "DaqOpts.h"
#include "ModelOpts.h"

TreeWidgetItem_DataSources::TreeWidgetItem_DataSources(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeWidgetItem_DataSources)
{
    ui->setupUi(this);

    QVector<DaqOptsPrototypeBase*> prototypes;
    for ( DAQProxy *proxy : DeviceManager::Register() )
        prototypes.push_back(new DaqOptsPrototype(proxy));
    for ( ModelProxy *proxy : ModelManager::Register() )
        prototypes.push_back(new ModelOptsPrototype(proxy));
    ui->DAQTable->init(prototypes, parent);

    connect(ui->DAQClear, &QPushButton::clicked, [=](){ui->DAQTable->importData(true);});
}

TreeWidgetItem_DataSources::~TreeWidgetItem_DataSources()
{
    delete ui;
}

void TreeWidgetItem_DataSources::importData(bool activeOnly)
{
    ui->DAQTable->importData(activeOnly);
}

void TreeWidgetItem_DataSources::exportData(bool ignoreDAQ)
{
    ui->DAQTable->exportData(ignoreDAQ);
}
