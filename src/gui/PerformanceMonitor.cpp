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

#include "PerformanceMonitor.h"
#include "ui_PerformanceMonitor.h"
#include "DCThread.h"

PerformanceMonitor::PerformanceMonitor(QWidget *parent) :
    QWidget(parent),
    q(100),
    ui(new Ui::PerformanceMonitor),
    dataTimer(this)
{
    ui->setupUi(this);

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s.%z");
    ui->plot->xAxis->setTicker(timeTicker);

    ui->plot->xAxis->setLabel("Time (s)");
    ui->plot->xAxis->setRange(0, 10);
    ui->plot->yAxis->setLabel("Time step (ms)");
    ui->plot->yAxis->setRange(0, 1);

    connect(ui->plot, &QCustomPlot::selectionChangedByUser, [=](){
        QList<QCPAxis *> axes = ui->plot->selectedAxes();
        if ( axes.isEmpty() )
           axes = ui->plot->axisRect()->axes();
        ui->plot->axisRect()->setRangeZoomAxes(axes);
        ui->plot->axisRect()->setRangeDragAxes(axes);

    });
    ui->plot->axisRect()->setRangeZoomAxes(ui->plot->axisRect()->axes());
    ui->plot->axisRect()->setRangeDragAxes(ui->plot->axisRect()->axes());

    ui->plot->legend->setVisible(true);
    resetGraph();
    ui->plot->replot();

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(replot()));
}

void PerformanceMonitor::resetGraph()
{
    ui->plot->clearGraphs();
    for ( int i = 0; i < 3; i++ )
        ui->plot->addGraph();

    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setColor(QColor(255, 0, 0, 120));
    ui->plot->graph(0)->setPen(pen);
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangleInverted, Qt::red, 4));
    ui->plot->graph(0)->setName("Worst case");

    pen.setColor(QColor(0,255,0,120));
    ui->plot->graph(1)->setPen(pen);
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangle, Qt::green, 4));
    ui->plot->graph(1)->setName("Best case");

    ui->plot->graph(2)->setName("Mean");

    ui->plot->xAxis->moveRange(-ui->plot->xAxis->range().lower);
}

PerformanceMonitor::~PerformanceMonitor()
{
    dataTimer.stop();
    delete ui;
}

bool PerformanceMonitor::startPlotting(DCThread *dct)
{
    if ( !PerfMonp.active ) {
        dct->setPerformanceMonitor();
        return false;
    }

    q.reset();
    resetGraph();
    dct->setPerformanceMonitor(this, PerfMonp.interval);
    setInteractive(false);

    if ( PerfMonp.interval < 0.1 )
        dataTimer.start(20);
    else
        dataTimer.start(PerfMonp.interval*200); // redundant replots to prevent wildly out-of-sync plot
    return true;
}

void PerformanceMonitor::stopPlotting()
{
    dataTimer.stop();
    replot();
    setInteractive(true);
}

void PerformanceMonitor::importData()
{
    ui->active->setChecked(PerfMonp.active);
    ui->samplingInterval->setValue(PerfMonp.interval);
}

void PerformanceMonitor::exportData()
{
    PerfMonp.active = ui->active->isChecked();
    PerfMonp.interval = ui->samplingInterval->value();
}

void PerformanceMonitor::setInteractive(bool maybe)
{
    ui->active->setEnabled(maybe);
    ui->samplingInterval->setEnabled(maybe);
}

void PerformanceMonitor::replot()
{
    DataPoint point;
    if ( q.pop(point) ) {
        bool rangeFound;
        QCPRange range = ui->plot->graph()->getKeyRange(rangeFound);

        do {
            ui->plot->graph(0)->addData(point.t, point.maxDt * 1e3);
            ui->plot->graph(1)->addData(point.t, point.minDt * 1e3);
            ui->plot->graph(2)->addData(point.t, PerfMonp.interval/point.n * 1e3);
        } while ( q.pop(point) );

        // make key axis range scroll with the data:
        if ( rangeFound ) {
            double xUpper = ui->plot->xAxis->range().upper;
            if ( range.upper <= xUpper && point.t > xUpper)
                ui->plot->xAxis->moveRange(point.t - xUpper);
        }
        ui->plot->replot(QCustomPlot::rpQueuedReplot);
    }
}
