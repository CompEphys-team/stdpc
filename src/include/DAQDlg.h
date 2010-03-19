#ifndef DAQDLG_H
#define DAQDLG_H

#include <QDialog>

class DAQDlg : public QDialog
{
     Q_OBJECT

  public:
     DAQDlg(QWidget *parent= 0);
          
  public slots:
    virtual void appear()= 0;
    
  signals:
    void reinitDAQ();

}; 

#endif



