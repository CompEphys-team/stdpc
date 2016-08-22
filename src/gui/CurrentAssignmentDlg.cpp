#include "CurrentAssignmentDlg.h"
#include "ui_CurrentAssignmentDlg.h"
#include <QHBoxLayout>

CurrentAssignmentDlg::CurrentAssignmentDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentAssignmentDlg),
    inm(ChannelListModel::In | ChannelListModel::Blank, this),
    outm(ChannelListModel::Out | ChannelListModel::Blank, this)
{
    ui->setupUi(this);
    ui->table->setHorizontalHeaderLabels(QStringList({"Active",
                                                      "Voltage input channel",
                                                      "Current output channel"}));
    ui->table->setColumnWidth(0, 49);
    ui->table->setColumnWidth(1, 150);
    ui->table->setColumnWidth(2, 150);
    growTable();

    connect(this, SIGNAL(chnsChanged()), &inm, SLOT(updateChns()));
    connect(this, SIGNAL(chnsChanged()), &outm, SLOT(updateChns()));
}

CurrentAssignmentDlg::~CurrentAssignmentDlg()
{
    delete ui;
}

void CurrentAssignmentDlg::exportData(mhHHData & p)
{
    CurrentAssignment a;
    QPoint in, out;
    p.assign.clear();
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        in = ins[i]->currentData().toPoint();
        out = outs[i]->currentData().toPoint();
        if ( in.y() == ChannelListModel::Blank || out.y() == ChannelListModel::Blank )
            continue;
        a.active = boxes[i]->isChecked();
        a.VChannel = in.x();
        a.IChannel = out.x();
        p.assign.push_back(a);
    }
}

void CurrentAssignmentDlg::importData(mhHHData const& p)
{
    int rows = (int)p.assign.size();
    QCheckBox *box;
    QComboBox *in, *out;

    boxes.clear();
    ins.clear();
    outs.clear();
    ui->table->setRowCount(0);

    for ( int i = 0; i < rows; i++ ) {
        box = new QCheckBox();
        in = new QComboBox();
        out = new QComboBox();
        addRow(i, box, in, out);
        box->setChecked(p.assign[i].active);
        in->setCurrentIndex(inm.index(p.assign[i].VChannel));
        out->setCurrentIndex(outm.index(p.assign[i].IChannel));
    }

    growTable();
}

void CurrentAssignmentDlg::addRow(int row, QCheckBox *box, QComboBox *in, QComboBox *out)
{
    ui->table->insertRow(row);
    ui->table->setRowHeight(row, 25);

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(box);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    widget->setLayout(layout);
    ui->table->setCellWidget(row, 0, widget);

    in->setModel(&inm);
    ui->table->setCellWidget(row, 1, in);

    out->setModel(&outm);
    ui->table->setCellWidget(row, 2, out);

    boxes.insert(row, box);
    ins.insert(row, in);
    outs.insert(row, out);
}

void CurrentAssignmentDlg::growTable()
{
    disconnect(boxc);
    disconnect(inc);
    disconnect(outc);

    if ( !boxes.empty() )
        boxes.last()->setChecked(true);

    QCheckBox *box = new QCheckBox();
    QComboBox *in = new QComboBox();
    QComboBox *out = new QComboBox();

    addRow(ui->table->rowCount(), box, in, out);

    boxc = connect(box, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    inc = connect(in, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
    outc = connect(out, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
}
