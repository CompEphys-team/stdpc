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

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <memory>
#include "CircularFifo.h"
#include "ObjectDataTypes.h"
#include "QCustomPlot.h"
#include "GraphDlg.h"

namespace Ui {
class GraphWidget;
}
class DCThread;

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = nullptr);
    ~GraphWidget();

    struct DataPoint { double t; double value; };
    std::vector<std::unique_ptr<CircularFifo<DataPoint>>> q;
    std::vector<ChannelIndex> plottedChannels;

public slots:
     bool startPlotting(DCThread *);
     void stopPlotting();
     void reloadGraphs();
     void importData();
     void exportData();

private:
    Ui::GraphWidget *ui;
    GraphDlg *dialog;
    QPushButton *more;

    QTimer dataTimer;

    bool initial;
    long long nPoints;
    double t0;

    std::vector<std::tuple<QCPGraph*, GraphData>> activeGraphs;
    std::vector<size_t> qIndex;

    std::vector<QCustomPlot*> plots;

    void setInteractive(bool maybe);

    void setupGraph(GraphData &p, QCPGraph *graph);

private slots:
    void replot();
    void addPlot(bool import);
    void removePlot(int);
    void updatePlot(int row);
    void rangeChanged(QCPRange);
};

#endif // GRAPHWIDGET_H
