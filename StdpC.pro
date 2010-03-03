# StdpC project file #

TARGET = 
DEPENDPATH += . src src/core src/drivers src/gui src/models \
              src/nidaqmx src/include lib
INCLUDEPATH += . src src/core src/drivers src/gui src/models \
               src/nidaqmx src/include lib

##INCLUDEPATH += "C:\Program Files\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include"         
#INCLUDEPATH += src/nidaqmx    

TEMPLATE = app
CONFIG -= debug_and_release
CONFIG += release
CONFIG += qt thread

# A directory to place intermediary build files.
RCC_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = src/gui/ui

# the sources
FORMS += mainwin.ui \
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
         abSynDlg.ui \
         MeasMethodDlg.ui
HEADERS += main.h \
           mainwin.h \
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
           pt_ioctl_tn.h \
           PortTalkX_IOCTL.h \
           SimulDAQDlg.h \
           DigiDataDlg.h \
           ChemSyn.h \
           GapJunction.h \
           HH.h \
           abHH.h \
           DAQ.h \
           SimulDAQ.h \
           SpkGen.h \
           Graph.h \
           GraphDlg.h \
           DAQDlg.h \
           AP.cc \
           scripting.h \
           abSynDlg.h \
           MeasMethodDlg.h \
           abSyn.h \
           AECChannel.h
## uncomment to compile with NIDAQmx support (needs NIDAQmx)           
# HEADERS += NIDAQDlg.h \
#            NIDAQ.h
     
SOURCES += main.cpp \
           mainwin.cpp \
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
           global.cpp \
           global_func.cpp \
           OutputChannelDlg.cpp \
           DigiData.cpp \
           Channels.cpp \
           SimulDAQDlg.cpp \
           DigiDataDlg.cpp \
           ChemSyn.cpp \
           GapJunction.cpp \
           HH.cpp \
           abHH.cpp \
           DAQ.cpp \
           SimulDAQ.cpp \
           SpkGen.cpp \
           Graph.cpp \
           GraphDlg.cpp \
           DAQDlg.cpp \
           ObjectDataTypes.cpp \
           scripting.cpp \
           abSynDlg.cpp \
           MeasMethodDlg.cpp \
           abSyn.cpp \
           AECChannel.cpp
          
## uncomment to compile with NIDAQmx support (needs NIDAQmx)           
#SOURCES += NIDAQDlg.cpp \
#           NIDAQ.cpp

LIBDIR += "C:\Qt\4.4.2\bin"
LIBS += staticlib/pt_ioctl_tn.a

## uncomment to compile with NIDAQmx support (needs NIDAQmx)           
# LIBS += staticlib/libnidaqmx.a
