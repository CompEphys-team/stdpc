/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
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

#ifndef ASSIGNMENTCELL_H
#define ASSIGNMENTCELL_H

#include "ChannelListModel.h"
#include "ChannelIndex.h"
#include "WideComboBox.h"
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSignalBlocker>

/// QObject - to allow signalling. Do not use.
class AssignmentCellQBase : public QObject
{
    Q_OBJECT
public:
    virtual ~AssignmentCellQBase() {}
signals:
    void grow();
};

/// Base class - use for polymorphic access
template <class Assignment>
class AssignmentCellBase : public AssignmentCellQBase
{
public:
    AssignmentCellBase(const QString &l, int w) : label(l), columnWidth(w) {}
    virtual ~AssignmentCellBase() {}

    virtual void importData(int row, const Assignment &from) = 0;
    virtual void exportData(int row, Assignment &to) const = 0;

    virtual void clear() = 0;

    virtual QWidget *extend(int row) = 0;

    QString label;
    int columnWidth;

protected:
    QMetaObject::Connection connection;
};

/// Use for double fields
template <class A>
class AssignmentCellDouble : public AssignmentCellBase<A>
{
public:
    AssignmentCellDouble(double A::*m, const QString &l, int w) :
        AssignmentCellBase<A>(l, w), member(m), rangeMin(0.), rangeMax(100.), decimals(2), fac(1.)
    {}

    void setRange(double min, double max) { rangeMin = min; rangeMax = max; }
    void setDecimals(int d) { decimals = d; }
    void setFactor(double factor) { fac = factor; }

    void importData(int row, const A &from) { QSignalBlocker b(this); widgets[row]->setValue(from.*member / fac); }
    void exportData(int row, A &to) const { to.*member = widgets[row]->value() * fac; }
    void clear() { widgets.clear(); }

    QWidget *extend(int row)
    {
        QDoubleSpinBox *tmp = new QDoubleSpinBox();
        tmp->setRange(rangeMin, rangeMax);
        tmp->setDecimals(decimals);
        if ( row == widgets.size() ) {
            QObject::disconnect(this->connection);
            this->connection = QObject::connect(tmp, SIGNAL(valueChanged(double)), this, SIGNAL(grow()));
        }
        widgets.insert(row, tmp);
        return tmp;
    }

private:
    double A::* member;
    QVector<QDoubleSpinBox *> widgets;
    double rangeMin, rangeMax;
    int decimals;
    double fac;
};

/// Use for channel dropdowns
template <class A>
class AssignmentCellChannel : public AssignmentCellBase<A>
{
public:
    AssignmentCellChannel(ChannelIndex A::* m, const QString &l, int w, ChannelListModel *model) :
        AssignmentCellBase<A>(l, w), member(m), model(model)
    {}

    void importData(int row, const A &from) { QSignalBlocker b(this); widgets[row]->setCurrentIndex(model->index(from.*member)); }
    void exportData(int row, A &to) const { to.*member = widgets[row]->currentData().value<ChannelIndex>(); }
    void clear() { widgets.clear(); }

    QWidget *extend(int row)
    {
        WideComboBox *tmp = new WideComboBox();
        tmp->setModel(model);
        if ( row == widgets.size() ) {
            QObject::disconnect(this->connection);
            this->connection = QObject::connect(tmp, SIGNAL(currentIndexChanged(int)), this, SIGNAL(grow()));
        }
        widgets.insert(row, tmp);
        return tmp;
    }

private:
    ChannelIndex A::* member;
    ChannelListModel *model;
    QVector<WideComboBox *> widgets;
};

/// Use for boolean checkboxes
template <class A>
class AssignmentCellBool : public AssignmentCellBase<A>
{
public:
    AssignmentCellBool(bool A::* m, const QString &l, int w) :
        AssignmentCellBase<A>(l, w), member(m)
    {}

    void importData(int row, const A &from) { QSignalBlocker b(this); widgets[row]->setChecked(from.*member); }
    void exportData(int row, A &to) const { to.*member = widgets[row]->isChecked(); }
    void clear() { widgets.clear(); }

    QWidget *extend(int row)
    {
        QCheckBox *box = new QCheckBox();
        if ( row == widgets.size() ) {
            QObject::disconnect(this->connection);
            this->connection = QObject::connect(box, SIGNAL(stateChanged(int)), this, SIGNAL(grow()));
        }
        widgets.insert(row, box);

        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->addWidget(box);
        layout->setAlignment(Qt::AlignCenter);
        layout->setMargin(0);
        widget->setLayout(layout);
        return widget;
    }

private:
    bool A::* member;
    QVector<QCheckBox *> widgets;
};


#endif // ASSIGNMENTCELL_H
