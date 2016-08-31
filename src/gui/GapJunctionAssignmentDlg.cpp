#include "GapJunctionAssignmentDlg.h"
#include "ui_GapJunctionAssignmentDlg.h"

GapJunctionAssignmentDlg::GapJunctionAssignmentDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GapJunctionAssignmentDlg),
    inm(ChannelListModel::In | ChannelListModel::Blank, this),
    outm(ChannelListModel::Out | ChannelListModel::Blank | ChannelListModel::None, this),
    reloadChns(true)
{
    ui->setupUi(this);
    ui->table->setHorizontalHeaderLabels(QStringList({"Active",
                                                      "Presynaptic voltage",
                                                      "Postsynaptic voltage",
                                                      "Presynaptic output channel",
                                                      "Postsynaptic output channel"}));
    ui->table->setColumnWidth(0, 49);
    ui->table->setColumnWidth(1, 150);
    ui->table->setColumnWidth(2, 150);
    ui->table->setColumnWidth(3, 150);
    ui->table->setColumnWidth(4, 150);
    growTable();
}

GapJunctionAssignmentDlg::~GapJunctionAssignmentDlg()
{
    delete ui;
}

void GapJunctionAssignmentDlg::exportData(std::vector<GapJunctionAssignment> &p)
{
    GapJunctionAssignment a;
    QPoint prein, postin, preout, postout;
    p.clear();
    inm.updateChns();
    outm.updateChns();
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        prein = preins[i]->currentData().toPoint();
        postin = postins[i]->currentData().toPoint();
        preout = preouts[i]->currentData().toPoint();
        postout= postouts[i]->currentData().toPoint();
        if ( prein.y() <= ChannelListModel::None
             || postin.y() <= ChannelListModel::None
             || preout.y() <= ChannelListModel::Blank
             || postout.y() <= ChannelListModel::Blank )
            continue;
        a.active = boxes[i]->isChecked();
        a.preInChannel = prein.x();
        a.postInChannel = postin.x();
        a.preOutChannel = preout.x();
        a.postOutChannel = postout.x();
        p.push_back(a);
    }
}

void GapJunctionAssignmentDlg::importData(const std::vector<GapJunctionAssignment> &p)
{
    QCheckBox *box;
    QComboBox *prein, *postin, *preout, *postout;

    boxes.clear();
    preins.clear();
    postins.clear();
    preouts.clear();
    postouts.clear();
    ui->table->setRowCount(0);
    inm.disconnect();
    outm.disconnect();

    int i = 0;
    for ( GapJunctionAssignment const& a : p ) {
        box = new QCheckBox();
        prein = new QComboBox();
        postin = new QComboBox();
        preout = new QComboBox();
        postout = new QComboBox();
        addRow(i++, box, prein, postin, preout, postout);
        box->setChecked(a.active);
        prein->setCurrentIndex(inm.index(a.preInChannel));
        postin->setCurrentIndex(inm.index(a.postInChannel));
        preout->setCurrentIndex(outm.index(a.preOutChannel));
        postout->setCurrentIndex(outm.index(a.postOutChannel));
        ChannelListModel::fixComboBoxWidth(prein);
        ChannelListModel::fixComboBoxWidth(postin);
        ChannelListModel::fixComboBoxWidth(preout);
        ChannelListModel::fixComboBoxWidth(postout);
    }

    growTable();
}

void GapJunctionAssignmentDlg::open()
{
    if ( reloadChns ) {
        inm.updateChns();
        outm.updateChns();
        reloadChns = false;
    }
    QDialog::open();
}

void GapJunctionAssignmentDlg::updateChns()
{
    reloadChns = true;
}

void GapJunctionAssignmentDlg::addRow(int row, QCheckBox *box,
                                  QComboBox *prein, QComboBox *postin,
                                  QComboBox *preout, QComboBox *postout)
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

    prein->setModel(&inm);
    connect(&inm, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(prein);});
    ui->table->setCellWidget(row, 1, prein);
    ChannelListModel::fixComboBoxWidth(prein);

    postin->setModel(&inm);
    connect(&inm, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(postin);});
    ui->table->setCellWidget(row, 2, postin);
    ChannelListModel::fixComboBoxWidth(postin);

    preout->setModel(&outm);
    connect(&outm, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(preout);});
    ui->table->setCellWidget(row, 3, preout);
    ChannelListModel::fixComboBoxWidth(preout);

    postout->setModel(&outm);
    connect(&outm, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(postout);});
    ui->table->setCellWidget(row, 4, postout);
    ChannelListModel::fixComboBoxWidth(postout);

    boxes.insert(row, box);
    preins.insert(row, prein);
    postins.insert(row, postin);
    preouts.insert(row, preout);
    postouts.insert(row, postout);
}

void GapJunctionAssignmentDlg::growTable()
{
    disconnect(boxc);
    disconnect(preinc);
    disconnect(postinc);
    disconnect(preoutc);
    disconnect(postoutc);

    if ( !boxes.empty() )
        boxes.last()->setChecked(true);

    QCheckBox *box = new QCheckBox();
    QComboBox *prein = new QComboBox();
    QComboBox *postin = new QComboBox();
    QComboBox *preout = new QComboBox();
    QComboBox *postout = new QComboBox();

    addRow(ui->table->rowCount(), box, prein, postin, preout, postout);

    boxc = connect(box, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    preinc = connect(prein, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
    postinc = connect(postin, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
    preoutc = connect(preout, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
    postoutc = connect(postout, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
}
