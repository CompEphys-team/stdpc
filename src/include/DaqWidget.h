/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
    explicit DaqFactoryWidget(QWidget *parent = 0);
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
    explicit DaqWidget(QWidget *parent = 0);
    ~DaqWidget();

    QPushButton *&params;
    QCheckBox *&active;

public slots:
    void setLabel(const QString &newLabel);
    void statusChanged(DeviceStatus s);
};

#endif // DAQWIDGET_H
