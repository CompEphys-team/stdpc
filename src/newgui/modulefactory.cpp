#include "modulefactory.h"
#include <stdexcept>

ModuleFactory::ModuleFactory()
{
    for (ModuleType type : {DAQ, Tool, Synapse, Current})
        groupModel.appendRow(new QStandardItem(label(type)));
}

ModuleFactory::~ModuleFactory()
{
}

const QString& ModuleFactory::label(ModuleType type)
{
    static const QMap<ModuleType, QString> map = {
        {DAQ, "Data acquisition"},
        {Tool, "Tools"},
        {Synapse, "Synapses"},
        {Current, "Ionic currents"}
    };
    static const QString unknown = "Other";
    auto it = map.constFind(type);
    return it != map.constEnd() ? it.value() : unknown;
}

ModuleFactory& ModuleFactory::instance()
{
    static ModuleFactory factory;
    return factory;
}

void ModuleFactory::registerModule(const QString &UID, const QString& displayName, ModuleType group, std::function<Module*()> createFunction)
{
    if ( moduleCreators.contains(UID) )
        throw std::runtime_error(QString("Module UID %1 has already been registered.").arg(UID).toStdString());
    moduleCreators.insert(UID, createFunction);

    QStandardItem* groupItem = nullptr;
    QString groupLabel = label(group);
    for (int i = 0; i < groupModel.rowCount(); ++i)
    {
        QStandardItem* item = groupModel.item(i);
        if (item->text() == groupLabel)
        {
            groupItem = item;
            break;
        }
    }

    if (!groupItem)
    {
        groupItem = new QStandardItem(groupLabel);
        groupModel.appendRow(groupItem);
    }

    QStandardItem* moduleItem = new QStandardItem(displayName);
    moduleItem->setData(UID, UIDRole);
    groupItem->appendRow(moduleItem);
}

Module* ModuleFactory::createModule(const QString& UID)
{
    std::function<Module*()> createFunction = moduleCreators.value(UID, nullptr);
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
