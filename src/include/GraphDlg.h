#ifndef GRAPHDLG_H
#define GRAPHDLG_H

#include <QDialog>
#include "ChannelListModel.h"
#include "QCustomPlot.h"
#include "CircularFifo.h"
#include <memory>

namespace Ui {
class GraphDlg;
}

class GraphDlg : public QWidget
{
     Q_OBJECT
private:
    Ui::GraphDlg *ui;
    ChannelListModel clm;
    QColor color;

    QTimer dataTimer;

    bool initial;
    long long nPoints;
    double t0;

private slots:
    void replot();
    void channelIndexChanged();

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
}; 

#endif
