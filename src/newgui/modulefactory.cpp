#include "modulefactory.h"

ModuleFactory::ModuleFactory()
{
    for (const QString& name : {"Data acquisition", "Tools", "Synapses", "Ionic currents"})
        groupModel.appendRow(new QStandardItem(name));
}

ModuleFactory::~ModuleFactory()
{
}

ModuleFactory& ModuleFactory::instance()
{
    static ModuleFactory factory;
    return factory;
}

void ModuleFactory::registerModule(const QString& moduleName, const QString& group, std::function<Module*()> createFunction)
{
    moduleCreators.insert(moduleName, createFunction);

    QStandardItem* groupItem = nullptr;
    for (int i = 0; i < groupModel.rowCount(); ++i)
    {
        QStandardItem* item = groupModel.item(i);
        if (item->text() == group)
        {
            groupItem = item;
            break;
        }
    }

    if (!groupItem)
    {
        groupItem = new QStandardItem(group);
        groupModel.appendRow(groupItem);
    }

    QStandardItem* moduleItem = new QStandardItem(moduleName);
    groupItem->appendRow(moduleItem);
}

QStringList ModuleFactory::getAvailableGroups() const
{
    QStringList groupList;
    for (int i = 0; i < groupModel.rowCount(); ++i)
    {
        QStandardItem* groupItem = groupModel.item(i);
        groupList.append(groupItem->text());
    }
    return groupList;
}

QStringList ModuleFactory::getAvailableModules(const QString& group) const
{
    QStringList moduleList;
    for (int i = 0; i < groupModel.rowCount(); ++i)
    {
        QStandardItem* groupItem = groupModel.item(i);
        if (groupItem->text() == group)
        {
            for (int j = 0; j < groupItem->rowCount(); ++j)
            {
                QStandardItem* moduleItem = groupItem->child(j);
                moduleList.append(moduleItem->text());
            }
            break;
        }
    }
    return moduleList;
}

Module* ModuleFactory::createModule(const QString& moduleName)
{
    std::function<Module*()> createFunction = moduleCreators.value(moduleName, nullptr);
    if (createFunction)
    {
        return createFunction();
    }
    return nullptr;
}

QStandardItemModel* ModuleFactory::getGroupModel() const
{
    return const_cast<QStandardItemModel*>(&groupModel);
}
