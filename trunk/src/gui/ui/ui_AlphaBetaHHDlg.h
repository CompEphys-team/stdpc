/********************************************************************************
** Form generated from reading ui file 'AlphaBetaHHDlg.ui'
**
** Created: Wed Sep 24 11:44:56 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ALPHABETAHHDLG_H
#define UI_ALPHABETAHHDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_AlphaBetaHHDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *HHDlgLabel;
    QComboBox *VChannelCombo;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_9;
    QLineEdit *gMaxE;
    QLabel *label_5;
    QLineEdit *VrevE;
    QLabel *label_10;
    QLabel *label_14;
    QLabel *label_15;
    QFrame *line;
    QLineEdit *mVaE;
    QLabel *label_6;
    QLineEdit *mkbE;
    QLabel *label_13;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_11;
    QLineEdit *mkaE;
    QLineEdit *mExpoE;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *msaE;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_26;
    QLineEdit *msbE;
    QLabel *label_21;
    QLineEdit *mVbE;
    QLineEdit *hVbE;
    QLabel *label_12;
    QLineEdit *hVaE;
    QLabel *label_22;
    QLineEdit *hExpoE;
    QLabel *label_30;
    QLineEdit *hsbE;
    QLabel *label_16;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_31;
    QLineEdit *hkaE;
    QLineEdit *hkbE;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QLineEdit *hsaE;
    QLabel *label_27;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_35;
    QLabel *label_3;
    QComboBox *IChannelCombo;
    QComboBox *LUCombo;
    QLabel *MethodL;
    QComboBox *maFuncCombo;
    QLabel *label_36;
    QLabel *label_37;
    QComboBox *mbFuncCombo;
    QLabel *label_38;
    QComboBox *haFuncCombo;
    QComboBox *hbFuncCombo;
    QLabel *label_39;

    void setupUi(QDialog *AlphaBetaHHDlg)
    {
    if (AlphaBetaHHDlg->objectName().isEmpty())
        AlphaBetaHHDlg->setObjectName(QString::fromUtf8("AlphaBetaHHDlg"));
    AlphaBetaHHDlg->resize(434, 506);
    QFont font;
    font.setPointSize(8);
    font.setBold(false);
    font.setWeight(50);
    AlphaBetaHHDlg->setFont(font);
    buttonBox = new QDialogButtonBox(AlphaBetaHHDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(250, 460, 160, 32));
    QFont font1;
    font1.setBold(true);
    font1.setWeight(75);
    buttonBox->setFont(font1);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    HHDlgLabel = new QLabel(AlphaBetaHHDlg);
    HHDlgLabel->setObjectName(QString::fromUtf8("HHDlgLabel"));
    HHDlgLabel->setEnabled(true);
    HHDlgLabel->setGeometry(QRect(350, 20, 51, 20));
    QFont font2;
    font2.setPointSize(10);
    font2.setBold(true);
    font2.setWeight(75);
    HHDlgLabel->setFont(font2);
    HHDlgLabel->setAutoFillBackground(false);
    HHDlgLabel->setFrameShape(QFrame::Panel);
    HHDlgLabel->setFrameShadow(QFrame::Raised);
    HHDlgLabel->setLineWidth(1);
    HHDlgLabel->setMidLineWidth(0);
    VChannelCombo = new QComboBox(AlphaBetaHHDlg);
    VChannelCombo->setObjectName(QString::fromUtf8("VChannelCombo"));
    VChannelCombo->setGeometry(QRect(90, 20, 40, 22));
    VChannelCombo->setFont(font);
    label_2 = new QLabel(AlphaBetaHHDlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 20, 61, 16));
    label_2->setFont(font);
    label_4 = new QLabel(AlphaBetaHHDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 50, 31, 16));
    label_4->setFont(font);
    label_9 = new QLabel(AlphaBetaHHDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(140, 50, 16, 16));
    label_9->setFont(font);
    gMaxE = new QLineEdit(AlphaBetaHHDlg);
    gMaxE->setObjectName(QString::fromUtf8("gMaxE"));
    gMaxE->setGeometry(QRect(70, 50, 61, 20));
    gMaxE->setFont(font);
    gMaxE->setMaxLength(8);
    gMaxE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_5 = new QLabel(AlphaBetaHHDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 80, 31, 16));
    label_5->setFont(font);
    VrevE = new QLineEdit(AlphaBetaHHDlg);
    VrevE->setObjectName(QString::fromUtf8("VrevE"));
    VrevE->setGeometry(QRect(70, 80, 61, 20));
    VrevE->setFont(font);
    VrevE->setMaxLength(8);
    VrevE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_10 = new QLabel(AlphaBetaHHDlg);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(140, 80, 16, 16));
    label_10->setFont(font);
    label_14 = new QLabel(AlphaBetaHHDlg);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(20, 120, 58, 16));
    label_14->setFont(font1);
    label_15 = new QLabel(AlphaBetaHHDlg);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setGeometry(QRect(240, 120, 69, 16));
    label_15->setFont(font1);
    line = new QFrame(AlphaBetaHHDlg);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(200, 150, 20, 281));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    mVaE = new QLineEdit(AlphaBetaHHDlg);
    mVaE->setObjectName(QString::fromUtf8("mVaE"));
    mVaE->setGeometry(QRect(80, 250, 61, 20));
    mVaE->setMaxLength(8);
    mVaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_6 = new QLabel(AlphaBetaHHDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(20, 150, 46, 16));
    mkbE = new QLineEdit(AlphaBetaHHDlg);
    mkbE->setObjectName(QString::fromUtf8("mkbE"));
    mkbE->setGeometry(QRect(80, 350, 61, 20));
    mkbE->setMaxLength(8);
    mkbE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_13 = new QLabel(AlphaBetaHHDlg);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(150, 350, 23, 16));
    label_7 = new QLabel(AlphaBetaHHDlg);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(20, 220, 56, 16));
    label_8 = new QLabel(AlphaBetaHHDlg);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(20, 250, 38, 16));
    label_11 = new QLabel(AlphaBetaHHDlg);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(20, 350, 56, 16));
    mkaE = new QLineEdit(AlphaBetaHHDlg);
    mkaE->setObjectName(QString::fromUtf8("mkaE"));
    mkaE->setGeometry(QRect(80, 220, 61, 20));
    mkaE->setMaxLength(8);
    mkaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    mExpoE = new QLineEdit(AlphaBetaHHDlg);
    mExpoE->setObjectName(QString::fromUtf8("mExpoE"));
    mExpoE->setGeometry(QRect(80, 150, 61, 20));
    mExpoE->setMaxLength(8);
    mExpoE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_17 = new QLabel(AlphaBetaHHDlg);
    label_17->setObjectName(QString::fromUtf8("label_17"));
    label_17->setGeometry(QRect(150, 250, 16, 16));
    label_18 = new QLabel(AlphaBetaHHDlg);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setGeometry(QRect(150, 280, 16, 16));
    msaE = new QLineEdit(AlphaBetaHHDlg);
    msaE->setObjectName(QString::fromUtf8("msaE"));
    msaE->setGeometry(QRect(80, 280, 61, 20));
    msaE->setMaxLength(8);
    msaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_19 = new QLabel(AlphaBetaHHDlg);
    label_19->setObjectName(QString::fromUtf8("label_19"));
    label_19->setGeometry(QRect(20, 280, 32, 16));
    label_20 = new QLabel(AlphaBetaHHDlg);
    label_20->setObjectName(QString::fromUtf8("label_20"));
    label_20->setGeometry(QRect(20, 380, 47, 16));
    label_26 = new QLabel(AlphaBetaHHDlg);
    label_26->setObjectName(QString::fromUtf8("label_26"));
    label_26->setGeometry(QRect(150, 410, 16, 16));
    msbE = new QLineEdit(AlphaBetaHHDlg);
    msbE->setObjectName(QString::fromUtf8("msbE"));
    msbE->setGeometry(QRect(80, 410, 61, 20));
    msbE->setMaxLength(8);
    msbE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_21 = new QLabel(AlphaBetaHHDlg);
    label_21->setObjectName(QString::fromUtf8("label_21"));
    label_21->setGeometry(QRect(20, 410, 30, 16));
    mVbE = new QLineEdit(AlphaBetaHHDlg);
    mVbE->setObjectName(QString::fromUtf8("mVbE"));
    mVbE->setGeometry(QRect(80, 380, 61, 20));
    mVbE->setMaxLength(8);
    mVbE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    hVbE = new QLineEdit(AlphaBetaHHDlg);
    hVbE->setObjectName(QString::fromUtf8("hVbE"));
    hVbE->setGeometry(QRect(300, 380, 61, 20));
    hVbE->setMaxLength(8);
    hVbE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_12 = new QLabel(AlphaBetaHHDlg);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(240, 150, 46, 16));
    hVaE = new QLineEdit(AlphaBetaHHDlg);
    hVaE->setObjectName(QString::fromUtf8("hVaE"));
    hVaE->setGeometry(QRect(300, 250, 61, 20));
    hVaE->setMaxLength(8);
    hVaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_22 = new QLabel(AlphaBetaHHDlg);
    label_22->setObjectName(QString::fromUtf8("label_22"));
    label_22->setGeometry(QRect(370, 250, 16, 16));
    hExpoE = new QLineEdit(AlphaBetaHHDlg);
    hExpoE->setObjectName(QString::fromUtf8("hExpoE"));
    hExpoE->setGeometry(QRect(300, 150, 61, 20));
    hExpoE->setMaxLength(8);
    hExpoE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_30 = new QLabel(AlphaBetaHHDlg);
    label_30->setObjectName(QString::fromUtf8("label_30"));
    label_30->setGeometry(QRect(370, 410, 16, 16));
    hsbE = new QLineEdit(AlphaBetaHHDlg);
    hsbE->setObjectName(QString::fromUtf8("hsbE"));
    hsbE->setGeometry(QRect(300, 410, 61, 20));
    hsbE->setMaxLength(8);
    hsbE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_16 = new QLabel(AlphaBetaHHDlg);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setGeometry(QRect(240, 220, 47, 16));
    label_23 = new QLabel(AlphaBetaHHDlg);
    label_23->setObjectName(QString::fromUtf8("label_23"));
    label_23->setGeometry(QRect(240, 250, 38, 16));
    label_24 = new QLabel(AlphaBetaHHDlg);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setGeometry(QRect(240, 410, 31, 16));
    label_25 = new QLabel(AlphaBetaHHDlg);
    label_25->setObjectName(QString::fromUtf8("label_25"));
    label_25->setGeometry(QRect(240, 350, 32, 16));
    label_31 = new QLabel(AlphaBetaHHDlg);
    label_31->setObjectName(QString::fromUtf8("label_31"));
    label_31->setGeometry(QRect(240, 380, 45, 16));
    hkaE = new QLineEdit(AlphaBetaHHDlg);
    hkaE->setObjectName(QString::fromUtf8("hkaE"));
    hkaE->setGeometry(QRect(300, 220, 61, 20));
    hkaE->setMaxLength(8);
    hkaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    hkbE = new QLineEdit(AlphaBetaHHDlg);
    hkbE->setObjectName(QString::fromUtf8("hkbE"));
    hkbE->setGeometry(QRect(300, 350, 61, 20));
    hkbE->setMaxLength(8);
    hkbE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_32 = new QLabel(AlphaBetaHHDlg);
    label_32->setObjectName(QString::fromUtf8("label_32"));
    label_32->setGeometry(QRect(370, 280, 16, 16));
    label_33 = new QLabel(AlphaBetaHHDlg);
    label_33->setObjectName(QString::fromUtf8("label_33"));
    label_33->setGeometry(QRect(240, 280, 32, 16));
    label_34 = new QLabel(AlphaBetaHHDlg);
    label_34->setObjectName(QString::fromUtf8("label_34"));
    label_34->setGeometry(QRect(370, 350, 23, 16));
    hsaE = new QLineEdit(AlphaBetaHHDlg);
    hsaE->setObjectName(QString::fromUtf8("hsaE"));
    hsaE->setGeometry(QRect(300, 280, 61, 20));
    hsaE->setMaxLength(8);
    hsaE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_27 = new QLabel(AlphaBetaHHDlg);
    label_27->setObjectName(QString::fromUtf8("label_27"));
    label_27->setGeometry(QRect(150, 220, 23, 16));
    label_28 = new QLabel(AlphaBetaHHDlg);
    label_28->setObjectName(QString::fromUtf8("label_28"));
    label_28->setGeometry(QRect(150, 380, 16, 16));
    label_29 = new QLabel(AlphaBetaHHDlg);
    label_29->setObjectName(QString::fromUtf8("label_29"));
    label_29->setGeometry(QRect(370, 220, 23, 16));
    label_35 = new QLabel(AlphaBetaHHDlg);
    label_35->setObjectName(QString::fromUtf8("label_35"));
    label_35->setGeometry(QRect(370, 380, 16, 16));
    label_3 = new QLabel(AlphaBetaHHDlg);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(170, 20, 61, 16));
    label_3->setFont(font);
    IChannelCombo = new QComboBox(AlphaBetaHHDlg);
    IChannelCombo->setObjectName(QString::fromUtf8("IChannelCombo"));
    IChannelCombo->setGeometry(QRect(240, 20, 40, 22));
    IChannelCombo->setFont(font);
    LUCombo = new QComboBox(AlphaBetaHHDlg);
    LUCombo->setObjectName(QString::fromUtf8("LUCombo"));
    LUCombo->setGeometry(QRect(240, 80, 121, 22));
    LUCombo->setFont(font);
    MethodL = new QLabel(AlphaBetaHHDlg);
    MethodL->setObjectName(QString::fromUtf8("MethodL"));
    MethodL->setGeometry(QRect(240, 60, 69, 16));
    MethodL->setFont(font1);
    maFuncCombo = new QComboBox(AlphaBetaHHDlg);
    maFuncCombo->setObjectName(QString::fromUtf8("maFuncCombo"));
    maFuncCombo->setGeometry(QRect(80, 190, 104, 22));
    maFuncCombo->setFont(font);
    label_36 = new QLabel(AlphaBetaHHDlg);
    label_36->setObjectName(QString::fromUtf8("label_36"));
    label_36->setGeometry(QRect(20, 190, 56, 16));
    label_37 = new QLabel(AlphaBetaHHDlg);
    label_37->setObjectName(QString::fromUtf8("label_37"));
    label_37->setGeometry(QRect(20, 320, 56, 16));
    mbFuncCombo = new QComboBox(AlphaBetaHHDlg);
    mbFuncCombo->setObjectName(QString::fromUtf8("mbFuncCombo"));
    mbFuncCombo->setGeometry(QRect(80, 320, 104, 22));
    mbFuncCombo->setFont(font);
    label_38 = new QLabel(AlphaBetaHHDlg);
    label_38->setObjectName(QString::fromUtf8("label_38"));
    label_38->setGeometry(QRect(240, 190, 56, 16));
    haFuncCombo = new QComboBox(AlphaBetaHHDlg);
    haFuncCombo->setObjectName(QString::fromUtf8("haFuncCombo"));
    haFuncCombo->setGeometry(QRect(300, 190, 104, 22));
    haFuncCombo->setFont(font);
    hbFuncCombo = new QComboBox(AlphaBetaHHDlg);
    hbFuncCombo->setObjectName(QString::fromUtf8("hbFuncCombo"));
    hbFuncCombo->setGeometry(QRect(300, 320, 104, 22));
    hbFuncCombo->setFont(font);
    label_39 = new QLabel(AlphaBetaHHDlg);
    label_39->setObjectName(QString::fromUtf8("label_39"));
    label_39->setGeometry(QRect(240, 320, 56, 16));

    retranslateUi(AlphaBetaHHDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), AlphaBetaHHDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), AlphaBetaHHDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(AlphaBetaHHDlg);
    } // setupUi

    void retranslateUi(QDialog *AlphaBetaHHDlg)
    {
    AlphaBetaHHDlg->setWindowTitle(QApplication::translate("AlphaBetaHHDlg", "Dialog", 0, QApplication::UnicodeUTF8));
    HHDlgLabel->setStyleSheet(QString());
    HHDlgLabel->setText(QApplication::translate("AlphaBetaHHDlg", "HH X", 0, QApplication::UnicodeUTF8));
    VChannelCombo->clear();
    VChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("AlphaBetaHHDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "SG", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("AlphaBetaHHDlg", "V inChannel", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("AlphaBetaHHDlg", "gMax", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("AlphaBetaHHDlg", "nS", 0, QApplication::UnicodeUTF8));
    gMaxE->setText(QApplication::translate("AlphaBetaHHDlg", "10.0", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("AlphaBetaHHDlg", "Vrev", 0, QApplication::UnicodeUTF8));
    VrevE->setText(QApplication::translate("AlphaBetaHHDlg", "10.0", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("AlphaBetaHHDlg", "Activation", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("AlphaBetaHHDlg", "Inactivation", 0, QApplication::UnicodeUTF8));
    mVaE->setText(QApplication::translate("AlphaBetaHHDlg", "-52.0", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("AlphaBetaHHDlg", "Exponent", 0, QApplication::UnicodeUTF8));
    mkbE->setText(QApplication::translate("AlphaBetaHHDlg", "-0.28", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("AlphaBetaHHDlg", "1/ms", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("AlphaBetaHHDlg", "mka", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("AlphaBetaHHDlg", "mVa", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("AlphaBetaHHDlg", "mkb", 0, QApplication::UnicodeUTF8));
    mkaE->setText(QApplication::translate("AlphaBetaHHDlg", "0.32", 0, QApplication::UnicodeUTF8));
    mExpoE->setText(QApplication::translate("AlphaBetaHHDlg", "3", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    msaE->setText(QApplication::translate("AlphaBetaHHDlg", "4.0", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("AlphaBetaHHDlg", "msa", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("AlphaBetaHHDlg", "mVb", 0, QApplication::UnicodeUTF8));
    label_26->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    msbE->setText(QApplication::translate("AlphaBetaHHDlg", "-5.0", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("AlphaBetaHHDlg", "msb", 0, QApplication::UnicodeUTF8));
    mVbE->setText(QApplication::translate("AlphaBetaHHDlg", "-25.0", 0, QApplication::UnicodeUTF8));
    hVbE->setText(QApplication::translate("AlphaBetaHHDlg", "-25.0", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("AlphaBetaHHDlg", "Exponent", 0, QApplication::UnicodeUTF8));
    hVaE->setText(QApplication::translate("AlphaBetaHHDlg", "-48.0", 0, QApplication::UnicodeUTF8));
    label_22->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    hExpoE->setText(QApplication::translate("AlphaBetaHHDlg", "1", 0, QApplication::UnicodeUTF8));
    label_30->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    hsbE->setText(QApplication::translate("AlphaBetaHHDlg", "5.0", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("AlphaBetaHHDlg", "hka", 0, QApplication::UnicodeUTF8));
    label_23->setText(QApplication::translate("AlphaBetaHHDlg", "hVa", 0, QApplication::UnicodeUTF8));
    label_24->setText(QApplication::translate("AlphaBetaHHDlg", "hsb", 0, QApplication::UnicodeUTF8));
    label_25->setText(QApplication::translate("AlphaBetaHHDlg", "hkb", 0, QApplication::UnicodeUTF8));
    label_31->setText(QApplication::translate("AlphaBetaHHDlg", "hVb", 0, QApplication::UnicodeUTF8));
    hkaE->setText(QApplication::translate("AlphaBetaHHDlg", "0.128", 0, QApplication::UnicodeUTF8));
    hkbE->setText(QApplication::translate("AlphaBetaHHDlg", "4.0", 0, QApplication::UnicodeUTF8));
    label_32->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_33->setText(QApplication::translate("AlphaBetaHHDlg", "hsa", 0, QApplication::UnicodeUTF8));
    label_34->setText(QApplication::translate("AlphaBetaHHDlg", "m1/s", 0, QApplication::UnicodeUTF8));
    hsaE->setText(QApplication::translate("AlphaBetaHHDlg", "18.0", 0, QApplication::UnicodeUTF8));
    label_27->setText(QApplication::translate("AlphaBetaHHDlg", "1/ms", 0, QApplication::UnicodeUTF8));
    label_28->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_29->setText(QApplication::translate("AlphaBetaHHDlg", "1/ms", 0, QApplication::UnicodeUTF8));
    label_35->setText(QApplication::translate("AlphaBetaHHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("AlphaBetaHHDlg", "I outChannel", 0, QApplication::UnicodeUTF8));
    IChannelCombo->clear();
    IChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("AlphaBetaHHDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "1", 0, QApplication::UnicodeUTF8)
    );
    LUCombo->clear();
    LUCombo->insertItems(0, QStringList()
     << QApplication::translate("AlphaBetaHHDlg", "Direct Functions", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "LookupTables", 0, QApplication::UnicodeUTF8)
    );
    MethodL->setText(QApplication::translate("AlphaBetaHHDlg", "Method", 0, QApplication::UnicodeUTF8));
    maFuncCombo->clear();
    maFuncCombo->insertItems(0, QStringList()
     << QApplication::translate("AlphaBetaHHDlg", "k*V/(exp(V)-1)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*exp(V)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*1/(1+exp(V))", 0, QApplication::UnicodeUTF8)
    );
    label_36->setText(QApplication::translate("AlphaBetaHHDlg", "ma type", 0, QApplication::UnicodeUTF8));
    label_37->setText(QApplication::translate("AlphaBetaHHDlg", "mb type", 0, QApplication::UnicodeUTF8));
    mbFuncCombo->clear();
    mbFuncCombo->insertItems(0, QStringList()
     << QApplication::translate("AlphaBetaHHDlg", "k*V/(exp(V)-1)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*exp(V)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*1/(1+exp(V))", 0, QApplication::UnicodeUTF8)
    );
    label_38->setText(QApplication::translate("AlphaBetaHHDlg", "ha type", 0, QApplication::UnicodeUTF8));
    haFuncCombo->clear();
    haFuncCombo->insertItems(0, QStringList()
     << QApplication::translate("AlphaBetaHHDlg", "k*V/(exp(V)-1)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*exp(V)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*1/(1+exp(V))", 0, QApplication::UnicodeUTF8)
    );
    hbFuncCombo->clear();
    hbFuncCombo->insertItems(0, QStringList()
     << QApplication::translate("AlphaBetaHHDlg", "k*V/(exp(V)-1)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*exp(V)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AlphaBetaHHDlg", "k*1/(1+exp(V))", 0, QApplication::UnicodeUTF8)
    );
    label_39->setText(QApplication::translate("AlphaBetaHHDlg", "hb type", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(AlphaBetaHHDlg);
    } // retranslateUi

};

namespace Ui {
    class AlphaBetaHHDlg: public Ui_AlphaBetaHHDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALPHABETAHHDLG_H
