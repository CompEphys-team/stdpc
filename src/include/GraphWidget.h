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
    explicit GraphWidget(QWidget *parent = 0);
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
