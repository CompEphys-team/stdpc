#ifndef DAQWIDGET_H
#define DAQWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include "DeviceManager.h"

namespace Ui {
class DaqFactoryWidget;
class DaqWidget;
}

class DaqFactoryWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::DaqFactoryWidget *ui;

public:
    explicit DaqFactoryWidget(QWidget *parent = nullptr);
    ~DaqFactoryWidget();

    QComboBox *&combo;
    QPushButton *&button;
};

class DaqWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::DaqWidget *ui;
    QString label;

public:
    explicit DaqWidget(QWidget *parent = nullptr);
    ~DaqWidget();

    QPushButton *&params;
    QCheckBox *&active;

public slots:
    void setLabel(const QString &newLabel);
    void statusChanged(DeviceStatus s);
};

#endif // DAQWIDGET_H
