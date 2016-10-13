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

class GraphDlg : public QDialog
{
     Q_OBJECT
private:
    Ui::GraphDlg *ui;
    ChannelListModel clm;
    QColor color;

    QTimer dataTimer;

private slots:
    void replot();
    void channelIndexChanged();

public:
     GraphDlg(QWidget *parent = 0);
     ~GraphDlg();

     struct DataPoint { double t; double value; };
     typedef CircularFifo<DataPoint, 10000> queue_type;
     std::vector<std::unique_ptr<queue_type>> q;

public slots:
     void startPlotting(DCThread *);
     void stopPlotting();
     void reloadGraphs();
}; 

#endif
