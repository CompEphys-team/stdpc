/********************************************************************************
** Form generated from reading ui file 'mainwin.ui'
**
** Created: Wed Sep 24 11:44:55 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWIN_H
#define UI_MAINWIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConfig;
    QAction *actionLoad_Protocol;
    QAction *actionSave_Protocol;
    QAction *actionLoad_Script;
    QAction *actionUnload_Script;
    QAction *actionExit;
    QAction *actionDAQ;
    QAction *actionAbout;
    QAction *actionInput_channels;
    QAction *actionOutput_channels;
    QAction *actionSave_config;
    QAction *actionExport_Log;
    QAction *actionClear_Log;
    QWidget *centralwidget;
    QGroupBox *DataBox;
    QGraphicsView *DataD1;
    QGraphicsView *DataD2;
    QPushButton *Graph1SetBut;
    QPushButton *Graph2SetBut;
    QGroupBox *HHBox;
    QFrame *frame_11;
    QLabel *HH0L;
    QComboBox *HH0Combo;
    QPushButton *HH0ParaBut;
    QFrame *frame_12;
    QLabel *HH1L;
    QComboBox *HH1Combo;
    QPushButton *HH1ParaBut;
    QFrame *frame_13;
    QLabel *HH2L;
    QPushButton *HH2ParaBut;
    QComboBox *HH2Combo;
    QFrame *frame_14;
    QLabel *HH3L;
    QPushButton *HH3ParaBut;
    QComboBox *HH3Combo;
    QFrame *frame_15;
    QLabel *HH4L;
    QPushButton *HH4ParaBut;
    QComboBox *HH4Combo;
    QFrame *frame_16;
    QLabel *HH5L;
    QComboBox *HH5Combo;
    QPushButton *HH5ParaBut;
    QGroupBox *SynBox;
    QFrame *frame;
    QLabel *Syn0L;
    QComboBox *Syn0Combo;
    QPushButton *Syn0ParaBut;
    QFrame *frame_2;
    QLabel *Syn1L;
    QComboBox *Syn1Combo;
    QPushButton *Syn1ParaBut;
    QFrame *frame_5;
    QLabel *Syn2L;
    QComboBox *Syn2Combo;
    QPushButton *Syn2ParaBut;
    QFrame *frame_8;
    QLabel *Syn3L;
    QComboBox *Syn3Combo;
    QPushButton *Syn3ParaBut;
    QFrame *frame_9;
    QLabel *Syn4L;
    QPushButton *Syn4ParaBut;
    QComboBox *Syn4Combo;
    QFrame *frame_10;
    QLabel *Syn5L;
    QPushButton *Syn5ParaBut;
    QComboBox *Syn5Combo;
    QGroupBox *groupBox;
    QComboBox *SGMethodCombo;
    QComboBox *BurstDetectionCombo;
    QLabel *label_31;
    QLabel *BurstDetectionL;
    QLabel *SpikesL;
    QLabel *VSpikeL;
    QLineEdit *VSpikeE;
    QLabel *VSpikeU;
    QLabel *WidthL;
    QLineEdit *WidthE;
    QLabel *WidthU;
    QLabel *VRestL;
    QLineEdit *VRestE;
    QLabel *VRestU;
    QLabel *ExplicitSpikeTimesL;
    QLineEdit *NumberE;
    QLabel *NumberL;
    QLabel *NumberU;
    QLineEdit *PeriodE;
    QLabel *PeriodL;
    QLabel *PeriodU;
    QPushButton *SpikeTimesBut;
    QLabel *ChannelL;
    QLineEdit *ThresholdE;
    QLabel *ThresholdL;
    QLabel *ThresholdU;
    QLineEdit *NUnderE;
    QLabel *NUnderL;
    QLineEdit *NOverE;
    QLabel *NOverL;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QLabel *STInputFileL;
    QLineEdit *STInputFileE;
    QComboBox *SGLUTableCombo;
    QComboBox *SGbdChannelCombo;
    QPushButton *StartBut;
    QPushButton *StopBut;
    QListWidget *MessageWindow;
    QComboBox *DAQComboBox;
    QLabel *DAQL;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuConfig;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(1013, 775);
    actionConfig = new QAction(MainWindow);
    actionConfig->setObjectName(QString::fromUtf8("actionConfig"));
    actionLoad_Protocol = new QAction(MainWindow);
    actionLoad_Protocol->setObjectName(QString::fromUtf8("actionLoad_Protocol"));
    actionSave_Protocol = new QAction(MainWindow);
    actionSave_Protocol->setObjectName(QString::fromUtf8("actionSave_Protocol"));
    actionLoad_Script = new QAction(MainWindow);
    actionLoad_Script->setObjectName(QString::fromUtf8("actionLoad_Script"));
    actionUnload_Script = new QAction(MainWindow);
    actionUnload_Script->setObjectName(QString::fromUtf8("actionUnload_Script"));
    actionExit = new QAction(MainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    actionDAQ = new QAction(MainWindow);
    actionDAQ->setObjectName(QString::fromUtf8("actionDAQ"));
    actionAbout = new QAction(MainWindow);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionInput_channels = new QAction(MainWindow);
    actionInput_channels->setObjectName(QString::fromUtf8("actionInput_channels"));
    actionOutput_channels = new QAction(MainWindow);
    actionOutput_channels->setObjectName(QString::fromUtf8("actionOutput_channels"));
    actionSave_config = new QAction(MainWindow);
    actionSave_config->setObjectName(QString::fromUtf8("actionSave_config"));
    actionExport_Log = new QAction(MainWindow);
    actionExport_Log->setObjectName(QString::fromUtf8("actionExport_Log"));
    actionClear_Log = new QAction(MainWindow);
    actionClear_Log->setObjectName(QString::fromUtf8("actionClear_Log"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    DataBox = new QGroupBox(centralwidget);
    DataBox->setObjectName(QString::fromUtf8("DataBox"));
    DataBox->setGeometry(QRect(360, 330, 641, 391));
    DataBox->setFlat(false);
    DataBox->setCheckable(false);
    DataD1 = new QGraphicsView(DataBox);
    DataD1->setObjectName(QString::fromUtf8("DataD1"));
    DataD1->setGeometry(QRect(10, 20, 531, 171));
    DataD1->setFrameShape(QFrame::Panel);
    DataD2 = new QGraphicsView(DataBox);
    DataD2->setObjectName(QString::fromUtf8("DataD2"));
    DataD2->setGeometry(QRect(10, 210, 531, 171));
    DataD2->setFrameShape(QFrame::Panel);
    Graph1SetBut = new QPushButton(DataBox);
    Graph1SetBut->setObjectName(QString::fromUtf8("Graph1SetBut"));
    Graph1SetBut->setGeometry(QRect(550, 20, 75, 23));
    Graph2SetBut = new QPushButton(DataBox);
    Graph2SetBut->setObjectName(QString::fromUtf8("Graph2SetBut"));
    Graph2SetBut->setGeometry(QRect(550, 210, 75, 23));
    HHBox = new QGroupBox(centralwidget);
    HHBox->setObjectName(QString::fromUtf8("HHBox"));
    HHBox->setGeometry(QRect(270, 190, 731, 131));
    frame_11 = new QFrame(HHBox);
    frame_11->setObjectName(QString::fromUtf8("frame_11"));
    frame_11->setGeometry(QRect(10, 20, 111, 101));
    frame_11->setAutoFillBackground(false);
    frame_11->setFrameShape(QFrame::Panel);
    frame_11->setFrameShadow(QFrame::Sunken);
    frame_11->setLineWidth(1);
    frame_11->setMidLineWidth(0);
    HH0L = new QLabel(frame_11);
    HH0L->setObjectName(QString::fromUtf8("HH0L"));
    HH0L->setEnabled(false);
    HH0L->setGeometry(QRect(10, 10, 76, 20));
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);
    HH0L->setFont(font);
    HH0L->setAutoFillBackground(true);
    HH0L->setStyleSheet(QString::fromUtf8(""));
    HH0L->setFrameShape(QFrame::Panel);
    HH0L->setFrameShadow(QFrame::Raised);
    HH0L->setLineWidth(1);
    HH0L->setMidLineWidth(0);
    HH0Combo = new QComboBox(frame_11);
    HH0Combo->setObjectName(QString::fromUtf8("HH0Combo"));
    HH0Combo->setGeometry(QRect(10, 40, 91, 22));
    HH0ParaBut = new QPushButton(frame_11);
    HH0ParaBut->setObjectName(QString::fromUtf8("HH0ParaBut"));
    HH0ParaBut->setEnabled(false);
    HH0ParaBut->setGeometry(QRect(10, 70, 91, 23));
    frame_12 = new QFrame(HHBox);
    frame_12->setObjectName(QString::fromUtf8("frame_12"));
    frame_12->setGeometry(QRect(130, 20, 111, 101));
    frame_12->setFrameShape(QFrame::Panel);
    frame_12->setFrameShadow(QFrame::Sunken);
    frame_12->setLineWidth(1);
    frame_12->setMidLineWidth(0);
    HH1L = new QLabel(frame_12);
    HH1L->setObjectName(QString::fromUtf8("HH1L"));
    HH1L->setEnabled(false);
    HH1L->setGeometry(QRect(10, 10, 76, 20));
    HH1L->setFont(font);
    HH1L->setAutoFillBackground(true);
    HH1L->setStyleSheet(QString::fromUtf8(""));
    HH1L->setFrameShape(QFrame::Panel);
    HH1L->setFrameShadow(QFrame::Raised);
    HH1L->setLineWidth(1);
    HH1L->setMidLineWidth(0);
    HH1Combo = new QComboBox(frame_12);
    HH1Combo->setObjectName(QString::fromUtf8("HH1Combo"));
    HH1Combo->setGeometry(QRect(10, 40, 91, 22));
    HH1ParaBut = new QPushButton(frame_12);
    HH1ParaBut->setObjectName(QString::fromUtf8("HH1ParaBut"));
    HH1ParaBut->setEnabled(false);
    HH1ParaBut->setGeometry(QRect(10, 70, 91, 23));
    frame_13 = new QFrame(HHBox);
    frame_13->setObjectName(QString::fromUtf8("frame_13"));
    frame_13->setGeometry(QRect(250, 20, 111, 101));
    frame_13->setFrameShape(QFrame::Panel);
    frame_13->setFrameShadow(QFrame::Sunken);
    frame_13->setLineWidth(1);
    frame_13->setMidLineWidth(0);
    HH2L = new QLabel(frame_13);
    HH2L->setObjectName(QString::fromUtf8("HH2L"));
    HH2L->setEnabled(false);
    HH2L->setGeometry(QRect(10, 10, 76, 20));
    HH2L->setFont(font);
    HH2L->setAutoFillBackground(true);
    HH2L->setStyleSheet(QString::fromUtf8(""));
    HH2L->setFrameShape(QFrame::Panel);
    HH2L->setFrameShadow(QFrame::Raised);
    HH2L->setLineWidth(1);
    HH2L->setMidLineWidth(0);
    HH2ParaBut = new QPushButton(frame_13);
    HH2ParaBut->setObjectName(QString::fromUtf8("HH2ParaBut"));
    HH2ParaBut->setEnabled(false);
    HH2ParaBut->setGeometry(QRect(10, 70, 91, 23));
    HH2Combo = new QComboBox(frame_13);
    HH2Combo->setObjectName(QString::fromUtf8("HH2Combo"));
    HH2Combo->setGeometry(QRect(10, 40, 91, 22));
    frame_14 = new QFrame(HHBox);
    frame_14->setObjectName(QString::fromUtf8("frame_14"));
    frame_14->setGeometry(QRect(370, 20, 111, 101));
    frame_14->setFrameShape(QFrame::Panel);
    frame_14->setFrameShadow(QFrame::Sunken);
    frame_14->setLineWidth(1);
    frame_14->setMidLineWidth(0);
    HH3L = new QLabel(frame_14);
    HH3L->setObjectName(QString::fromUtf8("HH3L"));
    HH3L->setEnabled(false);
    HH3L->setGeometry(QRect(10, 10, 76, 20));
    HH3L->setFont(font);
    HH3L->setAutoFillBackground(true);
    HH3L->setStyleSheet(QString::fromUtf8(""));
    HH3L->setFrameShape(QFrame::Panel);
    HH3L->setFrameShadow(QFrame::Raised);
    HH3L->setLineWidth(1);
    HH3L->setMidLineWidth(0);
    HH3ParaBut = new QPushButton(frame_14);
    HH3ParaBut->setObjectName(QString::fromUtf8("HH3ParaBut"));
    HH3ParaBut->setEnabled(false);
    HH3ParaBut->setGeometry(QRect(10, 70, 91, 23));
    HH3Combo = new QComboBox(frame_14);
    HH3Combo->setObjectName(QString::fromUtf8("HH3Combo"));
    HH3Combo->setGeometry(QRect(10, 40, 91, 22));
    frame_15 = new QFrame(HHBox);
    frame_15->setObjectName(QString::fromUtf8("frame_15"));
    frame_15->setGeometry(QRect(490, 20, 111, 101));
    frame_15->setFrameShape(QFrame::Panel);
    frame_15->setFrameShadow(QFrame::Sunken);
    frame_15->setLineWidth(1);
    frame_15->setMidLineWidth(0);
    HH4L = new QLabel(frame_15);
    HH4L->setObjectName(QString::fromUtf8("HH4L"));
    HH4L->setEnabled(false);
    HH4L->setGeometry(QRect(10, 10, 76, 20));
    HH4L->setFont(font);
    HH4L->setAutoFillBackground(true);
    HH4L->setStyleSheet(QString::fromUtf8(""));
    HH4L->setFrameShape(QFrame::Panel);
    HH4L->setFrameShadow(QFrame::Raised);
    HH4L->setLineWidth(1);
    HH4L->setMidLineWidth(0);
    HH4ParaBut = new QPushButton(frame_15);
    HH4ParaBut->setObjectName(QString::fromUtf8("HH4ParaBut"));
    HH4ParaBut->setEnabled(false);
    HH4ParaBut->setGeometry(QRect(10, 70, 91, 23));
    HH4Combo = new QComboBox(frame_15);
    HH4Combo->setObjectName(QString::fromUtf8("HH4Combo"));
    HH4Combo->setGeometry(QRect(10, 40, 91, 22));
    frame_16 = new QFrame(HHBox);
    frame_16->setObjectName(QString::fromUtf8("frame_16"));
    frame_16->setGeometry(QRect(610, 20, 111, 101));
    frame_16->setFrameShape(QFrame::Panel);
    frame_16->setFrameShadow(QFrame::Sunken);
    frame_16->setLineWidth(1);
    frame_16->setMidLineWidth(0);
    HH5L = new QLabel(frame_16);
    HH5L->setObjectName(QString::fromUtf8("HH5L"));
    HH5L->setEnabled(false);
    HH5L->setGeometry(QRect(10, 10, 76, 20));
    HH5L->setFont(font);
    HH5L->setAutoFillBackground(true);
    HH5L->setStyleSheet(QString::fromUtf8(""));
    HH5L->setFrameShape(QFrame::Panel);
    HH5L->setFrameShadow(QFrame::Raised);
    HH5L->setLineWidth(1);
    HH5L->setMidLineWidth(0);
    HH5Combo = new QComboBox(frame_16);
    HH5Combo->setObjectName(QString::fromUtf8("HH5Combo"));
    HH5Combo->setGeometry(QRect(10, 40, 91, 22));
    HH5ParaBut = new QPushButton(frame_16);
    HH5ParaBut->setObjectName(QString::fromUtf8("HH5ParaBut"));
    HH5ParaBut->setEnabled(false);
    HH5ParaBut->setGeometry(QRect(10, 70, 91, 23));
    SynBox = new QGroupBox(centralwidget);
    SynBox->setObjectName(QString::fromUtf8("SynBox"));
    SynBox->setGeometry(QRect(270, 50, 731, 131));
    frame = new QFrame(SynBox);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setGeometry(QRect(10, 20, 111, 101));
    frame->setFrameShape(QFrame::Panel);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(1);
    frame->setMidLineWidth(0);
    Syn0L = new QLabel(frame);
    Syn0L->setObjectName(QString::fromUtf8("Syn0L"));
    Syn0L->setEnabled(false);
    Syn0L->setGeometry(QRect(10, 10, 76, 20));
    Syn0L->setFont(font);
    Syn0L->setAutoFillBackground(true);
    Syn0L->setStyleSheet(QString::fromUtf8(""));
    Syn0L->setFrameShape(QFrame::Panel);
    Syn0L->setFrameShadow(QFrame::Raised);
    Syn0L->setLineWidth(1);
    Syn0L->setMidLineWidth(0);
    Syn0Combo = new QComboBox(frame);
    Syn0Combo->setObjectName(QString::fromUtf8("Syn0Combo"));
    Syn0Combo->setGeometry(QRect(10, 40, 91, 22));
    Syn0ParaBut = new QPushButton(frame);
    Syn0ParaBut->setObjectName(QString::fromUtf8("Syn0ParaBut"));
    Syn0ParaBut->setEnabled(false);
    Syn0ParaBut->setGeometry(QRect(10, 70, 91, 23));
    frame_2 = new QFrame(SynBox);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setGeometry(QRect(129, 20, 111, 101));
    frame_2->setFrameShape(QFrame::Panel);
    frame_2->setFrameShadow(QFrame::Sunken);
    frame_2->setMidLineWidth(0);
    Syn1L = new QLabel(frame_2);
    Syn1L->setObjectName(QString::fromUtf8("Syn1L"));
    Syn1L->setEnabled(false);
    Syn1L->setGeometry(QRect(10, 10, 76, 20));
    Syn1L->setFont(font);
    Syn1L->setFrameShape(QFrame::Panel);
    Syn1L->setFrameShadow(QFrame::Raised);
    Syn1Combo = new QComboBox(frame_2);
    Syn1Combo->setObjectName(QString::fromUtf8("Syn1Combo"));
    Syn1Combo->setGeometry(QRect(10, 40, 91, 22));
    Syn1ParaBut = new QPushButton(frame_2);
    Syn1ParaBut->setObjectName(QString::fromUtf8("Syn1ParaBut"));
    Syn1ParaBut->setEnabled(false);
    Syn1ParaBut->setGeometry(QRect(10, 70, 91, 23));
    frame_5 = new QFrame(SynBox);
    frame_5->setObjectName(QString::fromUtf8("frame_5"));
    frame_5->setGeometry(QRect(250, 20, 111, 101));
    frame_5->setFrameShape(QFrame::Panel);
    frame_5->setFrameShadow(QFrame::Sunken);
    Syn2L = new QLabel(frame_5);
    Syn2L->setObjectName(QString::fromUtf8("Syn2L"));
    Syn2L->setEnabled(false);
    Syn2L->setGeometry(QRect(10, 10, 76, 20));
    Syn2L->setFont(font);
    Syn2L->setFrameShape(QFrame::Panel);
    Syn2L->setFrameShadow(QFrame::Raised);
    Syn2Combo = new QComboBox(frame_5);
    Syn2Combo->setObjectName(QString::fromUtf8("Syn2Combo"));
    Syn2Combo->setGeometry(QRect(10, 40, 91, 22));
    Syn2ParaBut = new QPushButton(frame_5);
    Syn2ParaBut->setObjectName(QString::fromUtf8("Syn2ParaBut"));
    Syn2ParaBut->setEnabled(false);
    Syn2ParaBut->setGeometry(QRect(10, 70, 91, 23));
    frame_8 = new QFrame(SynBox);
    frame_8->setObjectName(QString::fromUtf8("frame_8"));
    frame_8->setGeometry(QRect(370, 20, 111, 101));
    frame_8->setFrameShape(QFrame::Panel);
    frame_8->setFrameShadow(QFrame::Sunken);
    Syn3L = new QLabel(frame_8);
    Syn3L->setObjectName(QString::fromUtf8("Syn3L"));
    Syn3L->setEnabled(false);
    Syn3L->setGeometry(QRect(10, 10, 76, 20));
    Syn3L->setFont(font);
    Syn3L->setFrameShape(QFrame::Panel);
    Syn3L->setFrameShadow(QFrame::Raised);
    Syn3Combo = new QComboBox(frame_8);
    Syn3Combo->setObjectName(QString::fromUtf8("Syn3Combo"));
    Syn3Combo->setGeometry(QRect(10, 40, 91, 22));
    Syn3ParaBut = new QPushButton(frame_8);
    Syn3ParaBut->setObjectName(QString::fromUtf8("Syn3ParaBut"));
    Syn3ParaBut->setEnabled(false);
    Syn3ParaBut->setGeometry(QRect(10, 70, 91, 23));
    frame_9 = new QFrame(SynBox);
    frame_9->setObjectName(QString::fromUtf8("frame_9"));
    frame_9->setGeometry(QRect(490, 20, 111, 101));
    frame_9->setFrameShape(QFrame::Panel);
    frame_9->setFrameShadow(QFrame::Sunken);
    Syn4L = new QLabel(frame_9);
    Syn4L->setObjectName(QString::fromUtf8("Syn4L"));
    Syn4L->setEnabled(false);
    Syn4L->setGeometry(QRect(10, 10, 76, 20));
    Syn4L->setFont(font);
    Syn4L->setFrameShape(QFrame::Panel);
    Syn4L->setFrameShadow(QFrame::Raised);
    Syn4ParaBut = new QPushButton(frame_9);
    Syn4ParaBut->setObjectName(QString::fromUtf8("Syn4ParaBut"));
    Syn4ParaBut->setEnabled(false);
    Syn4ParaBut->setGeometry(QRect(10, 70, 91, 23));
    Syn4Combo = new QComboBox(frame_9);
    Syn4Combo->setObjectName(QString::fromUtf8("Syn4Combo"));
    Syn4Combo->setGeometry(QRect(10, 40, 91, 22));
    frame_10 = new QFrame(SynBox);
    frame_10->setObjectName(QString::fromUtf8("frame_10"));
    frame_10->setGeometry(QRect(610, 20, 111, 101));
    frame_10->setFrameShape(QFrame::Panel);
    frame_10->setFrameShadow(QFrame::Sunken);
    Syn5L = new QLabel(frame_10);
    Syn5L->setObjectName(QString::fromUtf8("Syn5L"));
    Syn5L->setEnabled(false);
    Syn5L->setGeometry(QRect(10, 10, 76, 20));
    Syn5L->setFont(font);
    Syn5L->setFrameShape(QFrame::Panel);
    Syn5L->setFrameShadow(QFrame::Raised);
    Syn5ParaBut = new QPushButton(frame_10);
    Syn5ParaBut->setObjectName(QString::fromUtf8("Syn5ParaBut"));
    Syn5ParaBut->setEnabled(false);
    Syn5ParaBut->setGeometry(QRect(10, 70, 91, 23));
    Syn5Combo = new QComboBox(frame_10);
    Syn5Combo->setObjectName(QString::fromUtf8("Syn5Combo"));
    Syn5Combo->setGeometry(QRect(10, 40, 91, 22));
    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 330, 341, 391));
    SGMethodCombo = new QComboBox(groupBox);
    SGMethodCombo->setObjectName(QString::fromUtf8("SGMethodCombo"));
    SGMethodCombo->setGeometry(QRect(20, 60, 131, 22));
    BurstDetectionCombo = new QComboBox(groupBox);
    BurstDetectionCombo->setObjectName(QString::fromUtf8("BurstDetectionCombo"));
    BurstDetectionCombo->setEnabled(false);
    BurstDetectionCombo->setGeometry(QRect(190, 130, 131, 22));
    label_31 = new QLabel(groupBox);
    label_31->setObjectName(QString::fromUtf8("label_31"));
    label_31->setGeometry(QRect(20, 30, 77, 16));
    QFont font1;
    font1.setBold(true);
    font1.setWeight(75);
    label_31->setFont(font1);
    BurstDetectionL = new QLabel(groupBox);
    BurstDetectionL->setObjectName(QString::fromUtf8("BurstDetectionL"));
    BurstDetectionL->setEnabled(false);
    BurstDetectionL->setGeometry(QRect(190, 100, 87, 16));
    BurstDetectionL->setFont(font1);
    SpikesL = new QLabel(groupBox);
    SpikesL->setObjectName(QString::fromUtf8("SpikesL"));
    SpikesL->setEnabled(false);
    SpikesL->setGeometry(QRect(20, 100, 46, 14));
    SpikesL->setFont(font1);
    VSpikeL = new QLabel(groupBox);
    VSpikeL->setObjectName(QString::fromUtf8("VSpikeL"));
    VSpikeL->setEnabled(false);
    VSpikeL->setGeometry(QRect(20, 130, 31, 16));
    VSpikeE = new QLineEdit(groupBox);
    VSpikeE->setObjectName(QString::fromUtf8("VSpikeE"));
    VSpikeE->setEnabled(false);
    VSpikeE->setGeometry(QRect(60, 130, 61, 20));
    VSpikeE->setMaxLength(8);
    VSpikeE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    VSpikeU = new QLabel(groupBox);
    VSpikeU->setObjectName(QString::fromUtf8("VSpikeU"));
    VSpikeU->setEnabled(false);
    VSpikeU->setGeometry(QRect(130, 130, 16, 16));
    WidthL = new QLabel(groupBox);
    WidthL->setObjectName(QString::fromUtf8("WidthL"));
    WidthL->setEnabled(false);
    WidthL->setGeometry(QRect(20, 160, 31, 16));
    WidthE = new QLineEdit(groupBox);
    WidthE->setObjectName(QString::fromUtf8("WidthE"));
    WidthE->setEnabled(false);
    WidthE->setGeometry(QRect(60, 160, 61, 20));
    WidthE->setMaxLength(8);
    WidthE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    WidthU = new QLabel(groupBox);
    WidthU->setObjectName(QString::fromUtf8("WidthU"));
    WidthU->setEnabled(false);
    WidthU->setGeometry(QRect(130, 160, 16, 16));
    VRestL = new QLabel(groupBox);
    VRestL->setObjectName(QString::fromUtf8("VRestL"));
    VRestL->setEnabled(false);
    VRestL->setGeometry(QRect(20, 190, 31, 16));
    VRestE = new QLineEdit(groupBox);
    VRestE->setObjectName(QString::fromUtf8("VRestE"));
    VRestE->setEnabled(false);
    VRestE->setGeometry(QRect(60, 190, 61, 20));
    VRestE->setMaxLength(8);
    VRestE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    VRestU = new QLabel(groupBox);
    VRestU->setObjectName(QString::fromUtf8("VRestU"));
    VRestU->setEnabled(false);
    VRestU->setGeometry(QRect(130, 190, 16, 16));
    ExplicitSpikeTimesL = new QLabel(groupBox);
    ExplicitSpikeTimesL->setObjectName(QString::fromUtf8("ExplicitSpikeTimesL"));
    ExplicitSpikeTimesL->setEnabled(false);
    ExplicitSpikeTimesL->setGeometry(QRect(20, 250, 111, 16));
    ExplicitSpikeTimesL->setFont(font1);
    NumberE = new QLineEdit(groupBox);
    NumberE->setObjectName(QString::fromUtf8("NumberE"));
    NumberE->setEnabled(false);
    NumberE->setGeometry(QRect(60, 280, 61, 20));
    NumberE->setMaxLength(2);
    NumberE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    NumberL = new QLabel(groupBox);
    NumberL->setObjectName(QString::fromUtf8("NumberL"));
    NumberL->setEnabled(false);
    NumberL->setGeometry(QRect(20, 280, 37, 16));
    NumberU = new QLabel(groupBox);
    NumberU->setObjectName(QString::fromUtf8("NumberU"));
    NumberU->setEnabled(false);
    NumberU->setGeometry(QRect(130, 280, 46, 14));
    PeriodE = new QLineEdit(groupBox);
    PeriodE->setObjectName(QString::fromUtf8("PeriodE"));
    PeriodE->setEnabled(false);
    PeriodE->setGeometry(QRect(60, 310, 61, 20));
    PeriodE->setMaxLength(8);
    PeriodE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    PeriodL = new QLabel(groupBox);
    PeriodL->setObjectName(QString::fromUtf8("PeriodL"));
    PeriodL->setEnabled(false);
    PeriodL->setGeometry(QRect(20, 310, 31, 16));
    PeriodU = new QLabel(groupBox);
    PeriodU->setObjectName(QString::fromUtf8("PeriodU"));
    PeriodU->setEnabled(false);
    PeriodU->setGeometry(QRect(130, 310, 16, 16));
    SpikeTimesBut = new QPushButton(groupBox);
    SpikeTimesBut->setObjectName(QString::fromUtf8("SpikeTimesBut"));
    SpikeTimesBut->setEnabled(false);
    SpikeTimesBut->setGeometry(QRect(20, 340, 101, 23));
    ChannelL = new QLabel(groupBox);
    ChannelL->setObjectName(QString::fromUtf8("ChannelL"));
    ChannelL->setEnabled(false);
    ChannelL->setGeometry(QRect(190, 160, 39, 16));
    ThresholdE = new QLineEdit(groupBox);
    ThresholdE->setObjectName(QString::fromUtf8("ThresholdE"));
    ThresholdE->setEnabled(false);
    ThresholdE->setGeometry(QRect(250, 190, 51, 20));
    ThresholdE->setMaxLength(8);
    ThresholdE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ThresholdL = new QLabel(groupBox);
    ThresholdL->setObjectName(QString::fromUtf8("ThresholdL"));
    ThresholdL->setEnabled(false);
    ThresholdL->setGeometry(QRect(190, 190, 47, 16));
    ThresholdU = new QLabel(groupBox);
    ThresholdU->setObjectName(QString::fromUtf8("ThresholdU"));
    ThresholdU->setEnabled(false);
    ThresholdU->setGeometry(QRect(310, 190, 16, 16));
    NUnderE = new QLineEdit(groupBox);
    NUnderE->setObjectName(QString::fromUtf8("NUnderE"));
    NUnderE->setEnabled(false);
    NUnderE->setGeometry(QRect(250, 220, 51, 20));
    NUnderE->setMaxLength(8);
    NUnderE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    NUnderL = new QLabel(groupBox);
    NUnderL->setObjectName(QString::fromUtf8("NUnderL"));
    NUnderL->setEnabled(false);
    NUnderL->setGeometry(QRect(190, 220, 39, 16));
    NOverE = new QLineEdit(groupBox);
    NOverE->setObjectName(QString::fromUtf8("NOverE"));
    NOverE->setEnabled(false);
    NOverE->setGeometry(QRect(250, 250, 51, 20));
    NOverE->setMaxLength(8);
    NOverE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    NOverL = new QLabel(groupBox);
    NOverL->setObjectName(QString::fromUtf8("NOverL"));
    NOverL->setEnabled(false);
    NOverL->setGeometry(QRect(190, 250, 39, 16));
    line = new QFrame(groupBox);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(20, 230, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line_2 = new QFrame(groupBox);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setGeometry(QRect(190, 290, 118, 3));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    line_3 = new QFrame(groupBox);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    line_3->setGeometry(QRect(160, 100, 20, 271));
    line_3->setFrameShape(QFrame::VLine);
    line_3->setFrameShadow(QFrame::Sunken);
    STInputFileL = new QLabel(groupBox);
    STInputFileL->setObjectName(QString::fromUtf8("STInputFileL"));
    STInputFileL->setEnabled(false);
    STInputFileL->setGeometry(QRect(190, 310, 118, 16));
    STInputFileL->setFont(font1);
    STInputFileE = new QLineEdit(groupBox);
    STInputFileE->setObjectName(QString::fromUtf8("STInputFileE"));
    STInputFileE->setEnabled(false);
    STInputFileE->setGeometry(QRect(190, 340, 131, 20));
    SGLUTableCombo = new QComboBox(groupBox);
    SGLUTableCombo->setObjectName(QString::fromUtf8("SGLUTableCombo"));
    SGLUTableCombo->setGeometry(QRect(190, 60, 131, 22));
    SGbdChannelCombo = new QComboBox(groupBox);
    SGbdChannelCombo->setObjectName(QString::fromUtf8("SGbdChannelCombo"));
    SGbdChannelCombo->setEnabled(false);
    SGbdChannelCombo->setGeometry(QRect(250, 160, 51, 22));
    StartBut = new QPushButton(centralwidget);
    StartBut->setObjectName(QString::fromUtf8("StartBut"));
    StartBut->setGeometry(QRect(10, 10, 101, 31));
    StartBut->setFont(font);
    StopBut = new QPushButton(centralwidget);
    StopBut->setObjectName(QString::fromUtf8("StopBut"));
    StopBut->setGeometry(QRect(120, 10, 101, 31));
    StopBut->setFont(font);
    MessageWindow = new QListWidget(centralwidget);
    MessageWindow->setObjectName(QString::fromUtf8("MessageWindow"));
    MessageWindow->setGeometry(QRect(10, 50, 251, 271));
    DAQComboBox = new QComboBox(centralwidget);
    DAQComboBox->setObjectName(QString::fromUtf8("DAQComboBox"));
    DAQComboBox->setGeometry(QRect(400, 20, 121, 22));
    DAQL = new QLabel(centralwidget);
    DAQL->setObjectName(QString::fromUtf8("DAQL"));
    DAQL->setEnabled(true);
    DAQL->setGeometry(QRect(270, 20, 121, 16));
    DAQL->setFont(font1);
    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 1013, 25));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuConfig = new QMenu(menubar);
    menuConfig->setObjectName(QString::fromUtf8("menuConfig"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuConfig->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionLoad_Protocol);
    menuFile->addAction(actionSave_Protocol);
    menuFile->addAction(actionLoad_Script);
    menuFile->addAction(actionUnload_Script);
    menuFile->addAction(actionExport_Log);
    menuFile->addAction(actionClear_Log);
    menuFile->addAction(actionExit);
    menuConfig->addAction(actionDAQ);
    menuConfig->addAction(actionInput_channels);
    menuConfig->addAction(actionOutput_channels);
    menuConfig->addAction(actionSave_config);
    menuHelp->addAction(actionAbout);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "StdpC 2007", 0, QApplication::UnicodeUTF8));
    actionConfig->setText(QApplication::translate("MainWindow", "Config", 0, QApplication::UnicodeUTF8));
    actionLoad_Protocol->setText(QApplication::translate("MainWindow", "Load Protocol", 0, QApplication::UnicodeUTF8));
    actionSave_Protocol->setText(QApplication::translate("MainWindow", "Save Protocol", 0, QApplication::UnicodeUTF8));
    actionLoad_Script->setText(QApplication::translate("MainWindow", "Load Script", 0, QApplication::UnicodeUTF8));
    actionUnload_Script->setText(QApplication::translate("MainWindow", "Unload Script", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
    actionDAQ->setText(QApplication::translate("MainWindow", "DAQ", 0, QApplication::UnicodeUTF8));
    actionAbout->setText(QApplication::translate("MainWindow", "About StdpC", 0, QApplication::UnicodeUTF8));
    actionInput_channels->setText(QApplication::translate("MainWindow", "Input channels", 0, QApplication::UnicodeUTF8));
    actionOutput_channels->setText(QApplication::translate("MainWindow", "Output channels", 0, QApplication::UnicodeUTF8));
    actionSave_config->setText(QApplication::translate("MainWindow", "Save config", 0, QApplication::UnicodeUTF8));
    actionExport_Log->setText(QApplication::translate("MainWindow", "Export Log", 0, QApplication::UnicodeUTF8));
    actionClear_Log->setText(QApplication::translate("MainWindow", "Clear Log", 0, QApplication::UnicodeUTF8));
    DataBox->setTitle(QApplication::translate("MainWindow", "Data Displays", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    DataD1->setToolTip(QApplication::translate("MainWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">First flexible data display</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    DataD2->setToolTip(QApplication::translate("MainWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Second flexible data display</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    Graph1SetBut->setText(QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
    Graph2SetBut->setText(QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
    HHBox->setTitle(QApplication::translate("MainWindow", "Ionic conductances", 0, QApplication::UnicodeUTF8));
    HH0L->setText(QApplication::translate("MainWindow", "HH 0", 0, QApplication::UnicodeUTF8));
    HH0Combo->clear();
    HH0Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "m/h/tau", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "alpha/beta", 0, QApplication::UnicodeUTF8)
    );
    HH0ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    HH1L->setText(QApplication::translate("MainWindow", "HH 1", 0, QApplication::UnicodeUTF8));
    HH1Combo->clear();
    HH1Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "m/h/tau", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "alpha/beta", 0, QApplication::UnicodeUTF8)
    );
    HH1ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    HH2L->setText(QApplication::translate("MainWindow", "HH 2", 0, QApplication::UnicodeUTF8));
    HH2ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    HH2Combo->clear();
    HH2Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "m/h/tau", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "alpha/beta", 0, QApplication::UnicodeUTF8)
    );
    HH3L->setText(QApplication::translate("MainWindow", "HH 3", 0, QApplication::UnicodeUTF8));
    HH3ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    HH3Combo->clear();
    HH3Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "m/h/tau", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "alpha/beta", 0, QApplication::UnicodeUTF8)
    );
    HH4L->setText(QApplication::translate("MainWindow", "HH 4", 0, QApplication::UnicodeUTF8));
    HH4ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    HH4Combo->clear();
    HH4Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "m/h/tau", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "alpha/beta", 0, QApplication::UnicodeUTF8)
    );
    HH5L->setText(QApplication::translate("MainWindow", "HH 5", 0, QApplication::UnicodeUTF8));
    HH5Combo->clear();
    HH5Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "m/h/tau", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "alpha/beta", 0, QApplication::UnicodeUTF8)
    );
    HH5ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    SynBox->setTitle(QApplication::translate("MainWindow", "Synapses", 0, QApplication::UnicodeUTF8));
    Syn0L->setText(QApplication::translate("MainWindow", "Synapse 0", 0, QApplication::UnicodeUTF8));
    Syn0Combo->clear();
    Syn0Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Chemical", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Gap Junction", 0, QApplication::UnicodeUTF8)
    );
    Syn0ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    Syn1L->setText(QApplication::translate("MainWindow", "Synapse 1", 0, QApplication::UnicodeUTF8));
    Syn1Combo->clear();
    Syn1Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Chemical", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Gap Junction", 0, QApplication::UnicodeUTF8)
    );
    Syn1ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    Syn2L->setText(QApplication::translate("MainWindow", "Synapse 2", 0, QApplication::UnicodeUTF8));
    Syn2Combo->clear();
    Syn2Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Chemical", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Gap Junction", 0, QApplication::UnicodeUTF8)
    );
    Syn2ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    Syn3L->setText(QApplication::translate("MainWindow", "Synapse 3", 0, QApplication::UnicodeUTF8));
    Syn3Combo->clear();
    Syn3Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Chemical", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Gap Junction", 0, QApplication::UnicodeUTF8)
    );
    Syn3ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    Syn4L->setText(QApplication::translate("MainWindow", "Synapse 4", 0, QApplication::UnicodeUTF8));
    Syn4ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    Syn4Combo->clear();
    Syn4Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Chemical", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Gap Junction", 0, QApplication::UnicodeUTF8)
    );
    Syn5L->setText(QApplication::translate("MainWindow", "Synapse 5", 0, QApplication::UnicodeUTF8));
    Syn5ParaBut->setText(QApplication::translate("MainWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    Syn5Combo->clear();
    Syn5Combo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Chemical", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Gap Junction", 0, QApplication::UnicodeUTF8)
    );
    groupBox->setTitle(QApplication::translate("MainWindow", "Spike generator", 0, QApplication::UnicodeUTF8));
    SGMethodCombo->clear();
    SGMethodCombo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Explicit spike times", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Spike times from file", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Voltage from file", 0, QApplication::UnicodeUTF8)
    );
    BurstDetectionCombo->clear();
    BurstDetectionCombo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "low->high detection", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "high->low detection", 0, QApplication::UnicodeUTF8)
    );
    label_31->setText(QApplication::translate("MainWindow", "Spike Method", 0, QApplication::UnicodeUTF8));
    BurstDetectionL->setText(QApplication::translate("MainWindow", "Burst detection", 0, QApplication::UnicodeUTF8));
    SpikesL->setText(QApplication::translate("MainWindow", "Spikes", 0, QApplication::UnicodeUTF8));
    VSpikeL->setText(QApplication::translate("MainWindow", "VSpike", 0, QApplication::UnicodeUTF8));
    VSpikeE->setInputMask(QString());
    VSpikeE->setText(QApplication::translate("MainWindow", "80.0", 0, QApplication::UnicodeUTF8));
    VSpikeU->setText(QApplication::translate("MainWindow", "mV", 0, QApplication::UnicodeUTF8));
    WidthL->setText(QApplication::translate("MainWindow", "Width", 0, QApplication::UnicodeUTF8));
    WidthE->setInputMask(QString());
    WidthE->setText(QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8));
    WidthU->setText(QApplication::translate("MainWindow", "ms", 0, QApplication::UnicodeUTF8));
    VRestL->setText(QApplication::translate("MainWindow", "VRest", 0, QApplication::UnicodeUTF8));
    VRestE->setInputMask(QString());
    VRestE->setText(QApplication::translate("MainWindow", "-50.0", 0, QApplication::UnicodeUTF8));
    VRestU->setText(QApplication::translate("MainWindow", "mV", 0, QApplication::UnicodeUTF8));
    ExplicitSpikeTimesL->setText(QApplication::translate("MainWindow", "Explicit Spike Times", 0, QApplication::UnicodeUTF8));
    NumberE->setInputMask(QString());
    NumberE->setText(QApplication::translate("MainWindow", "10", 0, QApplication::UnicodeUTF8));
    NumberL->setText(QApplication::translate("MainWindow", "Number", 0, QApplication::UnicodeUTF8));
    NumberU->setText(QApplication::translate("MainWindow", "/Burst", 0, QApplication::UnicodeUTF8));
    PeriodE->setInputMask(QString());
    PeriodE->setText(QApplication::translate("MainWindow", "500.0", 0, QApplication::UnicodeUTF8));
    PeriodL->setText(QApplication::translate("MainWindow", "Period", 0, QApplication::UnicodeUTF8));
    PeriodU->setText(QApplication::translate("MainWindow", "ms", 0, QApplication::UnicodeUTF8));
    SpikeTimesBut->setText(QApplication::translate("MainWindow", "Spike Times", 0, QApplication::UnicodeUTF8));
    ChannelL->setText(QApplication::translate("MainWindow", "Channel", 0, QApplication::UnicodeUTF8));
    ThresholdE->setInputMask(QString());
    ThresholdE->setText(QApplication::translate("MainWindow", "-40.0", 0, QApplication::UnicodeUTF8));
    ThresholdL->setText(QApplication::translate("MainWindow", "Threshold", 0, QApplication::UnicodeUTF8));
    ThresholdU->setText(QApplication::translate("MainWindow", "mV", 0, QApplication::UnicodeUTF8));
    NUnderE->setInputMask(QString());
    NUnderE->setText(QApplication::translate("MainWindow", "10", 0, QApplication::UnicodeUTF8));
    NUnderL->setText(QApplication::translate("MainWindow", "NUnder", 0, QApplication::UnicodeUTF8));
    NOverE->setInputMask(QString());
    NOverE->setText(QApplication::translate("MainWindow", "10", 0, QApplication::UnicodeUTF8));
    NOverL->setText(QApplication::translate("MainWindow", "NOver", 0, QApplication::UnicodeUTF8));
    STInputFileL->setText(QApplication::translate("MainWindow", "Spike Time Input File", 0, QApplication::UnicodeUTF8));
    STInputFileE->setText(QApplication::translate("MainWindow", "c:\\example.st", 0, QApplication::UnicodeUTF8));
    SGLUTableCombo->clear();
    SGLUTableCombo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Explicit Functions", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Lookup Tables", 0, QApplication::UnicodeUTF8)
    );
    SGbdChannelCombo->clear();
    SGbdChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8)
    );
    StartBut->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
    StopBut->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
    DAQComboBox->clear();
    DAQComboBox->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Simulated DAQ", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "DigiData 1200/1200A", 0, QApplication::UnicodeUTF8)
    );
    DAQL->setText(QApplication::translate("MainWindow", "Data Acqisition Board", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    menuConfig->setTitle(QApplication::translate("MainWindow", "Config", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIN_H
