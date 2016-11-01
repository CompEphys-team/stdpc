#include "DAQDlg.h"

DAQDlg::DAQDlg(int no, QWidget *parent) :
    QDialog(parent),
    idx(no),
    inDlg(new InputChannelDlg(this)),
    outDlg(new OutputChannelDlg(this))
{
}
