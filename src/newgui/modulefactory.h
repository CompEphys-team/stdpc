#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <qstring.h>
#include <qstringlist.h>
#include <qhash.h>
#include <qstandarditemmodel.h>
#include <functional>

class Module;

class ModuleFactory
{
public:
    static ModuleFactory& instance();

    void registerModule(const QString& moduleName, const QString& group, std::function<Module*()> createFunction);
    QStringList getAvailableGroups() const;
    QStringList getAvailableModules(const QString& group) const;
    Module* createModule(const QString& moduleName);
    QStandardItemModel* getGroupModel() const;

private:
    ModuleFactory();
    ~ModuleFactory();

    QStandardItemModel groupModel;
    QHash<QString, std::function<Module*()>> moduleCreators;
};

#endif // MODULEFACTORY_H
