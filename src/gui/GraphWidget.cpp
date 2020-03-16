/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
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

#include "GraphWidget.h"
#include "ui_GraphWidget.h"
#include "DCThread.h"
#include "Global.h"

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWidget),
    dialog(new GraphDlg(this)),
    more(new QPushButton("New plot")),
    dataTimer(this)
{
    ui->setupUi(this);

    ui->table->setRowHeight(0, 30);
    ui->table->setCellWidget(0, 0, more);
    connect(more, SIGNAL(clicked(bool)), this, SLOT(addPlot(bool)));

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(replot()));
    connect(dialog, SIGNAL(updatePlot(int)), this, SLOT(updatePlot(int)));
    connect(dialog, SIGNAL(removePlot(int)), this, SLOT(removePlot(int)));
}

GraphWidget::~GraphWidget()
{
    dataTimer.stop();
    delete ui;
}

void GraphWidget::addPlot(bool import)
{
    int row = ui->table->rowCount()-1;
    ui->table->insertRow(row);
    QCustomPlot *plot = new QCustomPlot();
    ui->table->setCellWidget(row, 0, plot);
    plots.push_back(plot);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend);
    plot->legend->setSelectableParts(QCPLegend::spItems);
    connect(plot, &QCustomPlot::mouseDoubleClick, [=](){
        dialog->setPlot(row, false);
        dialog->open();
    });

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s.%z");
    plot->xAxis->setTicker(timeTicker);
    plot->xAxis->setLabel("Time (s)");
    if ( !row )
        plot->xAxis->setRange(0, 10);

    plot->yAxis->setTickLabels(true);

    plot->legend->setVisible(true);

    connect(plot, &QCustomPlot::selectionChangedByUser, [=](){
        QList<QCPAxis *> axes = plot->selectedAxes();
        if ( axes.isEmpty() )
           axes = plot->axisRect()->axes();
        plot->axisRect()->setRangeZoomAxes(axes);
        plot->axisRect()->setRangeDragAxes(axes);

    });
    plot->axisRect()->setRangeZoomAxes(plot->axisRect()->axes());
    plot->axisRect()->setRangeDragAxes(plot->axisRect()->axes());

    if ( import ) {
        ui->table->setRowHeight(row, Plotp.plot[row].height);
        plot->xAxis->setRange(0, Plotp.xRange);
        plot->yAxis->setRange(Plotp.plot[row].yLower, Plotp.plot[row].yUpper);
    } else {
        Plotp.plot.resize(row+1);
        plot->xAxis->setRange(plots[0]->xAxis->range());
        if ( row > 0 ) {
            plot->yAxis->setRange(plots[row-1]->yAxis->range());
            ui->table->setRowHeight(row, ui->table->rowHeight(row-1));
        } else {
            plot->yAxis->setRange(Plotp.plot[row].yLower, Plotp.plot[row].yUpper);
        }
        dialog->setPlot(row, true);
        dialog->open();
    }

    connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(rangeChanged(QCPRange)));

    updatePlot(row);
}

void GraphWidget::removePlot(int row)
{
    ui->table->removeRow(row);
    plots.erase(plots.begin() + row);
    Plotp.plot.erase(Plotp.plot.begin() + row);
}

void GraphWidget::updatePlot(int row)
{
    QCustomPlot *plot = plots[row];
    bool hasGraphs = false;
    for ( auto tup : activeGraphs ) {
        if ( std::get<0>(tup)->parentPlot() == plot ) {
            hasGraphs = true;
            break;
        }
    }
    if ( !hasGraphs ) { // Add some dummies, but don't overwrite existing traces
        plot->clearGraphs();
        for ( GraphData &g : Plotp.plot[row].graph ) {
            if ( g.active ) {
                setupGraph(g, plot->addGraph());
            }
        }
    }
    plot->replot();
}

