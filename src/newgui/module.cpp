#include "module.h"

Module::Module(QObject *parent) : QObject(parent)
{

}

Module::~Module()
{

}

bool Module::active() const
{
    return m_active;
}

void Module::setActive(bool active)
{
    m_active = active;
}

bool Module::activeSettling() const
{
    return m_activeSettling;
}

void Module::setActiveSettling(bool activeSettling)
{
    m_activeSettling = activeSettling;
}


const QString MyModule::NAME = "MyModule";
const QString MyModule::GROUP = "MyGroup";
const ModuleFactory::Registrar<MyModule> MyModule::REG(MyModule::NAME, MyModule::GROUP);

MyModule::MyModule(QObject *parent)
{

}

MyModule::~MyModule()
{

}

QWidget *MyModule::createWidget(QWidget *parent)
{
    return nullptr;
}
