#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include "modulefactory.h"
#include "parameters.h"

class Module : public QObject
{
    Q_OBJECT
public:
    explicit Module(QObject *parent = nullptr);
    virtual ~Module();

    const QString& uid() const { return registrar().UID; }
    const QString& name() const { return registrar().NAME; }
    const QString& group() const { return ModuleFactory::label(registrar().GROUP); }

    virtual QWidget *createWidget(QWidget *parent) = 0;

    virtual bool active() const = 0;
    virtual inline bool activeSettling() const { return false; };
    virtual inline bool canSettle() const { return false; }

public slots:
    virtual void setActive(bool active) = 0;
    virtual inline void setActiveSettling(bool /*activeSettling*/) { /*no-op*/ };

protected:
    virtual const ModuleFactory::RegistrarBase& registrar() const = 0;
};


// Mock

PARAMETERS_DECL(Child, Parameters)
    double value;
};
PARAMETERS_DECL(MyParameters, Parameters)
    bool active;
    bool activeSettling;
    int myParam;
    QString myString;
    Child child;
    std::vector<Child> children;
};
class MyModule : public Module
{
    Q_OBJECT
public:
    explicit MyModule(QObject *parent = nullptr);
    ~MyModule() override;

    QWidget * createWidget(QWidget *parent) override;

    inline bool active() const override { return p.active; }
    inline bool activeSettling() const override { return p.activeSettling; }
    inline bool canSettle() const override { return true; }

public slots:
    inline void setActive(bool active) override { p.active = active; }
    inline void setActiveSettling(bool active) override { p.activeSettling = active; }

protected:
    static const ModuleFactory::Registrar<MyModule> REG;
    const ModuleFactory::RegistrarBase& registrar() const override { return REG; }

private:
    PARAMETERS_DECL(Derived, MyParameters)
        QString addendum;
    };

    Derived p;
};

#endif // MODULE_H