void GraphWidget::setupGraph(GraphData &p, QCPGraph *graph)
{
    QString name = QString("%1 [%2%3]").arg(p.chan.prettyName()).arg(GraphDlg::unitModifiers[p.unitMod]);

    if ( p.chan.isConductance )
        name = name.arg("S");
    else if ( p.chan.isAnalog )
        name = name.arg(p.chan.isInChn ? "V" : "A");
    else if ( p.chan.isVirtual )
        name = name.arg((p.chan.isInChn = p.isVoltage) ? "V" : "A");
    graph->setName(name);
    graph->setPen(QPen(p.color));

    QCPPlottableLegendItem *item = graph->parentPlot()->legend->itemWithPlottable(graph);
    connect(item, &QCPPlottableLegendItem::selectionChanged, [=](bool on){
        if ( !on ) return;
        on = graph->visible();
        graph->setVisible(!on);
        item->setTextColor(on ? Qt::lightGray : Qt::black);
        item->setSelected(false);
        graph->parentPlot()->replot();
    });
}

void GraphWidget::rangeChanged(QCPRange range)
{
    for ( QCustomPlot *plot : plots ) {
        plot->xAxis->blockSignals(true);
        plot->xAxis->setRange(range);
        plot->replot();
        plot->xAxis->blockSignals(false);
    }
}

void GraphWidget::importData()
{
    ui->active->setChecked(Plotp.active);
    ui->bufferExp->setValue(Plotp.bufferExp);
    ui->samplingInterval->setValue(Plotp.interval * 1e3);

    while ( ui->table->rowCount() > 1 )
        ui->table->removeRow(0);
    plots.clear();
    for ( size_t i = 0; i < Plotp.plot.size(); i++ )
        addPlot(true);
}

void GraphWidget::exportData()
{
    Plotp.active = ui->active->isChecked();
    Plotp.bufferExp = ui->bufferExp->value();
    Plotp.interval = ui->samplingInterval->value() * 1e-3;
    if ( !plots.empty() )
        Plotp.xRange = plots[0]->xAxis->range().size();
    for ( int i = 0; i < ui->table->rowCount()-1; i++ ) {
        Plotp.plot[i].height = ui->table->rowHeight(i);
        Plotp.plot[i].yLower = plots[i]->yAxis->range().lower;
        Plotp.plot[i].yUpper = plots[i]->yAxis->range().upper;
    }
}

