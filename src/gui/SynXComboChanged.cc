void MyMainWindow::Syn0ComboChanged()
{
  int index= Syn0Combo->currentIndex();
  Syn0ParaBut->disconnect();
  CSynDlg[0]->hide();
  abSDlg[0]->hide();
  GJunctDlg[0]->hide();
  if (index == 0) {
    Syn0L->setEnabled(false);
    Syn0ParaBut->setEnabled(false);
  }
  else {
    Syn0L->setEnabled(true);
    Syn0ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(Syn0ParaBut, SIGNAL(clicked()), CSynDlg[0], SLOT(show()));
  }
  if (index == 2) {
    connect(Syn0ParaBut, SIGNAL(clicked()), GJunctDlg[0], SLOT(show()));
  }
  if (index == 3) {
    connect(Syn0ParaBut, SIGNAL(clicked()), abSDlg[0], SLOT(show()));
  }               
}

void MyMainWindow::Syn1ComboChanged()
{
  int index= Syn1Combo->currentIndex();
  Syn1ParaBut->disconnect();
  CSynDlg[1]->hide();
  abSDlg[1]->hide();
  GJunctDlg[1]->hide();
  if (index == 0) {
    Syn1L->setEnabled(false);
    Syn1ParaBut->setEnabled(false);
  }
  else {
    Syn1L->setEnabled(true);
    Syn1ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(Syn1ParaBut, SIGNAL(clicked()), CSynDlg[1], SLOT(show()));
  }
  if (index == 2) {
    connect(Syn1ParaBut, SIGNAL(clicked()), GJunctDlg[1], SLOT(show()));
  }
  if (index == 3) {
    connect(Syn1ParaBut, SIGNAL(clicked()), abSDlg[1], SLOT(show()));
  }               
}

void MyMainWindow::Syn2ComboChanged()
{
  int index= Syn2Combo->currentIndex();
  Syn2ParaBut->disconnect();
  CSynDlg[2]->hide();
  abSDlg[2]->hide();
  GJunctDlg[2]->hide();
  if (index == 0) {
    Syn2L->setEnabled(false);
    Syn2ParaBut->setEnabled(false);
  }
  else {
    Syn2L->setEnabled(true);
    Syn2ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(Syn2ParaBut, SIGNAL(clicked()), CSynDlg[2], SLOT(show()));
  }
  if (index == 2) {
    connect(Syn2ParaBut, SIGNAL(clicked()), GJunctDlg[2], SLOT(show()));
  }   
  if (index == 3) {
    connect(Syn2ParaBut, SIGNAL(clicked()), abSDlg[2], SLOT(show()));
  }            
}

void MyMainWindow::Syn3ComboChanged()
{
  int index= Syn3Combo->currentIndex();
  Syn3ParaBut->disconnect();
  CSynDlg[3]->hide();
  abSDlg[3]->hide();
  GJunctDlg[3]->hide();
  if (index == 0) {
    Syn3L->setEnabled(false);
    Syn3ParaBut->setEnabled(false);
  }
  else {
    Syn3L->setEnabled(true);
    Syn3ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(Syn3ParaBut, SIGNAL(clicked()), CSynDlg[3], SLOT(show()));
  }
  if (index == 2) {
    connect(Syn3ParaBut, SIGNAL(clicked()), GJunctDlg[3], SLOT(show()));
  }   
  if (index == 3) {
    connect(Syn3ParaBut, SIGNAL(clicked()), abSDlg[3], SLOT(show()));
  }            
}

void MyMainWindow::Syn4ComboChanged()
{
  int index= Syn4Combo->currentIndex();
  Syn4ParaBut->disconnect();
  CSynDlg[4]->hide();
  abSDlg[4]->hide();
  GJunctDlg[4]->hide();
  if (index == 0) {
    Syn4L->setEnabled(false);
    Syn4ParaBut->setEnabled(false);
  }
  else {
    Syn4L->setEnabled(true);
    Syn4ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(Syn4ParaBut, SIGNAL(clicked()), CSynDlg[4], SLOT(show()));
  }
  if (index == 2) {
    connect(Syn4ParaBut, SIGNAL(clicked()), GJunctDlg[4], SLOT(show()));
  } 
  if (index == 3) {
    connect(Syn4ParaBut, SIGNAL(clicked()), abSDlg[4], SLOT(show()));
  }              
}

void MyMainWindow::Syn5ComboChanged()
{
  int index= Syn5Combo->currentIndex();
  Syn5ParaBut->disconnect();
  CSynDlg[5]->hide();
  abSDlg[5]->hide();
  GJunctDlg[5]->hide();
  if (index == 0) {
    Syn5L->setEnabled(false);
    Syn5ParaBut->setEnabled(false);
  }
  else {
    Syn5L->setEnabled(true);       
    Syn5ParaBut->setEnabled(true);
  }
  if (index == 1) {
    connect(Syn5ParaBut, SIGNAL(clicked()), CSynDlg[5], SLOT(show()));
  }
  if (index == 2) {
    connect(Syn5ParaBut, SIGNAL(clicked()), GJunctDlg[5], SLOT(show()));
  }       
  if (index == 3) {
    connect(Syn5ParaBut, SIGNAL(clicked()), abSDlg[5], SLOT(show()));
  }        
}
