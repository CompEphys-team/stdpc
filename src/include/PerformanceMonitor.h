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

#ifndef PERFORMANCEMONITOR_H
#define PERFORMANCEMONITOR_H

#include <QWidget>
#include <QTimer>
#include "CircularFifo.h"


namespace Ui {
class PerformanceMonitor;
}
class DCThread;

class PerformanceMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit PerformanceMonitor(QWidget *parent = 0);
    ~PerformanceMonitor();

    struct DataPoint {
        double t;
        int n;
        double minDt;
        double maxDt;
    };
    CircularFifo<DataPoint> q;

public slots:
     bool startPlotting(DCThread *);
     void stopPlotting();
     void importData();
     void exportData();

private:
    Ui::PerformanceMonitor *ui;
    QTimer dataTimer;

    void setInteractive(bool maybe);
    void resetGraph();

private slots:
    void replot();
};

#endif // PERFORMANCEMONITOR_H
