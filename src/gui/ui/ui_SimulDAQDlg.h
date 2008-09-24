/********************************************************************************
** Form generated from reading ui file 'SimulDAQDlg.ui'
**
** Created: Wed Sep 24 11:44:56 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SIMULDAQDLG_H
#define UI_SIMULDAQDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_SimulDAQDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *InputFileE;
    QLabel *InputFileL;
    QLineEdit *OutputFileE;
    QLabel *OutputFileL;
    QLineEdit *inChannelE;
    QLabel *inChannelL;
    QLabel *outChannelL;
    QLineEdit *outChannelE;
    QLineEdit *outDtE;
    QLabel *outDtL;
    QLabel *outDtU;
    QLineEdit *inTFacE;
    QLabel *inTFacU;
    QLabel *InTFacL;

    void setupUi(QDialog *SimulDAQDlg)
    {
    if (SimulDAQDlg->objectName().isEmpty())
        SimulDAQDlg->setObjectName(QString::fromUtf8("SimulDAQDlg"));
    SimulDAQDlg->resize(400, 217);
    buttonBox = new QDialogButtonBox(SimulDAQDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(221, 170, 160, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    InputFileE = new QLineEdit(SimulDAQDlg);
    InputFileE->setObjectName(QString::fromUtf8("InputFileE"));
    InputFileE->setGeometry(QRect(100, 30, 161, 20));
    InputFileL = new QLabel(SimulDAQDlg);
    InputFileL->setObjectName(QString::fromUtf8("InputFileL"));
    InputFileL->setGeometry(QRect(40, 30, 45, 16));
    OutputFileE = new QLineEdit(SimulDAQDlg);
    OutputFileE->setObjectName(QString::fromUtf8("OutputFileE"));
    OutputFileE->setGeometry(QRect(100, 60, 161, 20));
    OutputFileL = new QLabel(SimulDAQDlg);
    OutputFileL->setObjectName(QString::fromUtf8("OutputFileL"));
    OutputFileL->setGeometry(QRect(30, 60, 53, 16));
    inChannelE = new QLineEdit(SimulDAQDlg);
    inChannelE->setObjectName(QString::fromUtf8("inChannelE"));
    inChannelE->setGeometry(QRect(100, 100, 51, 20));
    inChannelL = new QLabel(SimulDAQDlg);
    inChannelL->setObjectName(QString::fromUtf8("inChannelL"));
    inChannelL->setGeometry(QRect(20, 100, 66, 16));
    outChannelL = new QLabel(SimulDAQDlg);
    outChannelL->setObjectName(QString::fromUtf8("outChannelL"));
    outChannelL->setGeometry(QRect(10, 130, 73, 16));
    outChannelE = new QLineEdit(SimulDAQDlg);
    outChannelE->setObjectName(QString::fromUtf8("outChannelE"));
    outChannelE->setGeometry(QRect(100, 130, 51, 20));
    outDtE = new QLineEdit(SimulDAQDlg);
    outDtE->setObjectName(QString::fromUtf8("outDtE"));
    outDtE->setGeometry(QRect(300, 130, 48, 20));
    outDtL = new QLabel(SimulDAQDlg);
    outDtL->setObjectName(QString::fromUtf8("outDtL"));
    outDtL->setGeometry(QRect(240, 130, 47, 16));
    outDtU = new QLabel(SimulDAQDlg);
    outDtU->setObjectName(QString::fromUtf8("outDtU"));
    outDtU->setGeometry(QRect(360, 130, 16, 16));
    inTFacE = new QLineEdit(SimulDAQDlg);
    inTFacE->setObjectName(QString::fromUtf8("inTFacE"));
    inTFacE->setGeometry(QRect(300, 100, 48, 20));
    inTFacU = new QLabel(SimulDAQDlg);
    inTFacU->setObjectName(QString::fromUtf8("inTFacU"));
    inTFacU->setGeometry(QRect(360, 100, 16, 16));
    InTFacL = new QLabel(SimulDAQDlg);
    InTFacL->setObjectName(QString::fromUtf8("InTFacL"));
    InTFacL->setGeometry(QRect(180, 100, 111, 20));

    retranslateUi(SimulDAQDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), SimulDAQDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), SimulDAQDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(SimulDAQDlg);
    } // setupUi

    void retranslateUi(QDialog *SimulDAQDlg)
    {
    SimulDAQDlg->setWindowTitle(QApplication::translate("SimulDAQDlg", "Simulated DAQ Setup", 0, QApplication::UnicodeUTF8));
    InputFileE->setText(QApplication::translate("SimulDAQDlg", "c:\\StdpcIn1short.dat", 0, QApplication::UnicodeUTF8));
    InputFileL->setText(QApplication::translate("SimulDAQDlg", "Input File", 0, QApplication::UnicodeUTF8));
    OutputFileE->setText(QApplication::translate("SimulDAQDlg", "c:\\StdpCOut1.dat", 0, QApplication::UnicodeUTF8));
    OutputFileL->setText(QApplication::translate("SimulDAQDlg", "Output File", 0, QApplication::UnicodeUTF8));
    inChannelE->setText(QApplication::translate("SimulDAQDlg", "2", 0, QApplication::UnicodeUTF8));
    inChannelL->setText(QApplication::translate("SimulDAQDlg", "# in Channels", 0, QApplication::UnicodeUTF8));
    outChannelL->setText(QApplication::translate("SimulDAQDlg", "# out Channels", 0, QApplication::UnicodeUTF8));
    outChannelE->setText(QApplication::translate("SimulDAQDlg", "2", 0, QApplication::UnicodeUTF8));
    outDtE->setText(QApplication::translate("SimulDAQDlg", "0.1", 0, QApplication::UnicodeUTF8));
    outDtL->setText(QApplication::translate("SimulDAQDlg", "Output dt", 0, QApplication::UnicodeUTF8));
    outDtU->setText(QApplication::translate("SimulDAQDlg", "ms", 0, QApplication::UnicodeUTF8));
    inTFacE->setText(QApplication::translate("SimulDAQDlg", "0.001", 0, QApplication::UnicodeUTF8));
    inTFacU->setText(QApplication::translate("SimulDAQDlg", "s", 0, QApplication::UnicodeUTF8));
    InTFacL->setText(QApplication::translate("SimulDAQDlg", "Input t conversion  1 =", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SimulDAQDlg);
    } // retranslateUi

};

namespace Ui {
    class SimulDAQDlg: public Ui_SimulDAQDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMULDAQDLG_H
