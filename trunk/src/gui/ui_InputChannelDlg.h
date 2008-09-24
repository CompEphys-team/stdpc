/********************************************************************************
** Form generated from reading ui file 'InputChannelDlg.ui'
**
** Created: Mon Aug 18 17:20:46 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_INPUTCHANNELDLG_H
#define UI_INPUTCHANNELDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_InputChannelDlg
{
public:
    QDialogButtonBox *buttonBox;
    QFrame *frame;
    QLabel *label;

    void setupUi(QDialog *InputChannelDlg)
    {
    if (InputChannelDlg->objectName().isEmpty())
        InputChannelDlg->setObjectName(QString::fromUtf8("InputChannelDlg"));
    InputChannelDlg->resize(613, 500);
    buttonBox = new QDialogButtonBox(InputChannelDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(420, 450, 160, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    frame = new QFrame(InputChannelDlg);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setGeometry(QRect(470, 20, 111, 21));
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
    label->setGeometry(QRect(10, 0, 97, 18));

    retranslateUi(InputChannelDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), InputChannelDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), InputChannelDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(InputChannelDlg);
    } // setupUi

    void retranslateUi(QDialog *InputChannelDlg)
    {
    InputChannelDlg->setWindowTitle(QApplication::translate("InputChannelDlg", "Input Channels", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("InputChannelDlg", "Input Channels", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(InputChannelDlg);
    } // retranslateUi

};

namespace Ui {
    class InputChannelDlg: public Ui_InputChannelDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTCHANNELDLG_H
