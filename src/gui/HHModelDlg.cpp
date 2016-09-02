#include "HHModelDlg.h"
#include "ui_HHModelDlg.h"
#include "Global.h"

#define BTNSTRING "Instances..."

/// Developer's note: In the interest of channel index stability, do not allow removing/reordering rows

HHModelDlg::HHModelDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HHModelDlg)
{
    ui->setupUi(this);
    ui->table->setHorizontalHeaderLabels(QStringList({"Active",
                                                      "Capacitance (nF)",
                                                      "g Leak (nS)",
                                                      "E Leak (mV)",
                                                      "Instances"}));
    ui->table->setColumnWidth(0, 49);
    ui->table->setColumnWidth(1, 100);
    ui->table->setColumnWidth(2, 100);
    ui->table->setColumnWidth(3, 100);
    ui->table->setColumnWidth(4, 120);
    growTable();
}

HHModelDlg::~HHModelDlg()
{
    delete ui;
}

void HHModelDlg::importData()
{
    int row = 0;
    QCheckBox *box;
    QDoubleSpinBox *C, *gLeak, *ELeak;
    QPushButton *btn;

    boxes.clear();
    Cs.clear();
    gLeaks.clear();
    ELeaks.clear();
    for ( ModelInstDlg *i : instDlg )
        delete i;
    instDlg.clear();
    ui->table->setRowCount(0);

    for ( HHNeuronData &model : HHNeuronp ) {
        box = new QCheckBox();
        C = new QDoubleSpinBox();
        gLeak = new QDoubleSpinBox();
        ELeak = new QDoubleSpinBox();
        btn = new QPushButton(BTNSTRING);
        addRow(row, box, C, gLeak, ELeak, btn);
        box->setChecked(model.active);
        C->setValue(model.C * 1e9);
        gLeak->setValue(model.gLeak * 1e9);
        ELeak->setValue(model.ELeak * 1e3);
        instDlg[row++]->importData(model.inst);
    }
    growTable();
}

void HHModelDlg::exportData()
{
    HHNeuronData p;
    HHNeuronp.clear();
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        p.active = boxes[i]->isChecked();
        p.C = Cs[i]->value() * 1e-9;
        p.gLeak = gLeaks[i]->value() * 1e-9;
        p.ELeak = ELeaks[i]->value() * 1e-3;
        instDlg[i]->exportData(p.inst);
        HHNeuronp.push_back(p);
    }
}

void HHModelDlg::accept()
{
    exportData();
    QDialog::accept();
}

void HHModelDlg::reject()
{
    importData();
    QDialog::reject();
}

void HHModelDlg::addRow(int row, QCheckBox *box, QDoubleSpinBox *C, QDoubleSpinBox *gLeak, QDoubleSpinBox *ELeak, QPushButton *btn)
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

    ui->table->setCellWidget(row, 1, C);
    ui->table->setCellWidget(row, 2, gLeak);
    ui->table->setCellWidget(row, 3, ELeak);
    ui->table->setCellWidget(row, 4, btn);

    C->setRange(0,1e6);
    C->setDecimals(6);
    gLeak->setRange(0, 1e9);
    ELeak->setRange(-1000, 1000);

    boxes.insert(row, box);
    Cs.insert(row, C);
    gLeaks.insert(row, gLeak);
    ELeaks.insert(row, ELeak);
    instDlg.insert(row, new ModelInstDlg(this));

    connect(btn, SIGNAL(clicked(bool)), instDlg[row], SLOT(open()));
}

void HHModelDlg::growTable()
{
    disconnect(boxc);
    disconnect(Cc);
    disconnect(gLc);
    disconnect(ELc);
    disconnect(btnc);

    if ( !boxes.empty() )
        boxes.last()->setChecked(true);

    QCheckBox *box = new QCheckBox();
    QDoubleSpinBox *C = new QDoubleSpinBox();
    QDoubleSpinBox *gLeak = new QDoubleSpinBox();
    QDoubleSpinBox *ELeak = new QDoubleSpinBox();
    QPushButton *btn = new QPushButton(BTNSTRING);

    addRow(ui->table->rowCount(), box, C, gLeak, ELeak, btn);

    C->setValue(2.5);
    gLeak->setValue(20);
    ELeak->setValue(-20);

    boxc = connect(box, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    Cc = connect(C, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
    gLc = connect(gLeak, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
    ELc = connect(ELeak, SIGNAL(valueChanged(double)), this, SLOT(growTable()));
    btnc = connect(btn, SIGNAL(clicked(bool)), this, SLOT(growTable()));
}
