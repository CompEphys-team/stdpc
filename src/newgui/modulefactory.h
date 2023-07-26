#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QStandardItemModel>
#include <functional>

class Module;

class ModuleFactory
{
public:
    static ModuleFactory& instance();

    enum ModuleType { DAQ, Tool, Synapse, Current, Other };
    static const QString& label(ModuleType);

    void registerModule(const QString& UID, const QString& displayName, ModuleType group, std::function<Module*()> createFunction);
    Module* createModule(const QString& UID);
    QStandardItemModel* getModel() const;

    class RegistrarBase
    {
    protected:
        RegistrarBase(const QString& UID, const QString& name, ModuleType group) :
            UID(UID),
            NAME(name),
            GROUP(group)
        {}
    public:
        const QString UID, NAME;
        const ModuleType GROUP;
    };

    template <class T>
    class Registrar : public RegistrarBase
    {
    public:
        Registrar(const QString& UID, const QString& name, ModuleType group) :
            RegistrarBase(UID, name, group)
        {
            ModuleFactory::instance().registerModule(UID, name, group, [](){ return new T; });
        }
    };

    static const int UIDRole = Qt::UserRole + 2;

private:
    ModuleFactory();
    ~ModuleFactory();

    QStandardItemModel model;
    QHash<QString, std::function<Module*()>> moduleCreators;
};

#endif // MODULEFACTORY_H
