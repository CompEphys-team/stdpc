#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include "modulefactory.h"

class Module : public QObject
{
    Q_OBJECT
public:
    explicit Module(QObject *parent = nullptr);
    virtual ~Module();

    virtual const QString& name() const = 0;
    virtual const QString& group() const = 0;
    virtual QWidget *createWidget(QWidget *parent) = 0;

    bool active() const;
    bool activeSettling() const;

public slots:
    void setActive(bool active);
    void setActiveSettling(bool activeSettling);

private:
    bool m_active;
    bool m_activeSettling;
};



// Mock
#include "ObjectDataTypes.h"

class MyModule : public Module
{
    Q_OBJECT
public:
    static const QString NAME;
    static const QString GROUP;

    explicit MyModule(QObject *parent = nullptr);
    ~MyModule() override;

    const QString& name() const override { return NAME; }
    const QString& group() const override { return GROUP; }
    QWidget * createWidget(QWidget *parent) override;

private:
    struct Registrar
    {
        Registrar()
        {
            ModuleFactory::instance().registerModule(NAME, GROUP,  []() { return new MyModule; });
        }
    };
    static Registrar registrar;

    struct Parameters : public CurrentData
    {
        int myParam;
    };
    Parameters p;
};

#endif // MODULE_H