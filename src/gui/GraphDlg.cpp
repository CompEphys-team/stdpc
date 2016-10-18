#include "ui_GraphDlg.h"
#include "GraphDlg.h"
#include <QColorDialog>
#include "Global.h"
#include "DCThread.h"

GraphDlg::GraphDlg(QWidget *parent)
     : QDialog(parent),
       ui(new Ui::GraphDlg),
       clm(ChannelListModel::AnalogIn
         | ChannelListModel::AnalogOut
         | ChannelListModel::SpikeGen
         | ChannelListModel::Virtual, this),
       color("black"),
       dataTimer(this)
 {
    ui->setupUi(this);

    connect(parent, SIGNAL(channelsChanged()), &clm, SLOT(updateChns()));
    connect(parent, SIGNAL(modelRemoved(ChannelIndex)), &clm, SLOT(updateChns(ChannelIndex)));
    ui->channel->setModel(&clm);

    connect(ui->colorBtn, &QToolButton::clicked, [=](){
        color = QColorDialog::getColor(color, this, "Choose a graph color");
        ui->colorBtn->setStyleSheet(QString("background-color: rgb(%1,%2,%3);")
                                    .arg(color.red()).arg(color.green()).arg(color.blue()));
    });
    ui->colorBtn->setStyleSheet(QString("background-color: black;"));

    connect(ui->channel, SIGNAL(currentIndexChanged(int)), this, SLOT(channelIndexChanged()));
    ui->channel->setCurrentIndex(0);

    connect(ui->addBtn, &QPushButton::clicked, [=](){
        if ( ui->channel->currentIndex() >= 0 ) {
            GraphData p;
            p.active = true;
            p.chan = ui->channel->currentData().value<ChannelIndex>();
            p.isVoltage = ui->radioVoltage->isChecked();
            p.color = color;
            Graphp.push_back(p);
            reloadGraphs();
        }
    });
    connect(ui->clearBtn, &QPushButton::clicked, [=](){
        Graphp.clear();
        reloadGraphs();
    });

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s.%z");
    ui->plot->xAxis->setTicker(timeTicker);
    ui->plot->axisRect()->setupFullAxesBox();

    ui->plot->xAxis->setLabel("Time (s)");
    ui->plot->xAxis->setRange(0, 10);
    ui->plot->yAxis->setLabel("Voltage (mV)");
    ui->plot->yAxis->setRange(-100, 50);
    ui->plot->yAxis2->setLabel("Current (nA)");
    ui->plot->yAxis2->setRange(-100, 100);
    ui->plot->yAxis2->setTickLabels(true);

    ui->plot->legend->setVisible(true);
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    ui->plot->plotLayout()->addElement(0, 1, subLayout);
    subLayout->addElement(0, 0, new QCPLayoutElement);
    subLayout->addElement(1, 0, ui->plot->legend);
    subLayout->addElement(2, 0, new QCPLayoutElement);
    subLayout->addElement(0, 1, new QCPLayoutElement);
    ui->plot->plotLayout()->setColumnStretchFactor(1, 0.001);

    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot, &QCustomPlot::selectionChangedByUser, [=](){
        QList<QCPAxis *> axes = ui->plot->selectedAxes();
        if ( axes.isEmpty() )
           axes = ui->plot->axisRect()->axes();
        ui->plot->axisRect()->setRangeZoomAxes(axes);
        ui->plot->axisRect()->setRangeDragAxes(axes);

    });
    ui->plot->axisRect()->setRangeZoomAxes(ui->plot->axisRect()->axes());
    ui->plot->axisRect()->setRangeDragAxes(ui->plot->axisRect()->axes());

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(replot()));

    reloadGraphs();
}

void GraphDlg::channelIndexChanged()
{
    ChannelIndex sel = ui->channel->currentData().value<ChannelIndex>();
    ui->radioCurrent->setEnabled(sel.isVirtual);
    ui->radioVoltage->setEnabled(sel.isVirtual);
    if ( !sel.isVirtual ) {
        ui->radioCurrent->setChecked(!sel.isInChn && !sel.isSG);
        ui->radioVoltage->setChecked(sel.isInChn || sel.isSG);
    }
}

GraphDlg::~GraphDlg()
{
    dataTimer.stop();
    delete ui;
}

void GraphDlg::reject()
{
    if ( !dataTimer.isActive() )
        QDialog::reject();
}

void GraphDlg::reloadGraphs()
{
    ui->plot->clearGraphs();
    QCPGraph *graph;
    for ( GraphData const& p : Graphp ) {
        if ( p.isVoltage ) {
            graph = ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis);
            graph->setName(QString("%1 (mV)").arg(p.chan.prettyName()));
        } else {
            graph = ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis2);
            graph->setName(QString("%1 (nA)").arg(p.chan.prettyName()));
        }
        graph->setPen(QPen(p.color));
    }
    ui->plot->replot();
}

void GraphDlg::startPlotting(DCThread *DCT)
{
    if ( !isVisible() || Graphp.empty() ) {
        for ( GraphData &p : Graphp )
            p.active = false;
        DCT->setGraph();
        return;
    }

    ui->addBtn->setEnabled(false);
    ui->clearBtn->setEnabled(false);
    ui->samplingInterval->setEnabled(false);

    reloadGraphs();
    ui->plot->xAxis->moveRange(-ui->plot->xAxis->range().lower);
    q.clear();

    for ( GraphData &p : Graphp ) {
        p.active = true;
        q.push_back(std::unique_ptr<queue_type>(new queue_type()));
    }

    DCT->setGraph(this, ui->samplingInterval->value() * 1e-3);

    initial = true;
    nPoints = 0;

    dataTimer.start(20); // Plotting interval at 50 Hz regardless of sampling interval
}

void GraphDlg::stopPlotting()
{
    dataTimer.stop();
    ui->addBtn->setEnabled(true);
    ui->clearBtn->setEnabled(true);
    ui->samplingInterval->setEnabled(true);
}

void GraphDlg::replot()
{
    bool rangeFound;
    QCPRange range = ui->plot->graph()->getKeyRange(rangeFound);

    DataPoint point {0., 0.};

    if ( initial && q[0]->pop(point) ) {
        initial = false;
        t0 = point.t;
    }

    int i = 0;
    double fac;
    double tNow = 0.;
    for ( auto &queue : q ) {
        if ( Graphp[i].isVoltage )
            fac = 1e3;
        else
            fac = 1e9;
        while ( queue->pop(point) ) {
            ui->plot->graph(i)->addData(point.t, point.value * fac);
            if ( !i )
                ++nPoints;
        }
        if ( !i )
            tNow = point.t;
        ++i;
    }

    // make key axis range scroll with the data:
    if ( rangeFound ) {
        double xUpper = ui->plot->xAxis->range().upper;
        if ( range.upper <= xUpper && point.t > xUpper)
            ui->plot->xAxis->moveRange(point.t - xUpper);
    }
    ui->plot->replot(QCustomPlot::rpQueuedReplot);

    ui->cycleFreq->setText(QString::number(nPoints / (tNow - t0)));
}
