#include "pipedaqdlg.h"

PipeDAQDlg::PipeDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
  setupUi(this);
  connect(channelsBtn, &QPushButton::clicked, this, [=](){
      if ( readCombo->currentIndex() )
          openInChnDlg();
      else
          openOutChnDlg();
  });
  setIndex(idx);
}

void PipeDAQDlg::setIndex(size_t no)
{
    DAQLabel->setText(QString("%1 %2").arg(PipeDAQProxy::get()->prettyName()).arg(no));
    DAQDlg::setIndex(no);
}

void PipeDAQDlg::exportData(bool forceInit)
{
    PipeDAQData &p = PipeDAQProxy::p[idx];
    bool change = false;

    uint nChans = nChannelsSpin->value();
    getEntry<bool>(p.read, readCombo->currentIndex(), change);
    if ( change || (p.read && nChans != p.inChn.size()) || (!p.read && nChans != p.outChn.size()) ) {
        if ( p.read ) {
            p.inChn.resize(nChans);
            for ( inChnData &i : p.inChn )
                i.active = true;
        } else {
            p.outChn.resize(nChans);
            for ( outChnData &i : p.outChn )
                i.active = true;
        }
        change = true;
    }

    getEntry<QString>(p.pipeName, pipenameE->text(), change);
    getEntry<QString>(p.hostName, hostnameE->text(), change);
    getEntry(p.server, serverCheck->isChecked(), change);
    getEntry(p.connectLater, connectLaterCheck->isChecked(), change);

    getEntry<QString>(PipeDAQProxy::p[idx].label, leLabel->text(), change);

    DAQDlg::exportData(change || forceInit);
}

void PipeDAQDlg::importData()
{
    PipeDAQData &p = PipeDAQProxy::p[idx];
    leLabel->setText(p.label);
    pipenameE->setText(p.pipeName);
    hostnameE->setText(p.hostName);
    serverCheck->setChecked(p.server);
    connectLaterCheck->setChecked(p.connectLater);
    readCombo->setCurrentIndex(p.read);
    nChannelsSpin->setValue(p.read ? p.inChn.size() : p.outChn.size());

    DAQDlg::importData();
}

void PipeDAQDlg::backup()
{
    bak = PipeDAQProxy::p[idx];
}

void PipeDAQDlg::restoreBackup()
{
  PipeDAQProxy::p[idx] = bak;
}
