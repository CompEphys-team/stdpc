#include "CurrentAssignmentDlg.h"
#include "ui_CurrentAssignmentDlg.h"
#include <QHBoxLayout>

CurrentAssignmentDlg::CurrentAssignmentDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentAssignmentDlg),
    inm(ChannelListModel::In | ChannelListModel::Blank, this),
    outm(ChannelListModel::Out | ChannelListModel::Blank, this),
    reloadChns(true)
{
    ui->setupUi(this);
    ui->table->setHorizontalHeaderLabels(QStringList({"Active",
                                                      "Voltage input channel",
                                                      "Current output channel"}));
    ui->table->setColumnWidth(0, 49);
    ui->table->setColumnWidth(1, 150);
    ui->table->setColumnWidth(2, 150);
    growTable();
}

CurrentAssignmentDlg::~CurrentAssignmentDlg()
{
    delete ui;
}

void CurrentAssignmentDlg::exportData(std::vector<CurrentAssignment> &p)
{
    CurrentAssignment a;
    QPoint in, out;
    p.clear();
    inm.updateChns();
    outm.updateChns();
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        in = ins[i]->currentData().toPoint();
        out = outs[i]->currentData().toPoint();
        if ( in.y() <= ChannelListModel::None || out.y() <= ChannelListModel::None )
            continue;
        a.active = boxes[i]->isChecked();
        a.VChannel = in.x();
        a.IChannel = out.x();
        p.push_back(a);
    }
}

void CurrentAssignmentDlg::importData(const std::vector<CurrentAssignment> &p)
{
    QCheckBox *box;
    QComboBox *in, *out;

    boxes.clear();
    ins.clear();
    outs.clear();
    ui->table->setRowCount(0);
    inm.updateChns();
    outm.updateChns();

    int i = 0;
    for ( CurrentAssignment const& a : p ) {
        box = new QCheckBox();
        in = new QComboBox();
        out = new QComboBox();
        addRow(i++, box, in, out);
        box->setChecked(a.active);
        in->setCurrentIndex(inm.index(a.VChannel));
        out->setCurrentIndex(outm.index(a.IChannel));
    }

    growTable();
}

void CurrentAssignmentDlg::open()
{
    if ( reloadChns ) {
        inm.updateChns();
        outm.updateChns();
        reloadChns = false;
    }
    QDialog::open();
}

void CurrentAssignmentDlg::updateChns()
{
    reloadChns = true;
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
