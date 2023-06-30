#ifndef MODULEREGISTRY_H
#define MODULEREGISTRY_H

#include <QStandardItemModel>

class Module;

class ModuleRegistry
{
public:
    static ModuleRegistry& instance();

    QModelIndex addModule(Module *module);
    void removeModule(Module *module);
    QStandardItemModel* getModuleModel() const;

    static const int ModPtr = Qt::UserRole + 1;
private:
    ModuleRegistry();
    ~ModuleRegistry();

    QStandardItemModel registeredModules;
};

#endif // MODULEREGISTRY_H
