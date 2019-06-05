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
#include "QCustomPlot.h"
#include "CircularFifo.h"
#include <memory>
#include "WideComboBox.h"

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


class GraphDlg : public QWidget
{
     Q_OBJECT
private:
    Ui::GraphDlg *ui;
    ChannelListModel clm;

    QTimer dataTimer;

    bool initial;
    long long nPoints;
    double t0;

    std::vector<GraphData> activeGraphs;

    QVector<QCheckBox*> actives;
    QVector<ColorButton*> colors;
    QVector<QComboBox*> types;
    QVector<WideComboBox*> channels;
    QMetaObject::Connection activec, typec, channelc;

    void addRow(int row, QCheckBox *active, ColorButton *colBtn, QComboBox *type, WideComboBox *channel);

    void setInteractive(bool maybe);

private slots:
    void replot();
    void growTable(bool reactive = true);
    void checkChannelTypes();

    void on_TraceActivate_clicked();
    void on_TraceDeactivate_clicked();
    void on_TraceClear_clicked();
    void on_TraceReset_clicked();

public:
     GraphDlg(QWidget *parent = 0);
     void link(QWidget *mainwin);
     ~GraphDlg();

     struct DataPoint { double t; double value; };
     std::vector<std::unique_ptr<CircularFifo<DataPoint>>> q;

public slots:
     bool startPlotting(DCThread *);
     void stopPlotting();
     void reloadGraphs();
     void importData();
     void exportData();
}; 

#endif
