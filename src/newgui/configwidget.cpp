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

    // Disable category headings
    for(int row = 0; row < model->rowCount(); ++row) {
        QStandardItem *item = model->item(row);
        // setDisabled(false) leads to stray selection of the last hovered item. This leads to wonky keyboard navigation. Choose your poison.
        item->setSelectable(false);
        item->setData(ui->comboBox->palette().color(QPalette::Disabled, QPalette::Text), Qt::TextColorRole);
    }

    // Ensure consistent instructions
    ui->comboBox->setUnselectedLabel("Add new...");
    ui->comboBox->setCurrentIndex(-1);

    // Add a new Module on combobox select
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int idx){
        if (idx == -1)
            return;

        const QString& UID = ui->comboBox->currentData(ModuleFactory::UIDRole).toString();
        Module *module = ModuleFactory::instance().createModule(UID);
        QModelIndex index = ModuleRegistry::instance().addModule(module);
        if ( index.isValid() ) {
            QModelIndex parent = index.parent();
            if ( !ui->treeView->isExpanded(parent) )
                ui->treeView->expand(parent);
            ui->treeView->scrollTo(index);
            QItemSelectionModel *selection = ui->treeView->selectionModel();
            selection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            ui->treeView->setFocus();
        } else {
            delete module;
        }

        ui->comboBox->setCurrentIndex(-1);
    });

    // Set up tree widget with the ModuleRegistry contents
    QStandardItemModel *registryModel = ModuleRegistry::instance().getModuleModel();
    ui->treeView->setModel(registryModel);

    // Link tree widget selection to sidebox
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, [=](const QModelIndex &current, const QModelIndex &){
        disconnect(ui->active, SIGNAL(clicked(bool)), nullptr, nullptr);
        disconnect(ui->settling, SIGNAL(clicked(bool)), nullptr, nullptr);
        disconnect(ui->params, SIGNAL(clicked(bool)), nullptr, nullptr);

        Module *currentModule = nullptr;
        if ( current.isValid() )
            currentModule = current.data(ModuleRegistry::ModPtr).value<Module*>();
        if ( currentModule ) {
            ui->active->setEnabled(true);
            ui->active->setChecked(currentModule->active());
            connect(ui->active, &QCheckBox::clicked, currentModule, &Module::setActive);

            ui->settling->setEnabled(currentModule->canSettle());
            ui->settling->setChecked(currentModule->canSettle() && currentModule->activeSettling());
            connect(ui->settling, &QCheckBox::clicked, currentModule, &Module::setActiveSettling);

            connect(ui->params, &QPushButton::clicked, this, &ConfigWidget::openParams);
        } else {
            ui->active->setEnabled(false);
            ui->active->setChecked(false);

            ui->settling->setEnabled(false);
            ui->settling->setChecked(false);
        }
    });

    // Remove registered modules
    connect(ui->remove, &QPushButton::clicked, this, [=](bool){
        QItemSelectionModel *selectionModel = ui->treeView->selectionModel();
        QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

        // Sort the selected indexes in reverse order
        std::sort(selectedIndexes.begin(), selectedIndexes.end(),
                  [](const QModelIndex& index1, const QModelIndex& index2) {
                      return index1.row() > index2.row();
                  });

        // Remove the selected items from the model
        for (const QModelIndex& index : qAsConst(selectedIndexes)) {
            QStandardItem* item = registryModel->itemFromIndex(index);
            if (item) {
                QStandardItem* parentItem = item->parent();
                if (parentItem) {
                    parentItem->removeRow(item->row());
                } else {
                   registryModel->removeRow(item->row());
                }
            }
        }
    });
}

ConfigWidget::~ConfigWidget()
{
    delete ui;
}

void ConfigWidget::openParams()
{
    QItemSelectionModel *selectionModel = ui->treeView->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    if ( selectedIndexes.length() != 1 )
        return;
    QModelIndex &index = selectedIndexes.first();
    Module *module = index.data(ModuleRegistry::ModPtr).value<Module*>();
    QWidget *dialog = module->createWidget(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
