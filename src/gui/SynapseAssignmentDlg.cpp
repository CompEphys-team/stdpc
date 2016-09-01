#include "SynapseAssignmentDlg.h"
#include "ui_SynapseAssignmentDlg.h"
#include <QHBoxLayout>

SynapseAssignmentDlg::SynapseAssignmentDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SynapseAssignmentDlg),
    inm(ChannelListModel::In | ChannelListModel::Blank, this),
    outm(ChannelListModel::Out | ChannelListModel::Blank, this),
    reloadChns(true)
{
    ui->setupUi(this);
    ui->table->setHorizontalHeaderLabels(QStringList({"Active",
                                                      "Presynaptic voltage",
                                                      "Postsynaptic voltage",
                                                      "Current output channel"}));
    ui->table->setColumnWidth(0, 49);
    ui->table->setColumnWidth(1, 150);
    ui->table->setColumnWidth(2, 150);
    ui->table->setColumnWidth(3, 150);
    growTable();
}

SynapseAssignmentDlg::~SynapseAssignmentDlg()
{
    delete ui;
}

void SynapseAssignmentDlg::exportData(std::vector<SynapseAssignment> &p)
{
    SynapseAssignment a;
    QPoint pre, post, out;
    p.clear();
    inm.updateChns();
    outm.updateChns();
    for ( int i = 0; i < ui->table->rowCount() - 1; i++ ) {
        pre = pres[i]->currentData().toPoint();
        post = posts[i]->currentData().toPoint();
        out = outs[i]->currentData().toPoint();
        if ( pre.y() <= ChannelListModel::None
             || post.y() <= ChannelListModel::None
             || out.y() <= ChannelListModel::None )
            continue;
        a.active = boxes[i]->isChecked();
        a.PreSynChannel = pre.x();
        a.PostSynChannel = post.x();
        a.OutSynChannel = out.x();
        p.push_back(a);
    }
}

void SynapseAssignmentDlg::importData(const std::vector<SynapseAssignment> &p)
{
    QCheckBox *box;
    QComboBox *pre, *post, *out;

    boxes.clear();
    pres.clear();
    posts.clear();
    outs.clear();
    ui->table->setRowCount(0);
    inm.disconnect();
    outm.disconnect();
    inm.updateChns();
    outm.updateChns();

    int i = 0;
    for ( SynapseAssignment const& a : p ) {
        box = new QCheckBox();
        pre = new QComboBox();
        post = new QComboBox();
        out = new QComboBox();
        addRow(i++, box, pre, post, out);
        box->setChecked(a.active);
        pre->setCurrentIndex(inm.index(a.PreSynChannel));
        post->setCurrentIndex(inm.index(a.PostSynChannel));
        out->setCurrentIndex(outm.index(a.OutSynChannel));
        ChannelListModel::fixComboBoxWidth(pre);
        ChannelListModel::fixComboBoxWidth(post);
        ChannelListModel::fixComboBoxWidth(out);
    }

    growTable();
}

void SynapseAssignmentDlg::open()
{
    if ( reloadChns ) {
        inm.updateChns();
        outm.updateChns();
        reloadChns = false;
    }
    QDialog::open();
}

void SynapseAssignmentDlg::updateChns()
{
    reloadChns = true;
}

void SynapseAssignmentDlg::addRow(int row, QCheckBox *box, QComboBox *pre, QComboBox *post, QComboBox *out)
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

    pre->setModel(&inm);
    connect(&inm, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(pre);});
    ui->table->setCellWidget(row, 1, pre);
    ChannelListModel::fixComboBoxWidth(pre);

    post->setModel(&inm);
    connect(&inm, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(post);});
    ui->table->setCellWidget(row, 2, post);
    ChannelListModel::fixComboBoxWidth(post);

    out->setModel(&outm);
    connect(&outm, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(out);});
    ui->table->setCellWidget(row, 3, out);
    ChannelListModel::fixComboBoxWidth(out);

    boxes.insert(row, box);
    pres.insert(row, pre);
    posts.insert(row, post);
    outs.insert(row, out);
}

void SynapseAssignmentDlg::growTable()
{
    disconnect(boxc);
    disconnect(prec);
    disconnect(postc);
    disconnect(outc);

    if ( !boxes.empty() )
        boxes.last()->setChecked(true);

    QCheckBox *box = new QCheckBox();
    QComboBox *pre = new QComboBox();
    QComboBox *post = new QComboBox();
    QComboBox *out = new QComboBox();

    addRow(ui->table->rowCount(), box, pre, post, out);

    boxc = connect(box, SIGNAL(stateChanged(int)), this, SLOT(growTable()));
    prec = connect(pre, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
    postc = connect(post, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
    outc = connect(out, SIGNAL(currentIndexChanged(int)), this, SLOT(growTable()));
}
