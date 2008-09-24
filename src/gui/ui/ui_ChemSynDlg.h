/********************************************************************************
** Form generated from reading ui file 'ChemSynDlg.ui'
**
** Created: Wed Sep 24 11:44:55 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHEMSYNDLG_H
#define UI_CHEMSYNDLG_H

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
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ChemSynDlg
{
public:
    QDialogButtonBox *ResCloseBox;
    QLabel *ChemSynDlgLabel;
    QLabel *label_2;
    QComboBox *PreSynChannelCombo;
    QLabel *label_3;
    QComboBox *PostSynChannelCombo;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *gSynE;
    QLineEdit *VSynE;
    QLineEdit *tauSynE;
    QLineEdit *VThreshE;
    QLineEdit *VSlopeE;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *STDVSlopeE;
    QLabel *STDAmplL;
    QLineEdit *STDtauAmplE;
    QLabel *STDVThreshL;
    QLabel *STDtauAmplL;
    QLabel *STDVSlopeL;
    QLabel *STDVThreshU;
    QLineEdit *STDVThreshE;
    QLineEdit *STDAmplE;
    QLabel *STDVSlopeU;
    QLabel *STDtau0U;
    QLineEdit *STDtau0E;
    QLabel *STDtau0L;
    QLabel *STDtauVThreshL;
    QLabel *STDtauVThreshU;
    QLineEdit *STDtauVThreshE;
    QLabel *STDtauVSlopeU;
    QLabel *STDtauVSlopeL;
    QLineEdit *STDtauVSlopeE;
    QLabel *label_18;
    QComboBox *fixVpostCombo;
    QLabel *label_29;
    QLabel *label_30;
    QLineEdit *VpostE;
    QLabel *label_31;
    QComboBox *PlasticityCombo;
    QPushButton *PlastParaBut;
    QFrame *line;
    QComboBox *STDCombo;
    QLabel *OutSynChannelL;
    QComboBox *OutSynChannelCombo;
    QComboBox *LUCombo;
    QLabel *MethodL;

    void setupUi(QDialog *ChemSynDlg)
    {
    if (ChemSynDlg->objectName().isEmpty())
        ChemSynDlg->setObjectName(QString::fromUtf8("ChemSynDlg"));
    ChemSynDlg->resize(374, 498);
    ResCloseBox = new QDialogButtonBox(ChemSynDlg);
    ResCloseBox->setObjectName(QString::fromUtf8("ResCloseBox"));
    ResCloseBox->setGeometry(QRect(200, 450, 160, 32));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    ResCloseBox->setFont(font);
    ResCloseBox->setOrientation(Qt::Horizontal);
    ResCloseBox->setStandardButtons(QDialogButtonBox::Close);
    ChemSynDlgLabel = new QLabel(ChemSynDlg);
    ChemSynDlgLabel->setObjectName(QString::fromUtf8("ChemSynDlgLabel"));
    ChemSynDlgLabel->setEnabled(true);
    ChemSynDlgLabel->setGeometry(QRect(280, 20, 76, 20));
    QFont font1;
    font1.setPointSize(10);
    font1.setBold(true);
    font1.setWeight(75);
    ChemSynDlgLabel->setFont(font1);
    ChemSynDlgLabel->setAutoFillBackground(false);
    ChemSynDlgLabel->setFrameShape(QFrame::Panel);
    ChemSynDlgLabel->setFrameShadow(QFrame::Raised);
    ChemSynDlgLabel->setLineWidth(1);
    ChemSynDlgLabel->setMidLineWidth(0);
    label_2 = new QLabel(ChemSynDlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 20, 98, 16));
    PreSynChannelCombo = new QComboBox(ChemSynDlg);
    PreSynChannelCombo->setObjectName(QString::fromUtf8("PreSynChannelCombo"));
    PreSynChannelCombo->setGeometry(QRect(130, 20, 40, 22));
    label_3 = new QLabel(ChemSynDlg);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(20, 50, 98, 16));
    PostSynChannelCombo = new QComboBox(ChemSynDlg);
    PostSynChannelCombo->setObjectName(QString::fromUtf8("PostSynChannelCombo"));
    PostSynChannelCombo->setGeometry(QRect(130, 50, 40, 22));
    label_4 = new QLabel(ChemSynDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 160, 24, 16));
    label_5 = new QLabel(ChemSynDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 190, 24, 16));
    label_6 = new QLabel(ChemSynDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(20, 220, 34, 16));
    gSynE = new QLineEdit(ChemSynDlg);
    gSynE->setObjectName(QString::fromUtf8("gSynE"));
    gSynE->setGeometry(QRect(70, 160, 61, 20));
    gSynE->setMaxLength(8);
    gSynE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    VSynE = new QLineEdit(ChemSynDlg);
    VSynE->setObjectName(QString::fromUtf8("VSynE"));
    VSynE->setGeometry(QRect(70, 190, 61, 20));
    VSynE->setMaxLength(8);
    VSynE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    tauSynE = new QLineEdit(ChemSynDlg);
    tauSynE->setObjectName(QString::fromUtf8("tauSynE"));
    tauSynE->setGeometry(QRect(70, 220, 61, 20));
    tauSynE->setMaxLength(8);
    tauSynE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    VThreshE = new QLineEdit(ChemSynDlg);
    VThreshE->setObjectName(QString::fromUtf8("VThreshE"));
    VThreshE->setGeometry(QRect(70, 250, 61, 20));
    VThreshE->setMaxLength(8);
    VThreshE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    VSlopeE = new QLineEdit(ChemSynDlg);
    VSlopeE->setObjectName(QString::fromUtf8("VSlopeE"));
    VSlopeE->setGeometry(QRect(70, 280, 61, 20));
    VSlopeE->setMaxLength(8);
    VSlopeE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_7 = new QLabel(ChemSynDlg);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(20, 250, 39, 16));
    label_8 = new QLabel(ChemSynDlg);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(20, 280, 32, 16));
    label_9 = new QLabel(ChemSynDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(140, 160, 16, 16));
    label_10 = new QLabel(ChemSynDlg);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(140, 190, 16, 16));
    label_11 = new QLabel(ChemSynDlg);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(140, 220, 16, 16));
    label_12 = new QLabel(ChemSynDlg);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(140, 250, 16, 16));
    label_13 = new QLabel(ChemSynDlg);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(140, 280, 16, 16));
    label_14 = new QLabel(ChemSynDlg);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(20, 130, 52, 16));
    label_14->setFont(font);
    label_15 = new QLabel(ChemSynDlg);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setGeometry(QRect(210, 130, 131, 16));
    label_15->setFont(font);
    STDVSlopeE = new QLineEdit(ChemSynDlg);
    STDVSlopeE->setObjectName(QString::fromUtf8("STDVSlopeE"));
    STDVSlopeE->setEnabled(false);
    STDVSlopeE->setGeometry(QRect(270, 260, 61, 20));
    STDVSlopeE->setMaxLength(8);
    STDVSlopeE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    STDAmplL = new QLabel(ChemSynDlg);
    STDAmplL->setObjectName(QString::fromUtf8("STDAmplL"));
    STDAmplL->setEnabled(false);
    STDAmplL->setGeometry(QRect(210, 200, 47, 16));
    STDtauAmplE = new QLineEdit(ChemSynDlg);
    STDtauAmplE->setObjectName(QString::fromUtf8("STDtauAmplE"));
    STDtauAmplE->setEnabled(false);
    STDtauAmplE->setGeometry(QRect(270, 320, 61, 20));
    STDtauAmplE->setMaxLength(8);
    STDtauAmplE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    STDVThreshL = new QLabel(ChemSynDlg);
    STDVThreshL->setObjectName(QString::fromUtf8("STDVThreshL"));
    STDVThreshL->setEnabled(false);
    STDVThreshL->setGeometry(QRect(210, 230, 39, 16));
    STDtauAmplL = new QLabel(ChemSynDlg);
    STDtauAmplL->setObjectName(QString::fromUtf8("STDtauAmplL"));
    STDtauAmplL->setEnabled(false);
    STDtauAmplL->setGeometry(QRect(210, 320, 41, 16));
    STDVSlopeL = new QLabel(ChemSynDlg);
    STDVSlopeL->setObjectName(QString::fromUtf8("STDVSlopeL"));
    STDVSlopeL->setEnabled(false);
    STDVSlopeL->setGeometry(QRect(210, 260, 32, 16));
    STDVThreshU = new QLabel(ChemSynDlg);
    STDVThreshU->setObjectName(QString::fromUtf8("STDVThreshU"));
    STDVThreshU->setEnabled(false);
    STDVThreshU->setGeometry(QRect(340, 230, 16, 16));
    STDVThreshE = new QLineEdit(ChemSynDlg);
    STDVThreshE->setObjectName(QString::fromUtf8("STDVThreshE"));
    STDVThreshE->setEnabled(false);
    STDVThreshE->setGeometry(QRect(270, 230, 61, 20));
    STDVThreshE->setMaxLength(8);
    STDVThreshE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    STDAmplE = new QLineEdit(ChemSynDlg);
    STDAmplE->setObjectName(QString::fromUtf8("STDAmplE"));
    STDAmplE->setEnabled(false);
    STDAmplE->setGeometry(QRect(270, 200, 61, 20));
    STDAmplE->setMaxLength(8);
    STDAmplE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    STDVSlopeU = new QLabel(ChemSynDlg);
    STDVSlopeU->setObjectName(QString::fromUtf8("STDVSlopeU"));
    STDVSlopeU->setEnabled(false);
    STDVSlopeU->setGeometry(QRect(340, 260, 16, 16));
    STDtau0U = new QLabel(ChemSynDlg);
    STDtau0U->setObjectName(QString::fromUtf8("STDtau0U"));
    STDtau0U->setEnabled(false);
    STDtau0U->setGeometry(QRect(340, 290, 16, 16));
    STDtau0E = new QLineEdit(ChemSynDlg);
    STDtau0E->setObjectName(QString::fromUtf8("STDtau0E"));
    STDtau0E->setEnabled(false);
    STDtau0E->setGeometry(QRect(270, 290, 61, 20));
    STDtau0E->setMaxLength(8);
    STDtau0E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    STDtau0L = new QLabel(ChemSynDlg);
    STDtau0L->setObjectName(QString::fromUtf8("STDtau0L"));
    STDtau0L->setEnabled(false);
    STDtau0L->setGeometry(QRect(210, 290, 22, 16));
    STDtauVThreshL = new QLabel(ChemSynDlg);
    STDtauVThreshL->setObjectName(QString::fromUtf8("STDtauVThreshL"));
    STDtauVThreshL->setEnabled(false);
    STDtauVThreshL->setGeometry(QRect(210, 350, 55, 16));
    STDtauVThreshU = new QLabel(ChemSynDlg);
    STDtauVThreshU->setObjectName(QString::fromUtf8("STDtauVThreshU"));
    STDtauVThreshU->setEnabled(false);
    STDtauVThreshU->setGeometry(QRect(340, 350, 16, 16));
    STDtauVThreshE = new QLineEdit(ChemSynDlg);
    STDtauVThreshE->setObjectName(QString::fromUtf8("STDtauVThreshE"));
    STDtauVThreshE->setEnabled(false);
    STDtauVThreshE->setGeometry(QRect(270, 350, 61, 20));
    STDtauVThreshE->setMaxLength(8);
    STDtauVThreshE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    STDtauVSlopeU = new QLabel(ChemSynDlg);
    STDtauVSlopeU->setObjectName(QString::fromUtf8("STDtauVSlopeU"));
    STDtauVSlopeU->setEnabled(false);
    STDtauVSlopeU->setGeometry(QRect(340, 380, 16, 16));
    STDtauVSlopeL = new QLabel(ChemSynDlg);
    STDtauVSlopeL->setObjectName(QString::fromUtf8("STDtauVSlopeL"));
    STDtauVSlopeL->setEnabled(false);
    STDtauVSlopeL->setGeometry(QRect(210, 380, 48, 16));
    STDtauVSlopeE = new QLineEdit(ChemSynDlg);
    STDtauVSlopeE->setObjectName(QString::fromUtf8("STDtauVSlopeE"));
    STDtauVSlopeE->setEnabled(false);
    STDtauVSlopeE->setGeometry(QRect(270, 380, 61, 20));
    STDtauVSlopeE->setMaxLength(8);
    STDtauVSlopeE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_18 = new QLabel(ChemSynDlg);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setGeometry(QRect(20, 320, 52, 16));
    label_18->setFont(font);
    fixVpostCombo = new QComboBox(ChemSynDlg);
    fixVpostCombo->setObjectName(QString::fromUtf8("fixVpostCombo"));
    fixVpostCombo->setGeometry(QRect(20, 340, 111, 22));
    label_29 = new QLabel(ChemSynDlg);
    label_29->setObjectName(QString::fromUtf8("label_29"));
    label_29->setGeometry(QRect(20, 370, 27, 16));
    label_30 = new QLabel(ChemSynDlg);
    label_30->setObjectName(QString::fromUtf8("label_30"));
    label_30->setGeometry(QRect(140, 370, 16, 16));
    VpostE = new QLineEdit(ChemSynDlg);
    VpostE->setObjectName(QString::fromUtf8("VpostE"));
    VpostE->setGeometry(QRect(70, 370, 61, 20));
    VpostE->setMaxLength(8);
    VpostE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_31 = new QLabel(ChemSynDlg);
    label_31->setObjectName(QString::fromUtf8("label_31"));
    label_31->setGeometry(QRect(20, 410, 52, 16));
    label_31->setFont(font);
    PlasticityCombo = new QComboBox(ChemSynDlg);
    PlasticityCombo->setObjectName(QString::fromUtf8("PlasticityCombo"));
    PlasticityCombo->setGeometry(QRect(20, 430, 121, 22));
    PlastParaBut = new QPushButton(ChemSynDlg);
    PlastParaBut->setObjectName(QString::fromUtf8("PlastParaBut"));
    PlastParaBut->setEnabled(false);
    PlastParaBut->setGeometry(QRect(20, 460, 121, 25));
    line = new QFrame(ChemSynDlg);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(170, 160, 20, 241));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    STDCombo = new QComboBox(ChemSynDlg);
    STDCombo->setObjectName(QString::fromUtf8("STDCombo"));
    STDCombo->setGeometry(QRect(210, 160, 121, 22));
    OutSynChannelL = new QLabel(ChemSynDlg);
    OutSynChannelL->setObjectName(QString::fromUtf8("OutSynChannelL"));
    OutSynChannelL->setGeometry(QRect(20, 80, 98, 16));
    OutSynChannelCombo = new QComboBox(ChemSynDlg);
    OutSynChannelCombo->setObjectName(QString::fromUtf8("OutSynChannelCombo"));
    OutSynChannelCombo->setGeometry(QRect(130, 80, 40, 22));
    LUCombo = new QComboBox(ChemSynDlg);
    LUCombo->setObjectName(QString::fromUtf8("LUCombo"));
    LUCombo->setGeometry(QRect(210, 80, 121, 22));
    QFont font2;
    font2.setPointSize(8);
    font2.setBold(false);
    font2.setWeight(50);
    LUCombo->setFont(font2);
    MethodL = new QLabel(ChemSynDlg);
    MethodL->setObjectName(QString::fromUtf8("MethodL"));
    MethodL->setGeometry(QRect(210, 60, 69, 16));
    MethodL->setFont(font);

    retranslateUi(ChemSynDlg);

    PostSynChannelCombo->setCurrentIndex(1);
    OutSynChannelCombo->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(ChemSynDlg);
    } // setupUi

    void retranslateUi(QDialog *ChemSynDlg)
    {
    ChemSynDlg->setWindowTitle(QApplication::translate("ChemSynDlg", "Chemical Synapse", 0, QApplication::UnicodeUTF8));
    ChemSynDlgLabel->setStyleSheet(QString());
    ChemSynDlgLabel->setText(QApplication::translate("ChemSynDlg", "Synapse X", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("ChemSynDlg", "Presynaptic Channel", 0, QApplication::UnicodeUTF8));
    PreSynChannelCombo->clear();
    PreSynChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("ChemSynDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "SG", 0, QApplication::UnicodeUTF8)
    );
    label_3->setText(QApplication::translate("ChemSynDlg", "Postynaptic Channel", 0, QApplication::UnicodeUTF8));
    PostSynChannelCombo->clear();
    PostSynChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("ChemSynDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "SG", 0, QApplication::UnicodeUTF8)
    );
    label_4->setText(QApplication::translate("ChemSynDlg", "gSyn", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("ChemSynDlg", "VSyn", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("ChemSynDlg", "tauSyn", 0, QApplication::UnicodeUTF8));
    gSynE->setText(QApplication::translate("ChemSynDlg", "10.0", 0, QApplication::UnicodeUTF8));
    VSynE->setText(QApplication::translate("ChemSynDlg", "0.0", 0, QApplication::UnicodeUTF8));
    tauSynE->setText(QApplication::translate("ChemSynDlg", "10", 0, QApplication::UnicodeUTF8));
    VThreshE->setText(QApplication::translate("ChemSynDlg", "-20.0", 0, QApplication::UnicodeUTF8));
    VSlopeE->setText(QApplication::translate("ChemSynDlg", "25.0", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("ChemSynDlg", "VThresh", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("ChemSynDlg", "VSlope", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("ChemSynDlg", "nS", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("ChemSynDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("ChemSynDlg", "Standard", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("ChemSynDlg", "Short Term Depression", 0, QApplication::UnicodeUTF8));
    STDVSlopeE->setText(QApplication::translate("ChemSynDlg", "10", 0, QApplication::UnicodeUTF8));
    STDAmplL->setText(QApplication::translate("ChemSynDlg", "Amplitude", 0, QApplication::UnicodeUTF8));
    STDtauAmplE->setText(QApplication::translate("ChemSynDlg", "25.0", 0, QApplication::UnicodeUTF8));
    STDVThreshL->setText(QApplication::translate("ChemSynDlg", "VThresh", 0, QApplication::UnicodeUTF8));
    STDtauAmplL->setText(QApplication::translate("ChemSynDlg", "tauAmpl", 0, QApplication::UnicodeUTF8));
    STDVSlopeL->setText(QApplication::translate("ChemSynDlg", "VSlope", 0, QApplication::UnicodeUTF8));
    STDVThreshU->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    STDVThreshE->setText(QApplication::translate("ChemSynDlg", "0.0", 0, QApplication::UnicodeUTF8));
    STDAmplE->setText(QApplication::translate("ChemSynDlg", "1.0", 0, QApplication::UnicodeUTF8));
    STDVSlopeU->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    STDtau0U->setText(QApplication::translate("ChemSynDlg", "ms", 0, QApplication::UnicodeUTF8));
    STDtau0E->setText(QApplication::translate("ChemSynDlg", "-20.0", 0, QApplication::UnicodeUTF8));
    STDtau0L->setText(QApplication::translate("ChemSynDlg", "tau0", 0, QApplication::UnicodeUTF8));
    STDtauVThreshL->setText(QApplication::translate("ChemSynDlg", "tauVThresh", 0, QApplication::UnicodeUTF8));
    STDtauVThreshU->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    STDtauVThreshE->setText(QApplication::translate("ChemSynDlg", "25.0", 0, QApplication::UnicodeUTF8));
    STDtauVSlopeU->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    STDtauVSlopeL->setText(QApplication::translate("ChemSynDlg", "tauVSlope", 0, QApplication::UnicodeUTF8));
    STDtauVSlopeE->setText(QApplication::translate("ChemSynDlg", "25.0", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("ChemSynDlg", "fix Vpost", 0, QApplication::UnicodeUTF8));
    fixVpostCombo->clear();
    fixVpostCombo->insertItems(0, QStringList()
     << QApplication::translate("ChemSynDlg", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "Fixed", 0, QApplication::UnicodeUTF8)
    );
    label_29->setText(QApplication::translate("ChemSynDlg", "Vpost", 0, QApplication::UnicodeUTF8));
    label_30->setText(QApplication::translate("ChemSynDlg", "mV", 0, QApplication::UnicodeUTF8));
    VpostE->setText(QApplication::translate("ChemSynDlg", "0.0", 0, QApplication::UnicodeUTF8));
    label_31->setText(QApplication::translate("ChemSynDlg", "Plasticity", 0, QApplication::UnicodeUTF8));
    PlasticityCombo->clear();
    PlasticityCombo->insertItems(0, QStringList()
     << QApplication::translate("ChemSynDlg", "None", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "Spike STDP", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "ODE STDP", 0, QApplication::UnicodeUTF8)
    );
    PlastParaBut->setText(QApplication::translate("ChemSynDlg", "Parameters", 0, QApplication::UnicodeUTF8));
    STDCombo->clear();
    STDCombo->insertItems(0, QStringList()
     << QApplication::translate("ChemSynDlg", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "On", 0, QApplication::UnicodeUTF8)
    );
    OutSynChannelL->setText(QApplication::translate("ChemSynDlg", "Output Channel", 0, QApplication::UnicodeUTF8));
    OutSynChannelCombo->clear();
    OutSynChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("ChemSynDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "1", 0, QApplication::UnicodeUTF8)
    );
    LUCombo->clear();
    LUCombo->insertItems(0, QStringList()
     << QApplication::translate("ChemSynDlg", "Direct Functions", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ChemSynDlg", "LookupTables", 0, QApplication::UnicodeUTF8)
    );
    MethodL->setText(QApplication::translate("ChemSynDlg", "Method", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ChemSynDlg);
    } // retranslateUi

};

namespace Ui {
    class ChemSynDlg: public Ui_ChemSynDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHEMSYNDLG_H
