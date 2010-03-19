# StdpC project file #
TARGET =
DEPENDPATH += . \
    src \
    src/core \
    src/drivers \
    src/gui \
    src/models \
    src/nidaqmx \
    src/include \
    lib
#INCLUDEPATH += "C:\Program Files\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include"         
#INCLUDEPATH += src/nidaqmx    
INCLUDEPATH += . \
    src \
    src/core \
    src/drivers \
    src/gui \
    src/models \
    src/nidaqmx \
    src/include \
    lib
INCLUDEPATH += "C:\Program Files\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include"
TEMPLATE = app
CONFIG += qt \
    thread

# A directory to place intermediary build files.
RCC_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = src/gui/ui

# the sources
FORMS += MainWin.ui \
    ChemSynDlg.ui \
    GapJunctionDlg.ui \
    STDPDlg.ui \
    ODESTDPDlg.ui \
    SpikeTimeDlg.ui \
    InputChannelDlg.ui \
    GraphDlg.ui \
    HHDlg.ui \
    AlphaBetaHHDlg.ui \
    OutputChannelDlg.ui \
    SimulDAQDlg.ui \
    DigiDataDlg.ui \
##uncomment for NIDAQ use
#         NIDAQDlg.ui \
    NIDAQDlg.ui \
    ElectrodeCompDlg.ui \
    AbSynDlg.ui \
    DataSavingDlg.ui
HEADERS += Main.h \
    Mainwin.h \
    ChemSynDlg.h \
    GapJunctionDlg.h \
    HHDlg.h \
    AlphaBetaHHDlg.h \
    DCThread.h \
    STDPDlg.h \
    ODESTDPDlg.h \
    SpikeTimeDlg.h \
    LUtables.h \
    InputChannelDlg.h \
    OutputChannelDlg.h \
    DigiData.h \
    Channels.h \
    Pt_ioctl_tn.h \
    PortTalkX_IOCTL.h \
    SimulDAQDlg.h \
    DigiDataDlg.h \
    ChemSyn.h \
    GapJunction.h \
    HH.h \
    AbHH.h \
    DAQ.h \
    SimulDAQ.h \
    SpkGen.h \
    Graph.h \
    GraphDlg.h \
    DAQDlg.h \
    AP.cc \
    Scripting.h \
    AbSynDlg.h \
    ElectrodeCompDlg.h \
    AbSyn.h \
    AECChannel.h \
    KernelCalculator.h \
    Calibrator.h \
    Datasaver.h \
    DataSavingDlg.h

# # uncomment to compile with NIDAQmx support (needs NIDAQmx)
# HEADERS += NIDAQDlg.h \
# NIDAQ.h
SOURCES += Main.cpp \
    MainWin.cpp \
    ChemSynDlg.cpp \
    GapJunctionDlg.cpp \
    HHDlg.cpp \
    AlphaBetaHHDlg.cpp \
    DCThread.cpp \
    STDPDlg.cpp \
    ODESTDPDlg.cpp \
    SpikeTimeDlg.cpp \
    LUtables.cpp \
    InputChannelDlg.cpp \
    Global.cpp \
    Global_func.cpp \
    OutputChannelDlg.cpp \
    DigiData.cpp \
    Channels.cpp \
    SimulDAQDlg.cpp \
    DigiDataDlg.cpp \
    ChemSyn.cpp \
    GapJunction.cpp \
    HH.cpp \
    AbHH.cpp \
    DAQ.cpp \
    SimulDAQ.cpp \
    SpkGen.cpp \
    Graph.cpp \
    GraphDlg.cpp \
    DAQDlg.cpp \
    ObjectDataTypes.cpp \
    Scripting.cpp \
    AbSynDlg.cpp \
    ElectrodeCompDlg.cpp \
    AbSyn.cpp \
    AECChannel.cpp \
    KernelCalculator.cpp \
    Calibrator.cpp \
    Datasaver.cpp \
    DataSavingDlg.cpp

# # uncomment to compile with NIDAQmx support (needs NIDAQmx)
# SOURCES += NIDAQDlg.cpp \
# NIDAQ.cpp
LIBDIR += "C:\Qt\4.4.2\bin"
LIBS += staticlib/Pt_ioctl_tn.a

## uncomment to compile with NIDAQmx support (needs NIDAQmx)           
# LIBS += staticlib/libnidaqmx.a