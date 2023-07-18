#include "module.h"
#include "qdebug.h"

Module::Module(QObject *parent) : QObject(parent)
{

}

Module::~Module()
{

}


const ModuleFactory::Registrar<MyModule> MyModule::REG("MyMod", "My Module", "My Group");

MyModule::MyModule(QObject *parent)
{
    std::vector<QStringList> paths = {
        {"myString"},
        {"addendum"},
        {"myParam"},
        {"child", "value"},
        {"children", "0", "value"},
        {"children", "1", "value"},
    };
    std::vector<QVariant> values = {
        "asdf",
        "summarum",
        3,
        1.2,
        3.4,
        5.6
    };

    for ( int i = 0; i < paths.size(); ++i ) {
        const QStringList& path = paths[i];
        const QVariant& value = values[i];
        QString retval = p.read(path, value);
        qDebug() << retval << '\t' << paths[i] << '\t' << values[i];
    }

    p.write(std::cout, "p.");
    Derived foo = p;
    foo.write(std::cout, "foo.");
}

MyModule::~MyModule()
{

}

QWidget *MyModule::createWidget(QWidget *parent)
{
    return nullptr;
}

PARAMETERS_IMPL(MyParameters)
{
    PARAMETER(MyParameters, myString)
    PARAMETER(MyParameters, child)
    PARAMETER(MyParameters, children)
}

PARAMETERS_IMPL(Child)
{
    PARAMETER(Child, value)
}

PARAMETERS_IMPL_INSCOPE(Derived, MyModule)
{
    PARAMETER(Derived, addendum)
    PARAMETER(Derived, myParam)
}
