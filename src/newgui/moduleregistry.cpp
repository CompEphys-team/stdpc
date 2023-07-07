#include "moduleregistry.h"
#include "module.h"

ModuleRegistry::ModuleRegistry()
{
}

ModuleRegistry::~ModuleRegistry()
{
    QStandardItem *groupItem, *item;
    Module *module;
    for (int i = 0; i < registeredModules.rowCount(); ++i)
    {
        groupItem = registeredModules.item(i);
        for (int j = 0; j < groupItem->rowCount(); ++j)
        {
            item = groupItem->child(i);
            module = item->data().value<Module*>();
            delete module;
        }
    }
    registeredModules.clear();
}

ModuleRegistry& ModuleRegistry::instance()
{
    static ModuleRegistry registry;
    return registry;
}

QModelIndex ModuleRegistry::addModule(Module *module)
{
    if (!module)
        return QModelIndex();

    QString group = module->group();

    // Find the group item
    QStandardItem* groupItem = nullptr;
    for (int i = 0; i < registeredModules.rowCount(); ++i)
    {
        QStandardItem* item = registeredModules.item(i);
        if (item->text() == group)
        {
            groupItem = item;
            break;
        }
    }

    // Create and append the group item, if none exists yet
    if (!groupItem)
    {
        groupItem = new QStandardItem(group);
        groupItem->setFlags(groupItem->flags() & ~Qt::ItemIsEnabled);
        registeredModules.appendRow(groupItem);
        registeredModules.sort(0);
    }

    // Create the new item
    QStandardItem* newItem = new QStandardItem(module->name());
    newItem->setData(QVariant::fromValue(module), ModPtr);
    groupItem->appendRow(newItem);
    groupItem->sortChildren(0);

    return registeredModules.indexFromItem(newItem);
}

void ModuleRegistry::removeModule(Module *module)
{
    if (!module)
        return;

    QString group = module->group();

    // Find the group item
    QStandardItem* groupItem = nullptr;
    for (int i = 0; i < registeredModules.rowCount(); ++i)
    {
        QStandardItem* item = registeredModules.item(i);
        if (item->text() == group)
        {
            groupItem = item;
            break;
        }
    }

    if (!groupItem)
        return;

    for (int i = 0; i < groupItem->rowCount(); ++i)
    {
        QStandardItem* item = groupItem->child(i);
        if (item->data(ModPtr).value<Module*>() == module)
        {
            groupItem->removeRow(i);
            delete module;
            break;
        }
    }
}

QStandardItemModel *ModuleRegistry::getModuleModel() const
{
    return const_cast<QStandardItemModel*>(&registeredModules);
}

