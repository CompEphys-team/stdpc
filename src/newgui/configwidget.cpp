#include "configwidget.h"
#include "ui_configwidget.h"
#include "modulefactory.h"
#include "moduleregistry.h"
#include "module.h"

ConfigWidget::ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWidget)
{
    ui->setupUi(this);

    // Set up combobox with the ModuleFactory hierarchy
    QTreeView *availableModulesView = new QTreeView(ui->comboBox);
    QStandardItemModel* model = ModuleFactory::instance().getGroupModel();
    ui->comboBox->setView(availableModulesView);
    ui->comboBox->setModel(model);
    availableModulesView->header()->hide();
    availableModulesView->expandAll();

    // Stick the combobox instructions in the top. Is there a cleaner way to do this?
    QStandardItem *addnew = new QStandardItem("Add new...");
    model->insertRow(0, addnew);
    for(int row = 0; row < model->rowCount(); ++row) {
        QStandardItem *item = model->item(row);
        item->setEnabled(false);
    }
    ui->comboBox->setCurrentIndex(0);

    // Add a new Module on combobox select
    connect(ui->comboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), [=](const QString &text){
        if (text == addnew->text())
            return;

        Module *module = ModuleFactory::instance().createModule(text);
        QModelIndex index = ModuleRegistry::instance().addModule(module);
        if ( index.isValid() ) {
            QModelIndex parent = index.parent();
            if ( !ui->treeView->isExpanded(parent) )
                ui->treeView->expand(parent);
            ui->treeView->scrollTo(index);
            QItemSelectionModel *selection = ui->treeView->selectionModel();
            selection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            ui->treeView->setFocus();
        }

        ui->comboBox->setCurrentIndex(0);
    });

    // Set up tree widget with the ModuleRegistry contents
    QStandardItemModel *registryModel = ModuleRegistry::instance().getModuleModel();
    ui->treeView->setModel(registryModel);

    // Link tree widget selection to sidebox
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, [=](const QModelIndex &current, const QModelIndex &previous){
        if ( previous.isValid() ) {
            Module *prevModule = previous.data(ModuleRegistry::ModPtr).value<Module*>();
            if ( prevModule ) {
                disconnect(ui->active, &QCheckBox::toggled, prevModule, &Module::setActive);
                disconnect(ui->settling, &QCheckBox::toggled, prevModule, &Module::setActiveSettling);
            }
        }

        if ( current.isValid() ) {
            Module *currentModule = current.data(ModuleRegistry::ModPtr).value<Module*>();
            if ( currentModule ) {
                ui->active->setChecked(currentModule->active());
                ui->settling->setChecked(currentModule->activeSettling());
                connect(ui->active, &QCheckBox::toggled, currentModule, &Module::setActive);
                connect(ui->settling, &QCheckBox::toggled, currentModule, &Module::setActiveSettling);
            }
        } else {
            ui->active->setChecked(false);
            ui->settling->setChecked(false);
        }
    });
}

ConfigWidget::~ConfigWidget()
{
    delete ui;
}
