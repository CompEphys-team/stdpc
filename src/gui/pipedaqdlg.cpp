#include "pipedaqdlg.h"

PipeDAQDlg::PipeDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
  setupUi(this);
  connect(inChanBtn, &QPushButton::clicked, this, &PipeDAQDlg::openInChnDlg);
  connect(outChanBtn, &QPushButton::clicked, this, &PipeDAQDlg::openOutChnDlg);
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

    getEntry<bool>(p.read, read->isChecked(), change);
    getEntry<QString>(p.readAddr, inAddress->text(), change);
    getEntry<bool>(p.bind_read, inServer->isChecked(), change);
    uint nInChans = nInChanSpin->value();
    if ( nInChans != p.inChn.size() ) {
        p.inChn.resize(nInChans);
        for ( inChnData &i : p.inChn )
            i.active = true;
        change = true;
    }

    getEntry<bool>(p.write, write->isChecked(), change);
    getEntry<QString>(p.writeAddr, outAddress->text(), change);
    getEntry<bool>(p.bind_write, outServer->isChecked(), change);
    uint nOutChans = nOutChanSpin->value();
    if ( nOutChans != p.outChn.size() ) {
        p.outChn.resize(nOutChans);
        for ( outChnData &i : p.outChn )
            i.active = true;
        change = true;
    }

    getEntry<QString>(PipeDAQProxy::p[idx].label, leLabel->text(), change);

    DAQDlg::exportData(change || forceInit);
}

void PipeDAQDlg::importData()
{
    PipeDAQData &p = PipeDAQProxy::p[idx];
    leLabel->setText(p.label);

    read->setChecked(p.read);
    inAddress->setText(p.readAddr);
    inServer->setChecked(p.bind_read);
    nInChanSpin->setValue(int(p.inChn.size()));

    write->setChecked(p.write);
    outAddress->setText(p.writeAddr);
    outServer->setChecked(p.bind_write);
    nOutChanSpin->setValue(int(p.outChn.size()));

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
