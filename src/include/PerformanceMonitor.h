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
