/********************************************************************************
** Form generated from reading ui file 'OutputChannelDlg.ui'
**
** Created: Mon Aug 18 17:20:46 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OUTPUTCHANNELDLG_H
#define UI_OUTPUTCHANNELDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_OutputChannelDlg
{
public:
    QDialogButtonBox *buttonBox;
    QFrame *frame;
    QLabel *label;

    void setupUi(QDialog *OutputChannelDlg)
    {
    if (OutputChannelDlg->objectName().isEmpty())
        OutputChannelDlg->setObjectName(QString::fromUtf8("OutputChannelDlg"));
    OutputChannelDlg->resize(455, 203);
    buttonBox = new QDialogButtonBox(OutputChannelDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(260, 150, 160, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    frame = new QFrame(OutputChannelDlg);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setGeometry(QRect(290, 20, 131, 21));
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);
    frame->setFont(font);
    frame->setAutoFillBackground(false);
    frame->setFrameShape(QFrame::Panel);
    frame->setFrameShadow(QFrame::Raised);
    label = new QLabel(frame);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(10, 0, 107, 18));

    retranslateUi(OutputChannelDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), OutputChannelDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), OutputChannelDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(OutputChannelDlg);
    } // setupUi

    void retranslateUi(QDialog *OutputChannelDlg)
    {
    OutputChannelDlg->setWindowTitle(QApplication::translate("OutputChannelDlg", "Output Channels", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("OutputChannelDlg", "Output Channels", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(OutputChannelDlg);
    } // retranslateUi

};

namespace Ui {
    class OutputChannelDlg: public Ui_OutputChannelDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUTCHANNELDLG_H
