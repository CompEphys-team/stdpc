#ifndef COMPONENTFACTORYWIDGET_H
#define COMPONENTFACTORYWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

namespace Ui {
class ComponentFactoryWidget;
class ComponentWidget;
}

class ComponentFactoryWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::ComponentFactoryWidget *ui;

public:
    explicit ComponentFactoryWidget(QWidget *parent = 0);
    ~ComponentFactoryWidget();

    QComboBox *&combo;
    QPushButton *&button;
};

class ComponentWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::ComponentWidget *ui;

public:
    explicit ComponentWidget(QWidget *parent = 0);
    ~ComponentWidget();

    QLabel *&label;
    QPushButton *&params;
    QCheckBox *&active;
};

#endif // COMPONENTFACTORYWIDGET_H
