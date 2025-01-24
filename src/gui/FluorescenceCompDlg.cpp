#include "FluorescenceCompDlg.h"
#include "ui_FluorescenceCompDlg.h"
#include "DeviceManager.h"

double FluorescenceCompDlg::V0 = -0.06;
double FluorescenceCompDlg::dV = 0.1;

FluorescenceCompDlg::FluorescenceCompDlg(QLineEdit *gain, QLineEdit *bias, ChannelIndex dex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FluorescenceCompDlg),
    exportGain(gain),
    exportBias(bias),
    dex(dex),
    daq(Devices.getDevice(dex)),
    backupGain(daq->params()->inChn[dex.chanID].gainFac),
    backupBias(daq->params()->inChn[dex.chanID].bias),
    gain(gain->text().toDouble()),
    bias(bias->text().toDouble()),
    f0(0),
    df(0)
{
    ui->setupUi(this);
    updateOutputs();
    ui->inputBaseline->setValue(int(V0 * 1e3));
    ui->inputSpikesize->setValue(int(dV * 1e3));

    daq->params()->inChn[dex.chanID].gainFac = 1;
    daq->params()->inChn[dex.chanID].bias = 0;

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(acquire()));
    connect(&calcTimer, SIGNAL(timeout()), this, SLOT(calculate()));

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s.%z");
    ui->plot->xAxis->setTicker(timeTicker);

    ui->plot->xAxis->setLabel("Time (s)");
    ui->plot->xAxis->setRange(0, 10);
    ui->plot->yAxis->setLabel("Input signal");
    ui->plot->yAxis->setRange(0, 1);

    connect(ui->plot, &QCustomPlot::selectionChangedByUser, this, [=](){
        QList<QCPAxis *> axes = ui->plot->selectedAxes();
        if ( axes.isEmpty() )
            axes = ui->plot->axisRect()->axes();
        ui->plot->axisRect()->setRangeZoomAxes(axes);
        ui->plot->axisRect()->setRangeDragAxes(axes);
    });
    ui->plot->axisRect()->setRangeZoomAxes(ui->plot->axisRect()->axes());
    ui->plot->axisRect()->setRangeDragAxes(ui->plot->axisRect()->axes());

    ui->plot->yAxis2->setVisible(true);
    ui->plot->yAxis2->setSelectableParts(QCPAxis::spNone);
    ui->plot->yAxis2->setLabel("Voltage (mV)");
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(adjustAxis()));

    resetGraph();
    ui->plot->replot();

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(ui->inputBaseline, SIGNAL(textChanged(QString)), this, SLOT(updateInputs()));
    connect(ui->inputSpikesize, SIGNAL(textChanged(QString)), this, SLOT(updateInputs()));
}

FluorescenceCompDlg::~FluorescenceCompDlg()
{
    daq->reset_board();
    daq->reset_chans();

    daq->params()->inChn[dex.chanID].gainFac = backupGain;
    daq->params()->inChn[dex.chanID].bias = backupBias;

    delete ui;
}

void FluorescenceCompDlg::exportData()
{
    exportGain->setText(QString::number(gain));
    exportBias->setText(QString::number(bias));
}

void FluorescenceCompDlg::start()
{
    resetGraph();

    QString ret;
    if ( daq->initialize_board(ret) ) {
        daq->init_chans();
        chan = Devices.getInChan(dex);

        ui->pushButton->disconnect(this);
        ui->pushButton->setText("Stop");
        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(stop()));
        ui->status->setText("Acquiring data...");

        daq->start();
        DAQClock.reset_RTC();
        dataTimer.start();
        calcTimer.start(2000);
    } else {
        ui->status->setText(QString("Failed to initialize: %1").arg(ret));
    }
}

void FluorescenceCompDlg::stop()
{
    daq->reset_board();
    daq->reset_chans();
    dataTimer.stop();
    calcTimer.stop();
    calculate();

    ui->pushButton->disconnect(this);
    ui->pushButton->setText("Start");
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(start()));
    ui->status->setText("Data acquisition complete.");
}

void FluorescenceCompDlg::acquire()
{
    daq->get_scan();
    DAQClock.get_RTC();

    bool tRangeFound, vRangeFound;
    QCPRange tRange = ui->plot->graph()->getKeyRange(tRangeFound);

    ui->plot->graph(0)->addData(DAQClock.t, chan->V);

    if ( tRangeFound ) {
        double tUpper = ui->plot->xAxis->range().upper;
        if ( tRange.upper <= tUpper && DAQClock.t > tUpper)
            ui->plot->xAxis->moveRange(DAQClock.t - tUpper);
    }

    QCPRange vRange = ui->plot->graph()->getValueRange(vRangeFound);
    if ( vRangeFound ) {
        double buffer = 0.05 * (vRange.upper - vRange.lower);
        ui->plot->yAxis->setRange(vRange.lower - buffer, vRange.upper + buffer);
    }

    ui->plot->replot(QCustomPlot::rpQueuedReplot);
}

bool comp(QCPGraphData const& left, QCPGraphData const& right)
{
    return left.value < right.value;
}

void FluorescenceCompDlg::calculate()
{
    auto g = ui->plot->graph()->data();
    std::sort(g->begin(), g->end(), comp);
    double fmax = (g->end()-1)->value;
    f0 = (g->begin() + g->size()/2)->value;
    df = fmax - f0;
    g->sort();  // Return to sort-by-key (time) for display.

    updateInputs();
}

void FluorescenceCompDlg::updateInputs()
{
    dV = ui->inputSpikesize->value() * 1e-3;
    V0 = ui->inputBaseline->value() * 1e-3;
    if ( df > 0 ) {
        gain = dV/df;  // dV = df * gain
        bias = V0 - f0*gain;  // V0 = f0 * gain + bias
    }
    updateOutputs();
    adjustAxis();
}

void FluorescenceCompDlg::adjustAxis()
{
    QCPRange r(ui->plot->yAxis->range());
    ui->plot->yAxis2->setRange((r.lower*gain + bias)*1e3, (r.upper*gain + bias)*1e3); // *1e3 for mV rather than V
    ui->plot->replot(QCustomPlot::rpQueuedReplot);
}

void FluorescenceCompDlg::updateOutputs()
{
    ui->outputBias->setText(QString::number(bias));
    ui->outputGain->setText(QString::number(gain));
    ui->outputDF->setText(QString::number(df));
    ui->outputF0->setText(QString::number(f0));
}

void FluorescenceCompDlg::resetGraph()
{
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->xAxis->moveRange(-ui->plot->xAxis->range().lower);
    adjustAxis();
}
