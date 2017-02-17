#include "ui_GraphDlg.h"
#include "GraphDlg.h"
#include <QColorDialog>
#include "Global.h"
#include "DCThread.h"


GraphDlg::GraphDlg(QWidget *parent)
     : QWidget(parent),
       ui(new Ui::GraphDlg),
       clm(ChannelListModel::AnalogIn
         | ChannelListModel::AnalogOut
         | ChannelListModel::Virtual, this),
       dataTimer(this)
 {
    ui->setupUi(this);

    ui->table->setHorizontalHeaderLabels({"Active",
                                          "Colour",
                                          "Type",
                                          "Channel"});
    ui->table->setColumnWidth(0, 50);
    ui->table->setColumnWidth(1, 60);
    ui->table->setColumnWidth(2, 80);
    growTable(false);

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

    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot->xAxis2, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis, SLOT(setRange(QCPRange)));
    ui->plot->xAxis2->setTicker(timeTicker);

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
}

void GraphDlg::link(QWidget *mainwin)
{
    connect(mainwin, SIGNAL(channelsChanged()), &clm, SLOT(updateChns()));
    connect(mainwin, SIGNAL(modelRemoved(ChannelIndex)), &clm, SLOT(updateChns(ChannelIndex)));
}

GraphDlg::~GraphDlg()
{
    dataTimer.stop();
    delete ui;
}

void GraphDlg::importData()
{
    ui->bufferExp->setValue(Plotp.bufferExp);
    ui->samplingInterval->setValue(Plotp.interval * 1e3);

    actives.clear();
    colors.clear();
    types.clear();
    channels.clear();
    ui->table->setRowCount(0);
    int row = 0;
    for ( GraphData const& p : Plotp.graphs ) {
        QCheckBox *active = new QCheckBox();
        ColorButton *colBtn = new ColorButton();
        QComboBox *type = new QComboBox();
        WideComboBox *channel = new WideComboBox();
        addRow(row++, active, colBtn, type, channel);
        active->setChecked(p.active);
        colBtn->setColor(p.color);
        type->setCurrentIndex(!p.isVoltage);
        channel->setCurrentIndex(clm.index(p.chan));
    }
    growTable(false);
}

void GraphDlg::exportData()
{
    Plotp.bufferExp = ui->bufferExp->value();
    Plotp.interval = ui->samplingInterval->value() * 1e-3;
    Plotp.graphs.clear();
    Plotp.graphs.reserve(ui->table->rowCount() - 1);
    GraphData p;
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        p.active = actives[i]->isChecked();
        p.color = colors[i]->color;
        p.isVoltage = !types[i]->currentIndex();
        p.chan = channels[i]->currentData().value<ChannelIndex>();
        Plotp.graphs.push_back(p);
    }
}

void GraphDlg::reloadGraphs()
{
    constexpr size_t bufferSize[] = {100,100,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
    ui->plot->clearGraphs();
    activeGraphs.clear();
    activeGraphs.reserve(Plotp.graphs.size());
    q.clear();
    q.reserve(Plotp.graphs.size());
    QCPGraph *graph;
    for ( GraphData const& p : Plotp.graphs ) {
        if ( p.active ) {
            activeGraphs.push_back(p);
            q.push_back(std::unique_ptr<CircularFifo<DataPoint>>(new CircularFifo<DataPoint>(bufferSize[Plotp.bufferExp])));
            if ( p.isVoltage ) {
                graph = ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis);
            } else {
                graph = ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis2);
            }
            graph->setPen(QPen(p.color));
        }
    }
    ui->plot->replot();
}

bool GraphDlg::startPlotting(DCThread *DCT)
{
    bool isPlotting = false;
    if ( Plotp.graphs.size() ) {
        for ( GraphData &p : Plotp.graphs ) {
            if ( p.active ) {
                isPlotting = true;
                break;
            }
        }
    }
    if ( !isPlotting ) {
        DCT->setGraph();
        return false;
    }

    ui->samplingInterval->setEnabled(false);
    ui->bufferExp->setEnabled(false);

    reloadGraphs();
    ui->plot->xAxis->moveRange(-ui->plot->xAxis->range().lower);

    DCT->setGraph(this, Plotp.interval);

    initial = true;
    nPoints = 0;

    if ( Plotp.interval < 1e-4 )
        dataTimer.start(); // As fast as possible to prevent q overflows
    else
        dataTimer.start(20); // 50 Hz for slower sampling

    return true;
}

void GraphDlg::stopPlotting()
{
    dataTimer.stop();
    ui->samplingInterval->setEnabled(true);
    ui->bufferExp->setEnabled(true);
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
        if ( activeGraphs[i].isVoltage )
            fac = 1e3;
        else
            fac = 1e9;
        while ( queue->pop(point) ) {
            try {
                ui->plot->graph(i)->addData(point.t, point.value * fac);
            } catch ( std::bad_alloc ) {
                // Out of memory - remove half of all data to keep going
                double middle = range.lower + (range.upper - range.lower) / 2;
                for ( int j = 0; j < (int)q.size(); j++ ) {
                    ui->plot->graph(j)->data()->removeBefore(middle);
                }
                // Then try again
                ui->plot->graph(i)->addData(point.t, point.value * fac);
            }

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




void GraphDlg::growTable(bool reactive)
{
    disconnect(activec);
    disconnect(typec);
    disconnect(channelc);

    if ( reactive && !actives.empty() )
        actives.last()->setChecked(true);

    QCheckBox *active = new QCheckBox();
    ColorButton *colBtn = new ColorButton();
    QComboBox *type = new QComboBox();
    WideComboBox *channel = new WideComboBox();

    addRow(ui->table->rowCount(), active, colBtn, type, channel);

    activec = connect(active, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    typec = connect(type, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
    channelc = connect(channel, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
}

void GraphDlg::addRow(int row, QCheckBox *active, ColorButton *colBtn, QComboBox *type, WideComboBox *channel)
{
    ui->table->insertRow(row);

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(active);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    widget->setLayout(layout);
    ui->table->setCellWidget(row, 0, widget);

    ui->table->setCellWidget(row, 1, colBtn);

    type->addItem("Voltage");
    type->addItem("Current");
    ui->table->setCellWidget(row, 2, type);

    channel->setModel(&clm);
    connect(channel, static_cast<void (WideComboBox::*)(int)>(&WideComboBox::currentIndexChanged), [=](int){
        ChannelIndex sel = channel->currentData().value<ChannelIndex>();
        if ( sel.isVirtual && sel.modelClass != "SG" ) {
            type->setEnabled(true);
        } else if ( sel.isAnalog && !sel.isInChn ) {
            type->setCurrentIndex(1);
            type->setEnabled(false);
        } else {
            type->setCurrentIndex(0);
            type->setEnabled(false);
        }
    });
    ui->table->setCellWidget(row, 3, channel);

    actives.insert(row, active);
    colors.insert(row, colBtn);
    types.insert(row, type);
    channels.insert(row, channel);
}
