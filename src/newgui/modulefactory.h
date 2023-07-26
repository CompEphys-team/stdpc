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

    void registerModule(const QString& UID, const QString& displayName, const QString& group, std::function<Module*()> createFunction);
    QStringList getAvailableGroups() const;
    QStringList getAvailableModules(const QString& group) const;
    Module* createModule(const QString& UID);
    QStandardItemModel* getGroupModel() const;

    class RegistrarBase
    {
    protected:
        RegistrarBase(const QString& UID, const QString& name, const QString& group) :
            UID(UID),
            NAME(name),
            GROUP(group)
        {}
    public:
        const QString UID, NAME, GROUP;
    };

    template <class T>
    class Registrar : public RegistrarBase
    {
    public:
        Registrar(const QString& UID, const QString& name, const QString& group) :
            RegistrarBase(UID, name, group)
        {
            ModuleFactory::instance().registerModule(UID, name, group, [](){ return new T; });
        }
    };

    static const int UIDRole = Qt::UserRole + 2;

private:
    ModuleFactory();
    ~ModuleFactory();

    QStandardItemModel groupModel;
    QHash<QString, std::function<Module*()>> moduleCreators;
};

#endif // MODULEFACTORY_H
