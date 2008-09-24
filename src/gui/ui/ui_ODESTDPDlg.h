/********************************************************************************
** Form generated from reading ui file 'ODESTDPDlg.ui'
**
** Created: Wed Sep 24 11:44:56 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ODESTDPDLG_H
#define UI_ODESTDPDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ODESTDPDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label_5;
    QLineEdit *gammaE;
    QLineEdit *InitialPE;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_6;
    QLineEdit *betaDE;
    QLineEdit *InitialDE;
    QLineEdit *betaPE;
    QLineEdit *etaE;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLineEdit *highDE;
    QLabel *label_9;
    QLineEdit *lowPE;
    QLabel *label_17;
    QLineEdit *highPE;
    QLabel *label_10;
    QLineEdit *lowDE;
    QLabel *label_11;
    QLabel *label_18;
    QLabel *label_13;
    QLabel *label_23;
    QLineEdit *gMidE;
    QLineEdit *sigmoidTableMaxEntryE;
    QLabel *label_24;
    QLineEdit *sigmoidTableDgE;
    QLabel *label_12;
    QLineEdit *gSlopeE;
    QLineEdit *gMaxE;
    QLabel *sigmoidTableDgL;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *sigmoidTableMaxEntryL;
    QLabel *sigmoidTableMaxEntryU;
    QComboBox *sigmoidTableCombo;
    QLabel *sigmoidTableDgU;
    QLabel *ChemSynDlgLabel;

    void setupUi(QDialog *ODESTDPDlg)
    {
    if (ODESTDPDlg->objectName().isEmpty())
        ODESTDPDlg->setObjectName(QString::fromUtf8("ODESTDPDlg"));
    ODESTDPDlg->resize(375, 444);
    buttonBox = new QDialogButtonBox(ODESTDPDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(201, 400, 160, 32));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    buttonBox->setFont(font);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    label_5 = new QLabel(ODESTDPDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 100, 33, 16));
    gammaE = new QLineEdit(ODESTDPDlg);
    gammaE->setObjectName(QString::fromUtf8("gammaE"));
    gammaE->setGeometry(QRect(70, 130, 61, 20));
    gammaE->setMaxLength(8);
    gammaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    InitialPE = new QLineEdit(ODESTDPDlg);
    InitialPE->setObjectName(QString::fromUtf8("InitialPE"));
    InitialPE->setGeometry(QRect(70, 70, 61, 20));
    InitialPE->setMaxLength(8);
    InitialPE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_8 = new QLabel(ODESTDPDlg);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(210, 100, 32, 16));
    label_7 = new QLabel(ODESTDPDlg);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(210, 70, 39, 16));
    label_4 = new QLabel(ODESTDPDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 70, 32, 16));
    label_6 = new QLabel(ODESTDPDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(20, 130, 34, 16));
    betaDE = new QLineEdit(ODESTDPDlg);
    betaDE->setObjectName(QString::fromUtf8("betaDE"));
    betaDE->setGeometry(QRect(260, 100, 61, 20));
    betaDE->setMaxLength(8);
    betaDE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    InitialDE = new QLineEdit(ODESTDPDlg);
    InitialDE->setObjectName(QString::fromUtf8("InitialDE"));
    InitialDE->setGeometry(QRect(70, 100, 61, 20));
    InitialDE->setMaxLength(8);
    InitialDE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    betaPE = new QLineEdit(ODESTDPDlg);
    betaPE->setObjectName(QString::fromUtf8("betaPE"));
    betaPE->setGeometry(QRect(260, 70, 61, 20));
    betaPE->setMaxLength(8);
    betaPE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    etaE = new QLineEdit(ODESTDPDlg);
    etaE->setObjectName(QString::fromUtf8("etaE"));
    etaE->setGeometry(QRect(260, 130, 61, 20));
    etaE->setMaxLength(8);
    etaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_14 = new QLabel(ODESTDPDlg);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(210, 130, 32, 16));
    label_15 = new QLabel(ODESTDPDlg);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setGeometry(QRect(20, 40, 115, 16));
    label_15->setFont(font);
    label_16 = new QLabel(ODESTDPDlg);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setGeometry(QRect(20, 170, 160, 16));
    label_16->setFont(font);
    highDE = new QLineEdit(ODESTDPDlg);
    highDE->setObjectName(QString::fromUtf8("highDE"));
    highDE->setGeometry(QRect(260, 200, 61, 20));
    highDE->setMaxLength(8);
    highDE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_9 = new QLabel(ODESTDPDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(210, 200, 32, 16));
    lowPE = new QLineEdit(ODESTDPDlg);
    lowPE->setObjectName(QString::fromUtf8("lowPE"));
    lowPE->setGeometry(QRect(70, 230, 61, 20));
    lowPE->setMaxLength(8);
    lowPE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_17 = new QLabel(ODESTDPDlg);
    label_17->setObjectName(QString::fromUtf8("label_17"));
    label_17->setGeometry(QRect(210, 230, 32, 16));
    highPE = new QLineEdit(ODESTDPDlg);
    highPE->setObjectName(QString::fromUtf8("highPE"));
    highPE->setGeometry(QRect(70, 200, 61, 20));
    highPE->setMaxLength(8);
    highPE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_10 = new QLabel(ODESTDPDlg);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(20, 230, 34, 16));
    lowDE = new QLineEdit(ODESTDPDlg);
    lowDE->setObjectName(QString::fromUtf8("lowDE"));
    lowDE->setGeometry(QRect(260, 230, 61, 20));
    lowDE->setMaxLength(8);
    lowDE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_11 = new QLabel(ODESTDPDlg);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(20, 200, 33, 16));
    label_18 = new QLabel(ODESTDPDlg);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setGeometry(QRect(20, 270, 164, 16));
    label_18->setFont(font);
    label_13 = new QLabel(ODESTDPDlg);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(140, 330, 16, 16));
    QFont font1;
    font1.setBold(false);
    font1.setWeight(50);
    label_13->setFont(font1);
    label_23 = new QLabel(ODESTDPDlg);
    label_23->setObjectName(QString::fromUtf8("label_23"));
    label_23->setGeometry(QRect(20, 360, 32, 16));
    label_23->setFont(font1);
    gMidE = new QLineEdit(ODESTDPDlg);
    gMidE->setObjectName(QString::fromUtf8("gMidE"));
    gMidE->setGeometry(QRect(70, 330, 61, 20));
    gMidE->setFont(font1);
    gMidE->setMaxLength(8);
    gMidE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    sigmoidTableMaxEntryE = new QLineEdit(ODESTDPDlg);
    sigmoidTableMaxEntryE->setObjectName(QString::fromUtf8("sigmoidTableMaxEntryE"));
    sigmoidTableMaxEntryE->setEnabled(false);
    sigmoidTableMaxEntryE->setGeometry(QRect(270, 360, 61, 20));
    sigmoidTableMaxEntryE->setFont(font1);
    sigmoidTableMaxEntryE->setMaxLength(8);
    sigmoidTableMaxEntryE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_24 = new QLabel(ODESTDPDlg);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setGeometry(QRect(20, 300, 34, 16));
    label_24->setFont(font1);
    sigmoidTableDgE = new QLineEdit(ODESTDPDlg);
    sigmoidTableDgE->setObjectName(QString::fromUtf8("sigmoidTableDgE"));
    sigmoidTableDgE->setEnabled(false);
    sigmoidTableDgE->setGeometry(QRect(270, 330, 61, 20));
    sigmoidTableDgE->setFont(font1);
    sigmoidTableDgE->setMaxLength(8);
    sigmoidTableDgE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_12 = new QLabel(ODESTDPDlg);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(20, 330, 43, 16));
    label_12->setFont(font1);
    gSlopeE = new QLineEdit(ODESTDPDlg);
    gSlopeE->setObjectName(QString::fromUtf8("gSlopeE"));
    gSlopeE->setGeometry(QRect(70, 360, 61, 20));
    gSlopeE->setFont(font1);
    gSlopeE->setMaxLength(8);
    gSlopeE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    gMaxE = new QLineEdit(ODESTDPDlg);
    gMaxE->setObjectName(QString::fromUtf8("gMaxE"));
    gMaxE->setGeometry(QRect(70, 300, 61, 20));
    gMaxE->setFont(font1);
    gMaxE->setMaxLength(8);
    gMaxE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    sigmoidTableDgL = new QLabel(ODESTDPDlg);
    sigmoidTableDgL->setObjectName(QString::fromUtf8("sigmoidTableDgL"));
    sigmoidTableDgL->setEnabled(false);
    sigmoidTableDgL->setGeometry(QRect(210, 330, 46, 16));
    sigmoidTableDgL->setFont(font1);
    label_25 = new QLabel(ODESTDPDlg);
    label_25->setObjectName(QString::fromUtf8("label_25"));
    label_25->setGeometry(QRect(140, 300, 16, 16));
    label_25->setFont(font1);
    label_26 = new QLabel(ODESTDPDlg);
    label_26->setObjectName(QString::fromUtf8("label_26"));
    label_26->setGeometry(QRect(140, 360, 22, 16));
    label_26->setFont(font1);
    sigmoidTableMaxEntryL = new QLabel(ODESTDPDlg);
    sigmoidTableMaxEntryL->setObjectName(QString::fromUtf8("sigmoidTableMaxEntryL"));
    sigmoidTableMaxEntryL->setEnabled(false);
    sigmoidTableMaxEntryL->setGeometry(QRect(210, 360, 46, 16));
    sigmoidTableMaxEntryL->setFont(font1);
    sigmoidTableMaxEntryU = new QLabel(ODESTDPDlg);
    sigmoidTableMaxEntryU->setObjectName(QString::fromUtf8("sigmoidTableMaxEntryU"));
    sigmoidTableMaxEntryU->setEnabled(false);
    sigmoidTableMaxEntryU->setGeometry(QRect(340, 360, 16, 16));
    sigmoidTableMaxEntryU->setFont(font1);
    sigmoidTableCombo = new QComboBox(ODESTDPDlg);
    sigmoidTableCombo->setObjectName(QString::fromUtf8("sigmoidTableCombo"));
    sigmoidTableCombo->setGeometry(QRect(210, 300, 121, 22));
    sigmoidTableCombo->setFont(font1);
    sigmoidTableDgU = new QLabel(ODESTDPDlg);
    sigmoidTableDgU->setObjectName(QString::fromUtf8("sigmoidTableDgU"));
    sigmoidTableDgU->setEnabled(false);
    sigmoidTableDgU->setGeometry(QRect(340, 330, 16, 16));
    sigmoidTableDgU->setFont(font1);
    ChemSynDlgLabel = new QLabel(ODESTDPDlg);
    ChemSynDlgLabel->setObjectName(QString::fromUtf8("ChemSynDlgLabel"));
    ChemSynDlgLabel->setEnabled(true);
    ChemSynDlgLabel->setGeometry(QRect(280, 20, 76, 20));
    QFont font2;
    font2.setPointSize(10);
    font2.setBold(true);
    font2.setWeight(75);
    ChemSynDlgLabel->setFont(font2);
    ChemSynDlgLabel->setAutoFillBackground(false);
    ChemSynDlgLabel->setFrameShape(QFrame::Panel);
    ChemSynDlgLabel->setFrameShadow(QFrame::Raised);
    ChemSynDlgLabel->setLineWidth(1);
    ChemSynDlgLabel->setMidLineWidth(0);

    retranslateUi(ODESTDPDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), ODESTDPDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), ODESTDPDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(ODESTDPDlg);
    } // setupUi

    void retranslateUi(QDialog *ODESTDPDlg)
    {
    ODESTDPDlg->setWindowTitle(QApplication::translate("ODESTDPDlg", "Dialog", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("ODESTDPDlg", "InitialD", 0, QApplication::UnicodeUTF8));
    gammaE->setText(QApplication::translate("ODESTDPDlg", "10", 0, QApplication::UnicodeUTF8));
    InitialPE->setText(QApplication::translate("ODESTDPDlg", "10.0", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("ODESTDPDlg", "betaD", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("ODESTDPDlg", "betaP", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("ODESTDPDlg", "InitialP", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("ODESTDPDlg", "gamma", 0, QApplication::UnicodeUTF8));
    betaDE->setText(QApplication::translate("ODESTDPDlg", "25.0", 0, QApplication::UnicodeUTF8));
    InitialDE->setText(QApplication::translate("ODESTDPDlg", "0.0", 0, QApplication::UnicodeUTF8));
    betaPE->setText(QApplication::translate("ODESTDPDlg", "-20.0", 0, QApplication::UnicodeUTF8));
    etaE->setText(QApplication::translate("ODESTDPDlg", "25.0", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("ODESTDPDlg", "eta", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("ODESTDPDlg", "Differential Equation", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("ODESTDPDlg", "Sigmoid Filter Function P&D", 0, QApplication::UnicodeUTF8));
    highDE->setText(QApplication::translate("ODESTDPDlg", "25.0", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("ODESTDPDlg", "highD", 0, QApplication::UnicodeUTF8));
    lowPE->setText(QApplication::translate("ODESTDPDlg", "10", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("ODESTDPDlg", "lowD", 0, QApplication::UnicodeUTF8));
    highPE->setText(QApplication::translate("ODESTDPDlg", "0.0", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("ODESTDPDlg", "lowP", 0, QApplication::UnicodeUTF8));
    lowDE->setText(QApplication::translate("ODESTDPDlg", "25.0", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("ODESTDPDlg", "highP", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("ODESTDPDlg", "Sigmoid Filter Function gSyn", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("ODESTDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    label_23->setText(QApplication::translate("ODESTDPDlg", "gSlope", 0, QApplication::UnicodeUTF8));
    gMidE->setText(QApplication::translate("ODESTDPDlg", "30.0", 0, QApplication::UnicodeUTF8));
    sigmoidTableMaxEntryE->setText(QApplication::translate("ODESTDPDlg", "100", 0, QApplication::UnicodeUTF8));
    label_24->setText(QApplication::translate("ODESTDPDlg", "gMax", 0, QApplication::UnicodeUTF8));
    sigmoidTableDgE->setText(QApplication::translate("ODESTDPDlg", "0.1", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("ODESTDPDlg", "gMid", 0, QApplication::UnicodeUTF8));
    gSlopeE->setText(QApplication::translate("ODESTDPDlg", "0.04", 0, QApplication::UnicodeUTF8));
    gMaxE->setText(QApplication::translate("ODESTDPDlg", "0.02", 0, QApplication::UnicodeUTF8));
    sigmoidTableDgL->setText(QApplication::translate("ODESTDPDlg", "table dg", 0, QApplication::UnicodeUTF8));
    label_25->setText(QApplication::translate("ODESTDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    label_26->setText(QApplication::translate("ODESTDPDlg", "1/nS", 0, QApplication::UnicodeUTF8));
    sigmoidTableMaxEntryL->setText(QApplication::translate("ODESTDPDlg", "maxEntry", 0, QApplication::UnicodeUTF8));
    sigmoidTableMaxEntryU->setText(QApplication::translate("ODESTDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    sigmoidTableCombo->clear();
    sigmoidTableCombo->insertItems(0, QStringList()
     << QApplication::translate("ODESTDPDlg", "Direct Functions", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ODESTDPDlg", "Lookup Table", 0, QApplication::UnicodeUTF8)
    );
    sigmoidTableDgU->setText(QApplication::translate("ODESTDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    ChemSynDlgLabel->setStyleSheet(QString());
    ChemSynDlgLabel->setText(QApplication::translate("ODESTDPDlg", "Synapse X", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ODESTDPDlg);
    } // retranslateUi

};

namespace Ui {
    class ODESTDPDlg: public Ui_ODESTDPDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ODESTDPDLG_H
