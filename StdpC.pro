# StdpC project file #
TARGET = StdpC
QT+= widgets printsupport
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

CONFIG(release, debug|release): DEFINES += NDEBUG

static {
    QMAKE_LFLAGS += -static
}

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
    $$PWD/src/gui/HHModelDlg.ui \
    $$PWD/src/gui/ComponentWidget.ui \
    $$PWD/src/gui/ComponentFactoryWidget.ui \
    $$PWD/src/gui/DaqWidget.ui \
    $$PWD/src/gui/DaqFactoryWidget.ui \
    $$PWD/src/gui/SpikeGenDlg.ui
HEADERS += \
    $$PWD/src/include/Mainwin.h \
    $$PWD/src/include/ChemSynDlg.h \
    $$PWD/src/include/GapJunctionDlg.h \
    $$PWD/src/include/HHDlg.h \
    $$PWD/src/include/AlphaBetaHHDlg.h \
    $$PWD/src/include/DCThread.h \
    $$PWD/src/include/STDPDlg.h \
    $$PWD/src/include/ODESTDPDlg.h \
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
    $$PWD/src/include/Daq.h \
    $$PWD/src/include/SimulDAQ.h \
    $$PWD/src/include/SpkGen.h \
    $$PWD/src/include/GraphDlg.h \
    $$PWD/src/include/DAQDlg.h \
    $$PWD/src/include/AbSynDlg.h \
    $$PWD/src/include/ElectrodeCompDlg.h \
    $$PWD/src/include/AbSyn.h \
    $$PWD/src/include/AECChannel.h \
    $$PWD/src/include/KernelCalculator.h \
    $$PWD/src/include/Calibrator.h \
    $$PWD/src/include/DataSaver.h \
    $$PWD/src/include/DataSavingDlg.h \
    $$PWD/src/include/ObjectDataTypes.h \
    $$PWD/src/include/Global.h \
    $$PWD/src/include/DestexheSynDlg.h \
    $$PWD/src/include/DestexheSyn.h \
    $$PWD/src/include/AP.h \
    $$PWD/src/include/ChannelListModel.h \
    $$PWD/src/include/HHNeuron.h \
    $$PWD/src/include/HHModelDlg.h \
    $$PWD/src/include/ChannelIndex.h \
    $$PWD/src/include/AssignmentWidget.h \
    $$PWD/src/include/ComponentTable.h \
    $$PWD/src/include/ComponentWidget.h \
    $$PWD/src/include/Clock.h \
    $$PWD/src/include/DaqTable.h \
    $$PWD/src/include/DaqWidget.h \
    $$PWD/src/include/DeviceManager.h \
    $$PWD/src/include/ComponentPrototype.h \
    $$PWD/src/include/Component.h \
    $$PWD/src/include/WideComboBox.h \
    $$PWD/src/include/DaqOpts.h \
    $$PWD/src/include/DaqOptsPrototype.h \
    $$PWD/src/include/QCustomPlot.h \
    $$PWD/src/include/CircularFifo.h \
    $$PWD/src/include/SpikeGenDlg.h \
    $$PWD/src/include/ChannelBufferHelper.h \
    $$PWD/src/include/AssignmentCell.h \
    $$PWD/src/include/ModelManager.h \
    $$PWD/src/include/Model.h

SOURCES += $$PWD/src/core/Main.cpp \
    $$PWD/src/gui/MainWin.cpp \
    $$PWD/src/gui/ChemSynDlg.cpp \
    $$PWD/src/gui/GapJunctionDlg.cpp \
    $$PWD/src/gui/HHDlg.cpp \
    $$PWD/src/gui/AlphaBetaHHDlg.cpp \
    $$PWD/src/core/DCThread.cpp \
    $$PWD/src/gui/STDPDlg.cpp \
    $$PWD/src/gui/ODESTDPDlg.cpp \
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
    $$PWD/src/drivers/Daq.cpp \
    $$PWD/src/drivers/SimulDAQ.cpp \
    $$PWD/src/models/SpkGen.cpp \
    $$PWD/src/gui/GraphDlg.cpp \
    $$PWD/src/gui/DAQDlg.cpp \
    $$PWD/src/gui/AbSynDlg.cpp \
    $$PWD/src/gui/ElectrodeCompDlg.cpp \
    $$PWD/src/models/AbSyn.cpp \
    $$PWD/src/core/AECChannel.cpp \
    $$PWD/src/core/KernelCalculator.cpp \
    $$PWD/src/core/Calibrator.cpp \
    $$PWD/src/core/DataSaver.cpp \
    $$PWD/src/gui/DataSavingDlg.cpp \
    $$PWD/src/gui/DestexheSynDlg.cpp \
    $$PWD/src/models/DestexheSyn.cpp \
    $$PWD/src/core/AP.cpp \
    $$PWD/src/gui/ChannelListModel.cpp \
    $$PWD/src/models/HHNeuron.cpp \
    $$PWD/src/gui/HHModelDlg.cpp \
    $$PWD/src/core/ChannelIndex.cpp \
    $$PWD/src/gui/ComponentTable.cpp \
    $$PWD/src/gui/ComponentWidget.cpp \
    $$PWD/src/drivers/Clock.cpp \
    $$PWD/src/gui/DaqTable.cpp \
    $$PWD/src/gui/DaqWidget.cpp \
    $$PWD/src/core/DeviceManager.cpp \
    $$PWD/src/gui/WideComboBox.cpp \
    $$PWD/src/gui/QCustomPlot.cpp \
    $$PWD/src/gui/SpikeGenDlg.cpp \
    $$PWD/src/core/ChannelBufferHelper.cpp \
    $$PWD/src/core/ModelManager.cpp \
    $$PWD/src/models/Model.cpp
LIBS += $$PWD/staticlib/pt_ioctl_tn.a

nidaqmx {
    # NIDAQmx static build based on NI DAQmx 15.5.1
    DEPENDPATH += $$PWD/src/nidaqmx
    SOURCES += $$PWD/src/gui/NIDAQDlg.cpp \
    $$PWD/src/drivers/Nidaq.cpp
    FORMS += $$PWD/src/gui/NIDAQDlg.ui

    HEADERS += $$PWD/src/include/NIDAQDlg.h \
    $$PWD/src/include/Nidaq.h

    INCLUDEPATH += $$PWD/src/nidaqmx
    LIBS += $$PWD/src/nidaqmx/nidaqmx.a
    DEFINES += NATIONAL_INSTRUMENTS
}
