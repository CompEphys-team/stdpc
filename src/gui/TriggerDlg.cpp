#include "TriggerDlg.h"
#include "ui_TriggerDlg.h"
#include "DeviceManager.h"

TriggerDlg::TriggerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TriggerDlg)
{
    ui->setupUi(this);
    connect(&Devices, SIGNAL(removedDevice(ChannelIndex)), this, SLOT(updateChannels()));
}

TriggerDlg::~TriggerDlg()
{
    delete ui;
}

void TriggerDlg::exportData()
{
    Triggerp.channel = ui->channel->currentData().value<ChannelIndex>();
    Triggerp.active = ui->active->isChecked();
}

void TriggerDlg::importData()
{
    for ( int i = 0; i < ui->channel->count(); i++ )
        if ( ui->channel->itemData(i).value<ChannelIndex>() == Triggerp.channel )
            ui->channel->setCurrentIndex(i);
    ui->active->setChecked(Triggerp.active);
}

void TriggerDlg::updateChannels()
{
    ChannelIndex currentChannel = ui->channel->currentData().value<ChannelIndex>();
    ui->channel->clear();
    for ( auto vec : Devices.all() ) {
        for ( std::shared_ptr<DAQ> daqp : vec ) {
            for ( int i = 0; i < daqp->digInChnNo; i++ ) {
                ChannelIndex tmp(daqp->proxy, daqp->devID, i, true);
                tmp.isDigital = true;
                tmp.isAnalog = false;
                ui->channel->addItem(tmp.prettyName(), QVariant::fromValue(tmp));
            }
        }
    }
    for ( int i = 0; i < ui->channel->count(); i++ )
        if ( ui->channel->itemData(i).value<ChannelIndex>() == currentChannel )
            ui->channel->setCurrentIndex(i);
}
