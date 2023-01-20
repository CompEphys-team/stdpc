#include "MicroManagerDAQDlg.h"

MicroManagerDAQDlg::MicroManagerDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
  setupUi(this);
  connect(channelsBtn, &QPushButton::clicked, this, [=](){
      openInChnDlg();
  });
  setIndex(idx);
}

void MicroManagerDAQDlg::setIndex(size_t no)
{
    DAQLabel->setText(QString("%1 %2").arg(MicroManagerDAQProxy::get()->prettyName()).arg(no));
    DAQDlg::setIndex(no);
}

void MicroManagerDAQDlg::exportData(bool forceInit)
{
    MicroManagerDAQData &p = MicroManagerDAQProxy::p[idx];
    bool change = false;

    uint nChans = nChannelsSpin->value();
    if ( nChans != p.inChn.size() ) {
        p.inChn.resize(nChans);
        for ( inChnData &i : p.inChn )
            i.active = true;
        change = true;
    }

    getEntry<QString>(MicroManagerDAQProxy::p[idx].label, leLabel->text(), change);

    DAQDlg::exportData(change || forceInit);
}

void MicroManagerDAQDlg::importData()
{
    MicroManagerDAQData &p = MicroManagerDAQProxy::p[idx];
    leLabel->setText(p.label);
    nChannelsSpin->setValue(p.inChn.size());

    DAQDlg::importData();
}

void MicroManagerDAQDlg::backup()
{
    bak = MicroManagerDAQProxy::p[idx];
}

void MicroManagerDAQDlg::restoreBackup()
{
  MicroManagerDAQProxy::p[idx] = bak;
}
