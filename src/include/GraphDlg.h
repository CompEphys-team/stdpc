#ifndef GRAPHDLG_H
#define GRAPHDLG_H

#include <QComboBox>
#include "ui_GraphDlg.h"
#include "ObjectDataTypes.h"
#include "ChannelListModel.h"

class GraphDlg : public QDialog, private Ui::GraphDlg
{
     Q_OBJECT

  private:
    int myNo;
    int inChnNo;
    int outChnNo;      
    QComboBox *clrCombo[4];
    QComboBox *ChannelCombo[4];
    QLineEdit *MinE[4];
    QLineEdit *MaxE[4];
    QComboBox *UnitCombo[4];
    ChannelListModel *clm;

  public:
     GraphDlg(int, QWidget *parent= 0);
     void exportData(graphData &);
     void importData(graphData);
}; 

#endif
