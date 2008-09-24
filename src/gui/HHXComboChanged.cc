void MyMainWindow::HH0ComboChanged()
{
  int index= HH0Combo->currentIndex();
  HH0ParaBut->disconnect();
  nHHDlg[0]->hide();
  abHHDlg[0]->hide();
  if (index == 0) {
    HH0L->setEnabled(false);
    HH0ParaBut->setEnabled(false);
  }
  else {
    HH0L->setEnabled(true);
    HH0ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(HH0ParaBut, SIGNAL(clicked()), nHHDlg[0], SLOT(show()));
  }
  if (index == 2) {
    connect(HH0ParaBut, SIGNAL(clicked()), abHHDlg[0], SLOT(show()));
  }            
}

void MyMainWindow::HH1ComboChanged()
{
  int index= HH1Combo->currentIndex();
  HH1ParaBut->disconnect();
  nHHDlg[1]->hide();
  abHHDlg[1]->hide();
  if (index == 0) {
    HH1L->setEnabled(false);
    HH1ParaBut->setEnabled(false);
  }
  else {
    HH1L->setEnabled(true);
    HH1ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(HH1ParaBut, SIGNAL(clicked()), nHHDlg[1], SLOT(show()));
  }
  if (index == 2) {
    connect(HH1ParaBut, SIGNAL(clicked()), abHHDlg[1], SLOT(show()));
  }            
}

void MyMainWindow::HH2ComboChanged()
{
  int index= HH2Combo->currentIndex();
  HH2ParaBut->disconnect();
  nHHDlg[2]->hide();
  abHHDlg[2]->hide();
  if (index == 0) {
    HH2L->setEnabled(false);
    HH2ParaBut->setEnabled(false);
  }
  else {
    HH2L->setEnabled(true);
    HH2ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(HH2ParaBut, SIGNAL(clicked()), nHHDlg[2], SLOT(show()));
  }
  if (index == 2) {
    connect(HH2ParaBut, SIGNAL(clicked()), abHHDlg[2], SLOT(show()));
  }            
}

void MyMainWindow::HH3ComboChanged()
{
  int index= HH3Combo->currentIndex();
  HH3ParaBut->disconnect();
  nHHDlg[3]->hide();
  abHHDlg[3]->hide();
  if (index == 0) {
    HH3L->setEnabled(false);
    HH3ParaBut->setEnabled(false);
  }
  else {
    HH3L->setEnabled(true);
    HH3ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(HH3ParaBut, SIGNAL(clicked()), nHHDlg[3], SLOT(show()));
  }
  if (index == 2) {
    connect(HH3ParaBut, SIGNAL(clicked()), abHHDlg[3], SLOT(show()));
  }            
}

void MyMainWindow::HH4ComboChanged()
{
  int index= HH4Combo->currentIndex();
  HH4ParaBut->disconnect();
  nHHDlg[4]->hide();
  abHHDlg[4]->hide();
  if (index == 0) {
    HH4L->setEnabled(false);
    HH4ParaBut->setEnabled(false);
  }
  else {
    HH4L->setEnabled(true);
    HH4ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(HH4ParaBut, SIGNAL(clicked()), nHHDlg[4], SLOT(show()));
  }
  if (index == 2) {
    connect(HH4ParaBut, SIGNAL(clicked()), abHHDlg[4], SLOT(show()));
  }            
}

void MyMainWindow::HH5ComboChanged()
{
  int index= HH5Combo->currentIndex();
  HH5ParaBut->disconnect();
  nHHDlg[5]->hide();
  abHHDlg[5]->hide();
  if (index == 0) {
    HH5L->setEnabled(false);
    HH5ParaBut->setEnabled(false);
  }
  else {
    HH5L->setEnabled(true);
    HH5ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(HH5ParaBut, SIGNAL(clicked()), nHHDlg[5], SLOT(show()));
  }
  if (index == 2) {
    connect(HH5ParaBut, SIGNAL(clicked()), abHHDlg[5], SLOT(show()));
  }            
}
