#include "ui_GraphDlg.h"
#include "GraphDlg.h"
#include <QColorDialog>

GraphDlg::GraphDlg(QWidget *parent)
     : QDialog(parent),
       ui(new Ui::GraphDlg),
       clm(ChannelListModel::getModel(
           ChannelListModel::AnalogIn
         | ChannelListModel::AnalogOut
         | ChannelListModel::Virtual
         | ChannelListModel::Conductance)),
       interactive(true)
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
}

GraphDlg::~GraphDlg()
{
    delete ui;
}

void GraphDlg::open()
{
    if ( !interactive )
        return;

    actives.clear();
    colors.clear();
    types.clear();
    channels.clear();
    ui->table->setRowCount(0);
    int row = 0;
    for ( GraphData const& p : Plotp.plot[m_plot].graph ) {
        QCheckBox *active = new QCheckBox();
        ColorButton *colBtn = new ColorButton();
        QComboBox *type = new QComboBox();
        WideComboBox *channel = new WideComboBox();
        addRow(row++, active, colBtn, type, channel);
        colBtn->setColor(p.color);
        type->setCurrentIndex(!p.isVoltage);
        channel->setCurrentIndex(clm->index(p.chan));
        active->setChecked(p.active);
    }
    growTable(false);

    QDialog::open();
}

void GraphDlg::accept()
{
    Plotp.plot[m_plot].graph.clear();
    Plotp.plot[m_plot].graph.reserve(ui->table->rowCount() - 1);
    GraphData p;
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        p.active = actives[i]->isChecked();
        p.color = colors[i]->color;
        p.chan = channels[i]->currentData().value<ChannelIndex>();
        p.isVoltage = types[i]->currentIndex() == 0;
        Plotp.plot[m_plot].graph.push_back(p);
    }
    emit updatePlot(m_plot);
    QDialog::accept();
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

void GraphDlg::checkChannelTypes()
{
    for ( int i = 0; i < actives.size(); i++ ) {
        bool maybe = channels[i]->currentData().value<ChannelIndex>().isVirtual;
        types[i]->setVisible(maybe);
        types[i]->setEnabled(maybe);
    }
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

    channel->setModel(clm);
    connect(channel, SIGNAL(currentIndexChanged(int)), this, SLOT(checkChannelTypes()));
    ui->table->setCellWidget(row, 3, channel);

    actives.insert(row, active);
    colors.insert(row, colBtn);
    types.insert(row, type);
    channels.insert(row, channel);

    checkChannelTypes();
}

void GraphDlg::on_TraceActivate_clicked()
{
    for ( int i = 0; i < actives.size() - 1; i++ )
        actives[i]->setChecked(true);
}

void GraphDlg::on_TraceDeactivate_clicked()
{
    for ( int i = 0; i < actives.size() - 1; i++ )
        actives[i]->setChecked(false);
}

void GraphDlg::on_TraceClear_clicked()
{
    for ( int i = ui->table->rowCount() - 2; i >= 0; i-- )
        if ( !actives[i]->isChecked() )
            ui->table->removeRow(i);
    checkChannelTypes();
}

void GraphDlg::on_removePlot_clicked()
{
    emit removePlot(m_plot);
    reject();
}
