# StdpC project file #
TARGET = StdpC
QT+= widgets
DEPENDPATH += $$PWD/ \
    $$PWD/src \
    $$PWD/src/core \
    $$PWD/src/drivers \
    $$PWD/src/gui \
    $$PWD/src/models \
    $$PWD/src/include \
    $$PWD/lib
INCLUDEPATH += $$PWD \
    $$PWD/src \
    $$PWD/src/core \
    $$PWD/src/drivers \
    $$PWD/src/gui \
    $$PWD/src/models \
    $$PWD/src/include \
    $$PWD/lib
TEMPLATE = app
CONFIG += qt \
    thread \
    debug_and_release

# A directory to place intermediary build files.
RCC_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
#UI_DIR = $$PWD/src/gui/ui

# the sources
FORMS += $$PWD/src/gui/MainWin.ui \
    $$PWD/src/gui/ChemSynDlg.ui \
    $$PWD/src/gui/GapJunctionDlg.ui \
    $$PWD/src/gui/STDPDlg.ui \
    $$PWD/src/gui/ODESTDPDlg.ui \
    $$PWD/src/gui/SpikeTimeDlg.ui \
    $$PWD/src/gui/InputChannelDlg.ui \
    $$PWD/src/gui/GraphDlg.ui \
    $$PWD/src/gui/HHDlg.ui \
    $$PWD/src/gui/AlphaBetaHHDlg.ui \
    $$PWD/src/gui/OutputChannelDlg.ui \
    $$PWD/src/gui/SimulDAQDlg.ui \
    $$PWD/src/gui/DigiDataDlg.ui \
    $$PWD/src/gui/ElectrodeCompDlg.ui \
    $$PWD/src/gui/AbSynDlg.ui \
    $$PWD/src/gui/DataSavingDlg.ui \
    $$PWD/src/gui/DestexheSynDlg.ui \
    $$PWD/src/gui/CurrentAssignmentDlg.ui
HEADERS += $$PWD/src/include/Main.h \
    $$PWD/src/include/Mainwin.h \
    $$PWD/src/include/ChemSynDlg.h \
    $$PWD/src/include/GapJunctionDlg.h \
    $$PWD/src/include/HHDlg.h \
    $$PWD/src/include/AlphaBetaHHDlg.h \
    $$PWD/src/include/DCThread.h \
    $$PWD/src/include/STDPDlg.h \
    $$PWD/src/include/ODESTDPDlg.h \
    $$PWD/src/include/SpikeTimeDlg.h \
    $$PWD/src/include/LUtables.h \
    $$PWD/src/include/InputChannelDlg.h \
    $$PWD/src/include/OutputChannelDlg.h \
    $$PWD/src/include/DigiData.h \
    $$PWD/src/include/Channels.h \
    $$PWD/src/include/Pt_ioctl_tn.h \
    $$PWD/src/include/PortTalkX_IOCTL.h \
    $$PWD/src/include/SimulDAQDlg.h \
    $$PWD/src/include/DigiDataDlg.h \
    $$PWD/src/include/ChemSyn.h \
    $$PWD/src/include/GapJunction.h \
    $$PWD/src/include/HH.h \
    $$PWD/src/include/AbHH.h \
    $$PWD/src/include/DAQ.h \
    $$PWD/src/include/SimulDAQ.h \
    $$PWD/src/include/SpkGen.h \
    $$PWD/src/include/Graph.h \
    $$PWD/src/include/GraphDlg.h \
    $$PWD/src/include/DAQDlg.h \
    $$PWD/src/include/AbSynDlg.h \
    $$PWD/src/include/ElectrodeCompDlg.h \
    $$PWD/src/include/AbSyn.h \
    $$PWD/src/include/AECChannel.h \
    $$PWD/src/include/KernelCalculator.h \
    $$PWD/src/include/Calibrator.h \
    $$PWD/src/include/Datasaver.h \
    $$PWD/src/include/DataSavingDlg.h \
    $$PWD/src/include/ObjectDataTypes.h \
    $$PWD/src/include/Global.h \
    $$PWD/src/include/DestexheSynDlg.h \
    $$PWD/src/include/DestexheSyn.h \
    $$PWD/src/include/AP.h \
    $$PWD/src/include/CurrentAssignmentDlg.h \
    $$PWD/src/include/ChannelListModel.h