void GraphWidget::reloadGraphs()
{
    constexpr size_t bufferSize[] = {100,100,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

    for ( QCustomPlot *plot : plots )
        plot->clearGraphs();
    activeGraphs.clear();
    q.clear();
    qIndex.clear();
    plottedChannels.clear();

    int nGraphs = 0;
    for ( const PlotData &p : Plotp.plot )
        nGraphs += p.graph.size();
    activeGraphs.reserve(nGraphs);
    q.reserve(nGraphs);

    QCPGraph *graph;
    int nth = 0;
    for ( size_t i = 0; i < Plotp.plot.size(); i++ ) {
        for ( size_t j = 0; j < Plotp.plot[i].graph.size(); j++ ) {
            GraphData &p = Plotp.plot[i].graph[j];
            if ( !p.active )
                continue;

            // Add to plot
            graph = plots[i]->addGraph(plots[i]->xAxis, plots[i]->yAxis);
            setupGraph(p, graph);

            activeGraphs.push_back(std::make_tuple(graph, p));

            // Add duplicates into q and plottedChannels only once
            size_t k;
            bool duplicate = false;
            for ( k = 0; k < plottedChannels.size(); k++ ) {
                if ( plottedChannels[k] == p.chan
                     && (!p.chan.isVirtual || (p.isVoltage == std::get<1>(activeGraphs[qIndex[k]]).isVoltage)) ) {
                    duplicate = true;
                    break;
                }
            }
            if ( duplicate ) {
                qIndex.push_back(k);
            } else {
                qIndex.push_back(nth++);
                q.push_back(std::unique_ptr<CircularFifo<DataPoint>>(new CircularFifo<DataPoint>(bufferSize[Plotp.bufferExp])));
                plottedChannels.push_back(p.chan);
            }
        }
        plots[i]->replot();
    }
}

bool GraphWidget::startPlotting(DCThread *DCT)
{
    bool isPlotting = false;
    for ( PlotData &pl : Plotp.plot ) {
        for ( GraphData &gr : pl.graph ) {
            if ( gr.active ) {
                isPlotting = true;
                break;
            }
        }
    }
    if ( !isPlotting || !Plotp.active ) {
        DCT->setGraph();
        return false;
    }

    setInteractive(false);

    reloadGraphs();
    plots[0]->xAxis->moveRange(-plots[0]->xAxis->range().lower); // Propagates to others by signal

    DCT->setGraph(this, Plotp.interval);

    initial = true;
    nPoints = 0;

    if ( Plotp.interval < 1e-4 )
        dataTimer.start(); // As fast as possible to prevent q overflows
    else
        dataTimer.start(20); // 50 Hz for slower sampling

    return true;
}

void GraphWidget::stopPlotting()
{
    dataTimer.stop();
    setInteractive(true);
}

void GraphWidget::setInteractive(bool maybe)
{
    dialog->setInteractive(maybe);
    ui->samplingInterval->setEnabled(maybe);
    ui->bufferExp->setEnabled(maybe);
    ui->active->setEnabled(maybe);
}

void GraphWidget::replot()
{
    if ( plots.empty() )
        return;

    bool rangeFound;
    QCPRange range = plots[0]->graph()->getKeyRange(rangeFound);

    bool settlingEnded = false;
    double settlingOffset = 0;

    DataPoint point {0., 0.};

    if ( initial && q[0]->pop(point) ) {
        initial = false;
        t0 = point.t;
    }

    size_t i = 0;
    double tNow = 0.;
    for ( auto &queue : q ) {
        std::vector<std::tuple<QCPGraph*, GraphData>> graphs;
        std::vector<double> fac;
        for ( size_t j = 0; j < qIndex.size(); j++ ) {
            if ( qIndex[j] == i ) {
                graphs.push_back(activeGraphs[j]);
                const GraphData &p = std::get<1>(activeGraphs[j]);
                fac.push_back(std::pow(10, p.unitMod*3));
            }
        }

        while ( queue->pop(point) ) {
            if ( point.t < 0 ) { // End of settling: Shift all existing data to end at t=0
                for ( size_t j = 0; j < graphs.size(); j++ ) {
                    QSharedPointer<QCPGraphDataContainer> g_container = std::get<0>(graphs[j])->data();
                    if ( !g_container->isEmpty() ) {
                        settlingOffset = (g_container->end()-1)->key;
                        for ( QCPGraphData &g_data : *g_container ) {
                            g_data.key -= settlingOffset;
                        }
                    }
                }
                settlingEnded = true;
                // Ignore unused point.value
                continue;
            }
            for ( size_t j = 0; j < graphs.size(); j++ ) {
                try {
                    std::get<0>(graphs[j])->addData(point.t, point.value * fac[j]);
                } catch ( std::bad_alloc ) {
                    // Out of memory - remove half of all data to keep going
                    double middle = range.lower + (range.upper - range.lower) / 2;
                    for ( size_t ci = 0; ci < plots.size(); ci++ ) {
                        for ( int cj = 0; cj < plots[ci]->graphCount(); cj++ ) {
                            plots[ci]->graph(cj)->data()->removeBefore(middle);
                        }
                    }
                    // Then try again
                    std::get<0>(graphs[j])->addData(point.t, point.value * fac[j]);
                }
            }

            if ( !i )
                ++nPoints;
        }
        if ( !i )
            tNow = point.t;
        ++i;
    }

    // Shift x axis along with data after settling
    if ( settlingEnded ) {
        for ( QCustomPlot *plot : plots ) {
            plot->xAxis->blockSignals(true);
            plot->xAxis->moveRange(-settlingOffset);
            plot->xAxis->blockSignals(false);
        }
        range -= settlingOffset;
    }

    // make key axis range scroll with the data:
    if ( rangeFound ) {
        double xUpper = plots[0]->xAxis->range().upper;
        if ( range.upper <= xUpper && point.t > xUpper) {
            for ( QCustomPlot *plot : plots ) {
                plot->xAxis->blockSignals(true);
                plot->xAxis->moveRange(point.t - xUpper);
                plot->xAxis->blockSignals(false);
            }
        }
    }
    for ( QCustomPlot *plot : plots )
        plot->replot(QCustomPlot::rpQueuedReplot);

    ui->cycleFreq->setText(QString::number(nPoints / (tNow - t0)));
}
