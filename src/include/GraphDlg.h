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

#ifndef GRAPHDLG_H
#define GRAPHDLG_H

#include <QWidget>
#include "ChannelListModel.h"
#include "WideComboBox.h"
#include <QCheckBox>
#include <QToolButton>
#include <QColorDialog>
#include <QLabel>

namespace Ui {
class GraphDlg;
}


class ColorButton : public QToolButton
{
    Q_OBJECT
public:
    template <typename... Args> ColorButton(Args... args) :
        QToolButton(args...),
        color("black")
    {
        setColor(color);
        connect(this, &ColorButton::clicked, [=](){
            QColor ret = QColorDialog::getColor(color, parentWidget(), "Choose a graph color");
            if ( ret.isValid() )
                setColor(ret);
        });
    }

    QColor color;

public slots:
    inline void setColor(QColor const& c)
    {
        color = c;
        setStyleSheet(QString("background-color: rgb(%1,%2,%3);").arg(c.red()).arg(c.green()).arg(c.blue()));
    }
};


class GraphDlg : public QDialog
{
     Q_OBJECT
private:
    Ui::GraphDlg *ui;
    ChannelListModel *clm;

    QVector<QCheckBox*> actives;
    QVector<ColorButton*> colors;
    QVector<QComboBox*> types;
    QVector<QComboBox*> unitMods;
    QVector<QLabel*> unitTypes;
    QVector<WideComboBox*> channels;
    QMetaObject::Connection activec, typec, unitc, channelc;

    int m_plot;
    bool interactive;
    bool m_delete;

    void addRow(int row, QCheckBox *active, ColorButton *colBtn, QComboBox *type, QComboBox *unitMod, QLabel *unitType, WideComboBox *channel);

private slots:
    void growTable(bool reactive = true);
    void checkChannelTypes();

    void on_TraceActivate_clicked();
    void on_TraceDeactivate_clicked();
    void on_TraceClear_clicked();

    void on_removePlot_clicked();

public:
     GraphDlg(QWidget *parent = nullptr);
     ~GraphDlg();

     inline void setPlot(int g, bool deleteOnClose) { m_plot = g; m_delete = deleteOnClose; }
     inline int plot() const { return m_plot; }

     inline void setInteractive(bool maybe) { interactive = maybe; }

     static QStringList unitModifiers;

public slots:
     void open();
     void accept();
     void reject();

signals:
     void updatePlot(int row);
     void removePlot(int row);
}; 

#endif