SOURCES += $$PWD/src/core/Main.cpp \
    $$PWD/src/gui/MainWin.cpp \
    $$PWD/src/gui/ChemSynDlg.cpp \
    $$PWD/src/gui/GapJunctionDlg.cpp \
    $$PWD/src/gui/HHDlg.cpp \
    $$PWD/src/gui/AlphaBetaHHDlg.cpp \
    $$PWD/src/core/DCThread.cpp \
    $$PWD/src/gui/STDPDlg.cpp \
    $$PWD/src/gui/ODESTDPDlg.cpp \
    $$PWD/src/gui/SpikeTimeDlg.cpp \
    $$PWD/src/core/LUtables.cpp \
    $$PWD/src/gui/InputChannelDlg.cpp \
    $$PWD/src/core/Global.cpp \
    $$PWD/src/core/Global_func.cpp \
    $$PWD/src/gui/OutputChannelDlg.cpp \
    $$PWD/src/drivers/DigiData.cpp \
    $$PWD/src/core/Channels.cpp \
    $$PWD/src/gui/SimulDAQDlg.cpp \
    $$PWD/src/gui/DigiDataDlg.cpp \
    $$PWD/src/models/ChemSyn.cpp \
    $$PWD/src/models/GapJunction.cpp \
    $$PWD/src/models/HH.cpp \
    $$PWD/src/models/AbHH.cpp \
    $$PWD/src/drivers/DAQ.cpp \
    $$PWD/src/drivers/SimulDAQ.cpp \
    $$PWD/src/models/SpkGen.cpp \
    $$PWD/src/core/Graph.cpp \
    $$PWD/src/gui/GraphDlg.cpp \
    $$PWD/src/gui/DAQDlg.cpp \
    $$PWD/src/core/ObjectDataTypes.cpp \
    $$PWD/src/gui/AbSynDlg.cpp \
    $$PWD/src/gui/ElectrodeCompDlg.cpp \
    $$PWD/src/models/AbSyn.cpp \
    $$PWD/src/core/AECChannel.cpp \
    $$PWD/src/core/KernelCalculator.cpp \
    $$PWD/src/core/Calibrator.cpp \
    $$PWD/src/core/Datasaver.cpp \
    $$PWD/src/gui/DataSavingDlg.cpp \
    $$PWD/src/gui/DestexheSynDlg.cpp \
    $$PWD/src/models/DestexheSyn.cpp \
    $$PWD/src/core/AP.cpp \
    $$PWD/src/gui/CurrentAssignmentDlg.cpp \
    $$PWD/src/gui/ChannelListModel.cpp
LIBS += $$PWD/staticlib/pt_ioctl_tn.a

# uncomment to compile with NIDAQmx support (needs NIDAQmx)
DEPENDPATH += $$PWD/src/nidaqmx
SOURCES += $$PWD/src/gui/NIDAQDlg.cpp \
$$PWD/src/drivers/NIDAQ.cpp
FORMS += $$PWD/src/gui/NIDAQDlg.ui

HEADERS += $$PWD/src/include/NIDAQDlg.h \
$$PWD/src/include/NIDAQ.h

INCLUDEPATH += "C:\\Program Files (x86)\\National Instruments\\NI-DAQ\\DAQmx ANSI C Dev\\include"
#INCLUDEPATH += "C:\\Users\\tn41\\NI-DAQ\\DAQmx ANSI C Dev\\include"
INCLUDEPATH += $$PWD/src/nidaqmx

LIBS += $$PWD/staticlib/nidaqmx.a
